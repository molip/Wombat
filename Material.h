#pragma once

#include "Colour.h"

class Material 
{
public:
	Material();
	void Apply() const;

	Colour ambient, diffuse, specular, emission;
	int shininess;
};
