#include "LBXPaletteEntry.h"

using namespace OpenMOO2::LBX;

LBXPaletteEntry::LBXPaletteEntry()
: _a(1), _r(0), _g(0), _b(0)
{}

LBXPaletteEntry::LBXPaletteEntry(const LBXPaletteEntry & other)
: _a(other._a), _r(other._r), _g(other._g), _b(other._b)
{}

LBXPaletteEntry::LBXPaletteEntry(LBXPaletteEntry && other)
: _a(other._a), _r(other._r), _g(other._g), _b(other._b)
{}

LBXPaletteEntry::LBXPaletteEntry(const LBXPalette & palette, size_t idx)
: _a(1), _r(0), _g(0), _b(0)
{
	LBXPaletteEntry other = palette.get_palette_entry(idx);
	_a = other.a();
	_r = other.r();
	_g = other.g();
	_b = other.b();
}

LBXPaletteEntry::LBXPaletteEntry(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
: _a(a), _r(r), _g(g), _b(b)
{}

uint8_t LBXPaletteEntry::a() const { return _a; }
uint8_t LBXPaletteEntry::r() const { return _r; }
uint8_t LBXPaletteEntry::g() const { return _g; }
uint8_t LBXPaletteEntry::b() const { return _b; }

void LBXPaletteEntry::a(uint8_t a) { _a = a; }
void LBXPaletteEntry::r(uint8_t r) { _r = r; }
void LBXPaletteEntry::g(uint8_t g) { _g = g; }
void LBXPaletteEntry::b(uint8_t b) { _b = b; }
