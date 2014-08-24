#ifndef _OM2_LIBLBX_LBXFILE_
#define _OM2_LIBLBX_LBXFILE_

#include <cstdint>
#include <string>
#include <vector>

/**
 * This file contains the necessary information to decode a MOO2 LBX File.
 * The details of the LBX file structure were taken from:
 * http://apolyton.net/showthread.php/113794-The-MOO2-LBX-format-explained
 * and from:
 * https://web.archive.org/web/20050310012845/http://www.xprt.net/~s8/prog/orion2/lbx/
 **/

/**
 * LBX File header structure is as follows:
 * (each line is 1b)
 * ----------------
 * | Number of files (n)
 * |
 * ----------------
 * | Verification Code
 * |
 * |
 * |
 * ----------------
 * | Version
 * |
 * ----------------
 * | Offset 1
 * |
 * |
 * |
 * ----------------
 * | Offset 2
 * |
 * |
 * |
 * ----------------
 * | ...
 * ----------------
 * | Offset n
 * |
 * |
 * |
 * ----------------
 * | Offset n+1
 * | aka
 * | End of last file
 * |
 * ----------------
 * | possible junk space (arbitrary size, may not be present)
 * ----------------
 * | file data
 * | .
 * | .
 * | .
 * ----------------
 * (eof)
 **/

namespace OpenMOO2 {
namespace LBX {

const uint16_t LBX_DEFAULT_VERSION     = 0x00;
const uint32_t VERIFICATION_CODE_VALUE = 0xFEAD;
const uint32_t EOF_MARK                = 0x03E8;
const size_t   LBX_HEADER_SIZE         = 0x08;
const size_t   LBX_OFFSET_SIZE         = 0x04;

class LBXFile {
public:
    LBXFile();
    LBXFile(const LBXFile&);
    LBXFile(LBXFile&&);
    LBXFile(std::string);

    void serialize() const;
    void deserialize(std::string);

    std::string filename() const;
    void filename(std::string filename);

    uint16_t version() const;
    void version(uint16_t);

    size_t size() const;

    size_t num_packed_files() const;
    std::vector<char> get_file_number(size_t) const;

    void pack_file(std::string);
    void pack_file(std::string, size_t);

    void remove_file(size_t);

    LBXFile & operator=(const LBXFile&);

private:
    std::string _filename;
    uint16_t _version;
    std::vector<char> junk_data;
    std::vector<std::vector<char>> files;
};

}}

#endif
