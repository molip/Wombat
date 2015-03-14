#include "Sprite.h"
#include "Planet.h"

#include <SFML/OpenGL.hpp>

Sprite::Sprite() : m_rotation(0)
{
}

Sprite::~Sprite()
{
}

void Sprite::Update(double tDelta)
{
}

void Sprite::Draw() const
{
	glPushMatrix();
	glTranslated(double(m_pos.x), double(m_pos.y), double(m_pos.z));
	glRotated(m_rotation, 0, 1, 0);
	DrawMesh();
	glPopMatrix();
}
