/*
	Author: Eric Winebrenner
*/


#pragma once
#include "mesh.h"

namespace ew {
	MeshData createCube(float size);
	MeshData createPlane(float width, float height, int subDivisions);
}