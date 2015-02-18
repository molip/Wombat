#include "Sprite.h"
#include "Texture.h"
#include "Matrix.h"
#include "Vector.h"
#include "Planet.h"

#include <SFML/OpenGL.hpp>

Sprite::Sprite()
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
	DrawMesh();
	glPopMatrix();
}
