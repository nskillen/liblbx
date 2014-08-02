#ifndef _OM2_LIBLBX_LBXPACKEDFILE_
#define _OM2_LIBLBX_LBXPACKEDFILE_

#include <cstdint>
#include <vector>

namespace OpenMOO2 {
namespace LBX {

class LBXPackedFile {
public:
    LBXPackedFile();
    LBXPackedFile(uint32_t, uint32_t, std::vector<char>);

    uint32_t starting_offset() const;
    void starting_offset(uint32_t);

    uint32_t ending_offset() const;
    void ending_offset(uint32_t);

    std::vector<char> data();
    void data(std::vector<char>);

    size_t size() const;

private:
    uint32_t _starting_offset;
    uint32_t _ending_offset;
    std::vector<char> _data;
};

}}

#endif
