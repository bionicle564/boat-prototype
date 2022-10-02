#pragma once

struct sVertexData
{
	float x, y, z, w; //position
	float nx, ny, nz, nw; //normals
	float u0, v0, u1, v1; //UVs
	int bId1, bId2, bId3, bId4; //bone IDs
	float bWeight1, bWeight2, bWeight3, bWeight4; //bone wieghtss

};