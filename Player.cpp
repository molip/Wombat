#include "Player.h"
#include "Texture.h"
#include "Vector.h"
#include "ObjMesh.h"

Player::Player()
{
	SetState(State::Stand);
	m_mesh.reset(new ObjMesh("../res/player.obj"));
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
	m_mesh->Draw();
}

