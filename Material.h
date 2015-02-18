#pragma once

#include "Colour.h"

class Material 
{
public:
	Material();
	void Apply();

	Colour ambient, diffuse, specular, emission;
	int shininess;
};
