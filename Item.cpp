#include "Item.h"
#include "Texture.h"

#include <SFML/OpenGL.hpp>

Item::Item() : Sprite(1),
	m_tex(Texture::Get("../res/item1.png"))
{
	SetAnimation(m_tex, 4, 10); 
}

Item::~Item()
{
}

void Item::Update(float tDelta)
{
	__super::Update(tDelta);

	m_rotation += tDelta * 360;
}


