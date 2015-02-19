#include "Mesh.h"

#include <SFML/OpenGL.hpp>
#include <iostream>
#include <memory>

namespace
{
	void DrawCuboid(float xx, float yy, float zz)
	{
		float x = xx / 2;
		float y = yy / 2;
		float z = zz / 2;

		glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);			// f
		glVertex3f(-x, -y, z);
		glVertex3f(x, -y, z);
		glVertex3f(x, y, z);
		glVertex3f(-x, y, z);

		glNormal3f(1, 0, 0);			// r
		glVertex3f(x, -y, z);
		glVertex3f(x, -y, -z);
		glVertex3f(x, y, -z);
		glVertex3f(x, y, z);

		glNormal3f(0, 0, -1);			// b
		glVertex3f(x, -y, -z);
		glVertex3f(-x, -y, -z);
		glVertex3f(-x, y, -z);
		glVertex3f(x, y, -z);

		glNormal3f(-1, 0, 0);			// r
		glVertex3f(-x, -y, -z);
		glVertex3f(-x, -y, z);
		glVertex3f(-x, y, z);
		glVertex3f(-x, y, -z);

		glNormal3f(0, 1, 0);			// top
		glVertex3f(-x, y, z);
		glVertex3f(x, y, z);
		glVertex3f(x, y, -z);
		glVertex3f(-x, y, -z);

		glNormal3f(0, -1, 0);			// top
		glVertex3f(-x, -y, -z);
		glVertex3f(x, -y, -z);
		glVertex3f(x, -y, z);
		glVertex3f(-x, -y, z);

		glEnd();
	}
}

void Mesh1::Draw() const
{
	float n = 256;
	float t = 64;
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-n, 0, -n);
	glTexCoord2f(t, 0);
	glVertex3f(-n, 0, n);
	glTexCoord2f(t, t);
	glVertex3f(n, 0, n);
	glTexCoord2f(0, t);
	glVertex3f(n, 0, -n);
	glEnd();
}
