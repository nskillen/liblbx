#include "LBXFile.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <new>
#include <stdexcept>

std::vector<char> get_file_contents(std::string);

using namespace OpenMOO2::LBX;

LBXFile::LBXFile()
: _filename(""), _version(LBX_DEFAULT_VERSION), junk_data(0), files(0)
{
	junk_data.resize(0);
	files.resize(0);
}

LBXFile::LBXFile(const LBXFile & other)
: _filename(other._filename), _version(other._version), junk_data(other.junk_data), files(other.files)
{
	junk_data.resize(other.junk_data.size());
	files.resize(other.files.size());
}

LBXFile::LBXFile(LBXFile && other)
: _filename(other._filename), _version(other._version), junk_data(other.junk_data), files(other.files)
{
	junk_data.resize(other.junk_data.size());
	files.resize(other.files.size());
}

LBXFile::LBXFile(std::string filename)
: _filename(filename), _version(LBX_DEFAULT_VERSION), junk_data(0), files(0)
{
	junk_data.resize(0);
	files.resize(0);
    deserialize(filename);
}

/**
 * Serializes the LBX file object into a file on disk
 **/
void LBXFile::serialize() const {
    if (0 == _filename.compare("")) {
        throw std::runtime_error("Cannot serialize without a filename");
    }

    if (0 == files.size()) {
        throw std::runtime_error("Must pack at least one file to serialize LBX");
    }

    std::vector<char> lbx_data;
    size_t file_size = 0;
    uint32_t offset;

    file_size += LBX_HEADER_SIZE; // fixed portion of header
    file_size += LBX_OFFSET_SIZE * (files.size()+1); // +4 bytes per file, with one extra
    file_size += junk_data.size();
    offset = file_size; // initial file offset is just past the end of the header
    for (const std::vector<char> & packed_file : files) { // plus all the file sizes
        file_size += packed_file.size();
    }
    lbx_data.resize(file_size); // set vector to appropriate size

    // first two bytes are number of files
    *reinterpret_cast<uint16_t*>(lbx_data.data()) = files.size();
    // next four bytes are LBX signature 0xFEAD
    *reinterpret_cast<uint32_t*>(lbx_data.data()+2) = VERIFICATION_CODE_VALUE;
    // next two bytes are the version number
    *reinterpret_cast<uint16_t*>(lbx_data.data()+6) = _version;

    std::copy(junk_data.begin(), junk_data.end(), lbx_data.begin()+(offset-junk_data.size()));

    size_t file_num = 0;
    for (auto file : files) {
        // now four bytes indicate the start offset for each file in the LBX
        *reinterpret_cast<uint32_t*>(lbx_data.data() + LBX_HEADER_SIZE + (LBX_OFFSET_SIZE * file_num)) = offset;
        file_num += 1;

        // and while we're here, let's copy the packed file data over
        std::copy(file.begin(), file.end(), lbx_data.begin() + offset);
        offset += file.size();
    }
    // and 4 bytes for the end of the last file
    *reinterpret_cast<uint32_t*>(lbx_data.data() + LBX_HEADER_SIZE + (LBX_OFFSET_SIZE * file_num)) = offset;

    // everything is copied now, let's write it to disk
    std::ofstream file(_filename, std::ios::binary|std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file "+_filename+" for writing");
    }

    file.write(lbx_data.data(), lbx_data.size());
    file.flush();
    file.close();
}

/**
 * De-serializes the LBX file from a file into the object
 * Params:
 * filename - the LBX file to deserialize
 **/
void LBXFile::deserialize(std::string filename) {
    auto data = get_file_contents(filename); // read file

	// validate file signature
    uint32_t data_sig = *reinterpret_cast<uint32_t*>(data.data()+2);
    if (VERIFICATION_CODE_VALUE != data_sig) {
        throw std::runtime_error("Provided data signature is incorrect");
    }

	// get file version
    _version = data[6] + static_cast<uint16_t>(256 * data[7]);

	// get number of files
    size_t num_files = *reinterpret_cast<uint16_t*>(data.data());
    files.resize(num_files);

    uint32_t hdr_end = LBX_HEADER_SIZE + (LBX_OFFSET_SIZE * (files.size()+1));
    uint32_t files_begin = *reinterpret_cast<uint32_t*>(data.data()+LBX_HEADER_SIZE);
    junk_data.resize(files_begin - hdr_end);
    std::copy(data.begin()+hdr_end, data.begin()+files_begin, junk_data.begin());

    for (size_t i = 0; i < num_files; ++i) {
        uint32_t start = *reinterpret_cast<uint32_t*>(data.data()+LBX_HEADER_SIZE+(LBX_OFFSET_SIZE*i));
        uint32_t end = *reinterpret_cast<uint32_t*>(data.data()+LBX_HEADER_SIZE+(LBX_OFFSET_SIZE*(i+1)));
        files[i].resize(end-start);
        std::copy(data.begin()+start, data.begin()+end, files[i].begin());
    }
    _filename = filename;
}

std::string LBXFile::filename() const {
    return _filename;
}

void LBXFile::filename(std::string filename) {
	_filename = filename;
}

uint16_t LBXFile::version() const {
	return _version;
}

void LBXFile::version(uint16_t version) {
	_version = version;
}

size_t LBXFile::size() const {
    if (0 == _filename.compare("")) { return 0; }
    size_t file_size = 0;
    file_size += LBX_HEADER_SIZE; // fixed header size
    file_size += LBX_OFFSET_SIZE * (files.size()+1); // 4 bytes for each file start offset, plus 4 bytes for final offset
    file_size += junk_data.size(); // file may contain junk after offsets, but before file data
    if (files.size() > 0) {
        for (auto file : files) {
            file_size += file.size(); // plus the size of the individual files
        }
    }
    return file_size;
}

size_t LBXFile::num_packed_files() const {
    return files.size();
}

std::vector<char> LBXFile::get_file_number(size_t file_num) const {
    if (files.size() <= file_num) {
        throw std::runtime_error("Invalid file number specified");
    }

    return files[file_num];
}

void LBXFile::pack_file(std::string filename) {
    pack_file(filename, files.size());
}

void LBXFile::pack_file(std::string filename, size_t file_num) {
    files.insert(files.begin() + file_num, get_file_contents(filename));
}

void LBXFile::remove_file(size_t file_num) {
    if (files.size() <= file_num) {
        throw std::runtime_error("Invalid file number specified for removal");
    }
    files.erase(files.begin() + file_num);
}

LBXFile & LBXFile::operator=(const LBXFile & other) {
    _filename = other._filename;
    _version = other._version;
    junk_data.resize(other.junk_data.size());
    std::copy(other.junk_data.begin(), other.junk_data.end(), junk_data.begin());
    files.resize(other.files.size());
    std::copy(other.files.begin(), other.files.end(), files.begin());
    return *this;
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
