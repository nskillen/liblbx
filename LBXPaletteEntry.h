#ifndef _OM2_LBX_PALETTE_ENTRY_
#define _OM2_LBX_PALETTE_ENTRY_

#include "LBXPalette.h"

namespace OpenMOO2 {
namespace LBX {

class LBXPalette;

class LBXPaletteEntry {
public:
	LBXPaletteEntry();
	LBXPaletteEntry(const LBXPaletteEntry&);
	LBXPaletteEntry(LBXPaletteEntry&&);
	LBXPaletteEntry(const LBXPalette&,size_t index);
	LBXPaletteEntry(uint8_t,uint8_t,uint8_t,uint8_t);

	uint8_t a() const;
	void a(uint8_t);

	uint8_t r() const;
	void r(uint8_t);

	uint8_t g() const;
	void g(uint8_t);

	uint8_t b() const;
	void b(uint8_t);
private:
	uint8_t _a, _r, _g, _b;
};

}}
#endif // OM2_LBX_PALETTE_ENTRY
