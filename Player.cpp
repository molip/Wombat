#include "Player.h"
#include "Texture.h"
#include "Vector.h"
#include "Mesh.h"

#include <SFML/OpenGL.hpp>

Player::Player()
{
	SetState(State::Stand);
	//SetPos(Point3(300, 300, 0));
}

Player::~Player()
{
}

void Player::Update(float tDelta)
{
	__super::Update(tDelta);
}

void Player::SetState(State s)
{
}

void Player::DrawMesh() const
{
	::DrawPlayer();
}

