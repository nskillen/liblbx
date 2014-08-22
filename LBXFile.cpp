#include "LBXFile.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <new>
#include <stdexcept>

using namespace OpenMOO2::LBX;

std::vector<char> get_file_contents(std::string);

/**
 * Reads an LBX file from disk, and returns a de-serialized LBXFile object
 * Params:
 * filename - the name of the file to read
 **/
LBXFile LBXFile::read_file(std::string filename) {
    LBXFile lbx;
    lbx.filename = filename;
    lbx.deserialize(get_file_contents(filename));
    return lbx;
}


/**
 * Creates a new LBX file.
 * Params:
 * filename - the name of the LBX file to create
 * files_to_pack - the names of the files to be packed into the LBX file
 * version - the version of the LBX file, default is 0
 **/
void LBXFile::write_file(std::string filename, LBXFile & lbx) {
    write_file(filename, &lbx);
}

void LBXFile::write_file(std::string filename, LBXFile * lbx) {
    std::ofstream file(filename, std::ios::trunc|std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open " + filename + " for writing");
    }

    std::vector<char> lbx_data = lbx->serialize();
    file.write(lbx_data.data(), lbx_data.size());

    if (file.fail()) {
        file.close();
        throw std::runtime_error("Error while writing to file: " + filename);
    }

    file.flush();
    file.close();
}

/**
 * Serializes the LBX file object into a bytestream
 **/
const std::vector<char> LBXFile::serialize() const {
    std::vector<char> lbx_data;
    size_t file_size = 0;

    file_size += LBX_HEADER_SIZE; // fixed portion of header

    file_size += 4 * (files.size()+1); // +4 bytes per file, with one extra

    for (LBXPackedFile* packed_file : files) { // plus all the file sizes
        file_size += packed_file->size();
    }

    file_size = std::max(static_cast<uint32_t>(file_size), files[files.size()-1]->ending_offset());

    lbx_data.resize(file_size); // set vector to appropriate size

    // first two bytes are number of files
    *reinterpret_cast<uint16_t*>(lbx_data.data()) = files.size();
    // next four bytes are LBX signature 0xFEAD
    *reinterpret_cast<uint32_t*>(lbx_data.data()+2) = VERIFICATION_CODE_VALUE;
    // next two bytes are the version number
    *reinterpret_cast<uint16_t*>(lbx_data.data()+6) = version;

    size_t file_num = 0;
    for (auto file : files) {
        // now four bytes indicate the start offset for each file in the LBX
        *reinterpret_cast<uint32_t*>(lbx_data.data() + 8 + (4 * file_num)) = file->starting_offset();
        file_num += 1;

        // and while we're here, let's copy the packed file data over
        std::copy(file->data().begin(), file->data().end(), lbx_data.begin() + file->starting_offset());
    }

    // and 4 bytes for the end of the last file
    *reinterpret_cast<uint32_t*>(lbx_data.data() + 8 + (4 * file_num)) = file_size;

    // everything is copied now, let's give it back
    return lbx_data;
}

/**
 * De-serializes the LBX file from a bytestream into the object
 * Params:
 * lbx_data - the bytestream to deserialize
 **/
void LBXFile::deserialize(std::vector<char> lbx_data) {
    version = lbx_data[6] + static_cast<uint16_t>(256 * lbx_data[7]);

    uint32_t data_sig = *reinterpret_cast<uint32_t*>(lbx_data.data()+2);

    if (VERIFICATION_CODE_VALUE != data_sig) {
        throw std::runtime_error("Provided data signature is incorrect");
    }

    size_t num_files = *reinterpret_cast<uint16_t*>(lbx_data.data());

    files.resize(num_files, nullptr);

    for (size_t i = 0; i < num_files; ++i) {
        files[i] = new LBXPackedFile();
        files[i]->starting_offset(*reinterpret_cast<uint32_t*>(lbx_data.data()+8+(4*i)));
        files[i]->ending_offset(*reinterpret_cast<uint32_t*>(lbx_data.data()+8+(4*(i+1))));
        std::copy(lbx_data.begin()+files[i]->starting_offset(), lbx_data.begin()+files[i]->ending_offset(), files[i]->data().begin());
    }
}

std::string LBXFile::get_filename() const {
    return filename;
}

size_t LBXFile::size() const {
    size_t file_size = 0;
    file_size += 8; // fixed header size
    file_size += 4 * files.size(); // 4 bytes for each file start offset
    if (files.size() > 0) {
        file_size += 1; // plus 1 because each file has an end offset (that is the start offset of the next file)
        for (auto file : files) {
            file_size += file->size(); // plus the size of the individual files
        }
    }
    return file_size;
}

size_t LBXFile::num_packed_files() const {
    return files.size();
}

LBXPackedFile* LBXFile::get_file_number(size_t file_num) const {
    return files[file_num];
}

void LBXFile::pack_file(std::string filename) {
    pack_file(filename, files.size());
}

void LBXFile::pack_file(std::string filename, size_t file_num) {
    std::vector<char> file_data = get_file_contents(filename);

    auto files_iter = files.begin() + file_num - (file_num > 0 ? 1 : 0);

    LBXPackedFile* packed_file = new LBXPackedFile();
    packed_file->starting_offset(file_num > 0 ? (*files_iter)->ending_offset() : 0);
    packed_file->ending_offset(packed_file->starting_offset() + file_data.size());
    packed_file->data(file_data);

    uint32_t new_offset = packed_file->ending_offset();

    files.insert(files.begin() + file_num, packed_file);
    for (auto iter = files.begin() + file_num + 1; iter != files.end(); ++iter) {
        (*iter)->starting_offset(new_offset);
        new_offset += (*iter)->size();
        (*iter)->ending_offset(new_offset);
    }
}

void LBXFile::remove_file(size_t file_num) {
    files.erase(files.begin() + file_num);

    auto new_offset = 0;
    if (file_num > 0) {
        new_offset = files[file_num-1]->ending_offset();
    }

    for (auto iter = files.begin() + file_num + 1; iter != files.end(); ++iter) {
        (*iter)->starting_offset(new_offset);
        new_offset += (*iter)->size();
        (*iter)->ending_offset(new_offset);
    }
}

std::vector<char> get_file_contents(std::string filename) {
    std::vector<char> file_contents;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: "+filename);
    }

    file.seekg(0, std::ifstream::end);
    size_t file_size = file.tellg();
    file.seekg(0);

    file_contents.resize(file_size);
    file.read(file_contents.data(), file_size);

    if (file.fail()) {
        throw std::runtime_error("Unable to read "+std::to_string(file_size)+" bytes from file: "+filename);
    }

    return file_contents;
}
