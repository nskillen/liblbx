#include "LBXPackedFile.h"

using namespace OpenMOO2::LBX;

LBXPackedFile::LBXPackedFile() : _starting_offset(0), _ending_offset(0), _data(0) {}

LBXPackedFile::LBXPackedFile(uint32_t starting_offset, uint32_t ending_offset, std::vector<char> data)
: _starting_offset(starting_offset), _ending_offset(ending_offset), _data(data) {}

uint32_t LBXPackedFile::starting_offset() const {
    return _starting_offset;
}

void LBXPackedFile::starting_offset(uint32_t starting_offset) {
    _starting_offset = starting_offset;
}

uint32_t LBXPackedFile::ending_offset() const {
    return _ending_offset;
}

void LBXPackedFile::ending_offset(uint32_t ending_offset) {
    _ending_offset = ending_offset;
}

std::vector<char> LBXPackedFile::data() {
    return _data;
}

void LBXPackedFile::data(std::vector<char> data) {
    _data = data;
}

size_t LBXPackedFile::size() const {
    return _data.size();
}
