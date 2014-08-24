#ifndef _OM2_LBX_PALETTE_
#define _OM2_LBX_PALETTE_

#include "LBXFile.h"
#include "LBXPaletteEntry.h"

namespace OpenMOO2 {
namespace LBX {

class LBXPaletteEntry;

class LBXPalette {
public:
	LBXPalette();
	LBXPalette(const LBXPalette&);
	LBXPalette(LBXPalette&&);
	LBXPalette(const LBXFile&, size_t);

	const LBXPaletteEntry & get_palette_entry(size_t) const;
	void override_entry(size_t, const LBXPaletteEntry&);
private:
	std::vector<LBXPaletteEntry> palette_entries;
};

}}
#endif // _OM2_LBX_PALETTE_
