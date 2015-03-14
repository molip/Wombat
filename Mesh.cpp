#include "Mesh.h"

#include <SFML/OpenGL.hpp>
#include <iostream>
#include <memory>

namespace
{
	void DrawCuboid(double xx, double yy, double zz)
	{
		double x = xx / 2;
		double y = yy / 2;
		double z = zz / 2;

		glBegin(GL_QUADS);
		glNormal3d(0, 0, 1);			// d
		glVertex3d(-x, -y, z);
		glVertex3d(x, -y, z);
		glVertex3d(x, y, z);
		glVertex3d(-x, y, z);

		glNormal3d(1, 0, 0);			// r
		glVertex3d(x, -y, z);
		glVertex3d(x, -y, -z);
		glVertex3d(x, y, -z);
		glVertex3d(x, y, z);

		glNormal3d(0, 0, -1);			// b
		glVertex3d(x, -y, -z);
		glVertex3d(-x, -y, -z);
		glVertex3d(-x, y, -z);
		glVertex3d(x, y, -z);

		glNormal3d(-1, 0, 0);			// r
		glVertex3d(-x, -y, -z);
		glVertex3d(-x, -y, z);
		glVertex3d(-x, y, z);
		glVertex3d(-x, y, -z);

		glNormal3d(0, 1, 0);			// top
		glVertex3d(-x, y, z);
		glVertex3d(x, y, z);
		glVertex3d(x, y, -z);
		glVertex3d(-x, y, -z);

		glNormal3d(0, -1, 0);			// top
		glVertex3d(-x, -y, -z);
		glVertex3d(x, -y, -z);
		glVertex3d(x, -y, z);
		glVertex3d(-x, -y, z);

		glEnd();
	}
}

void Mesh1::Draw() const
{
	double n = 256;
	double t = 64;
	glBegin(GL_QUADS);
	glNormal3d(0, 1, 0);
	glTexCoord2d(0, 0);
	glVertex3d(-n, 0, -n);
	glTexCoord2d(t, 0);
	glVertex3d(-n, 0, n);
	glTexCoord2d(t, t);
	glVertex3d(n, 0, n);
	glTexCoord2d(0, t);
	glVertex3d(n, 0, -n);
	glEnd();
}
