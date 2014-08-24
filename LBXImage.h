#ifndef _OM2_LBX_IMAGE_
#define _OM2_LBX_IMAGE_

#include "LBXFile.h"
#include "LBXPalette.h"

namespace OpenMOO2 {
namespace LBX {

class LBXImage {
public:
	LBXImage();
	LBXImage(const LBXImage&);
	LBXImage(LBXImage&&);
	LBXImage(const LBXFile&,size_t,const LBXPalette&);
};

}}
#endif // _OM2_LBX_IMAGE_
