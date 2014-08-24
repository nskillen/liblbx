#include "LBXPalette.h"

#include <stdexcept>

using namespace OpenMOO2::LBX;

LBXPalette::LBXPalette()
: palette_entries()
{}

LBXPalette::LBXPalette(const LBXPalette & other)
: palette_entries(other.palette_entries)
{
	palette_entries.resize(other.palette_entries.size());
}

LBXPalette::LBXPalette(LBXPalette && other)
: palette_entries(other.palette_entries)
{
	palette_entries.resize(other.palette_entries.size());
}

LBXPalette::LBXPalette(const LBXFile & lbx, size_t offset)
: palette_entries(0)
{
	//TODO: implement this
}

const LBXPaletteEntry & LBXPalette::get_palette_entry(size_t idx) const {
	if (palette_entries.size() <= idx) {
		throw std::runtime_error("attempt to get non-existent palette entry");
	}

	return palette_entries[idx];
}

void LBXPalette::override_entry(size_t idx, const LBXPaletteEntry & entry) {
	if (palette_entries.size() <= idx) {
		throw std::runtime_error("attempt to override a non-existent palette entry");
	}

	palette_entries[idx]
}
