#include "Mesh.h"
#include "Matrix.h"
#include "Vector.h"

#include <SFML/OpenGL.hpp>
#include <iostream>

void Mesh1::Draw() const
{
	float n = 600;
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1, 0);
	glVertex3f(n, 0, 0);
	glTexCoord2f(1, 1);
	glVertex3f(n, n, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0, n, 0);
	glEnd();
}

