#include "Sprite.h"
#include "Planet.h"

#include <SFML/OpenGL.hpp>

Sprite::Sprite() : m_rotation(0)
{
}

Sprite::~Sprite()
{
}

void Sprite::Update(float tDelta)
{
}

void Sprite::Draw() const
{
	glPushMatrix();
	glTranslatef(float(m_pos.x), float(m_pos.y), float(m_pos.z));
	glRotatef(m_rotation, 0, 1, 0);
	DrawMesh();
	glPopMatrix();
}
