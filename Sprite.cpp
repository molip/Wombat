#include "Sprite.h"
#include "Texture.h"
#include "Matrix.h"
#include "Vector.h"
#include "Planet.h"

#include <SFML/OpenGL.hpp>

Sprite::Sprite(float scale, float alpha) : m_scale(scale), m_alpha(alpha), m_rotation(0), m_footprintScale(1, 1)
{
}

Sprite::~Sprite()
{
}

void Sprite::SetAnimation(const sf::Texture& texture, int nFrames, float fps)
{
	m_pAnim.reset(new Animation(texture, nFrames, fps));
}

void Sprite::SetAnimation(const char* sPath, int nFrames, float fps)
{
	SetAnimation(Texture::Get(sPath), nFrames, fps);
}

void Sprite::Update(float tDelta)
{
	if (m_pAnim)
		m_pAnim->Update(tDelta);
}

bool Sprite::operator <(const Sprite& rhs) const
{
	return m_pos.y > rhs.m_pos.y;
}

void Sprite::Draw() const
{
	if (!m_pAnim)
		return;
	
	glPushMatrix();

	auto sz = m_pAnim->GetSize();

	glTranslatef(float(m_pos.x), float(m_pos.y + m_pos.z), 0);
	glScalef(m_scale, m_scale * sz.y / (float)sz.x, 1);

	glRotatef(m_rotation, 0, 1, 0);

	glColor4f(1, 1, 1, m_alpha);

	m_pAnim->Draw();

	glColor4f(1, 1, 1, 1);
	glPopMatrix();
}

Rect Sprite::GetFootprintRect() const
{
	return GetFootprintRect(m_pos);
}

Rect Sprite::GetFootprintRect(const Point3& pos) const
{
	Size2 footprint = m_footprint;
	if (footprint.x == 0 && footprint.y == 0)
		footprint.x = footprint.y = m_pAnim->GetSize().x;

	footprint.x = int(footprint.x * m_scale * m_footprintScale.x);
	footprint.y = int(footprint.y * m_scale * m_footprintScale.y);

	return Rect(pos.x - footprint.x / 2, pos.y, footprint.x, footprint.y); 
}
