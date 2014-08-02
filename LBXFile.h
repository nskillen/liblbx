#ifndef _OM2_LIBLBX_LBXFILE_
#define _OM2_LIBLBX_LBXFILE_

#include <cstdint>
#include <string>
#include <vector>

#include "LBXPackedFile.h"

/**
 * This file contains the necessary information to decode a MOO2 LBX File.
 * The details of the LBX file structure were taken from:
 * http://apolyton.net/showthread.php/113794-The-MOO2-LBX-format-explained
 * and from:
 * https://web.archive.org/web/20050310012845/http://www.xprt.net/~s8/prog/orion2/lbx/
 **/

namespace OpenMOO2 {
namespace LBX {

const uint16_t LBX_DEFAULT_VERSION = 0x00;
const uint32_t VERIFICATION_CODE_VALUE = 0xFEAD;
const uint32_t EOF_MARK = 0x03E8;
const size_t LBX_HEADER_SIZE = 0x08;

class LBXFile {
public:
    static LBXFile read_file(std::string);
    static LBXFile create_lbx(std::string, std::vector<std::string>);

    void serialize(std::string) const;
    const std::vector<char> serialize() const;

    void deserialize(std::string);
    void deserialize(std::vector<char>);

    size_t size() const;
    size_t num_packed_files() const;
    LBXPackedFile* get_file_number(size_t) const;
    void pack_file(std::string);
    void pack_file(std::string, size_t);
    void remove_file(size_t);

private:
    uint16_t version;
    std::vector<LBXPackedFile*> files;
};

}}

#endif
