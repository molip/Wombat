#include "Player.h"
#include "Jig/ObjMesh.h"
#include "Jig/MeshAnimation.h"

Player::Player() : m_val(0)
{
	SetState(State::Stand);
	m_mesh.reset(new Jig::ObjMesh("../res/player.obj"));
	m_anim.reset(new Jig::MeshAnimation(*m_mesh));
}

Player::~Player()
{
}

void Player::Update(float tDelta)
{
	__super::Update(tDelta);
	if (m_state == State::Walk)
		m_val += tDelta;
}

void Player::SetState(State s)
{
	if (s != m_state)
		m_val = 0;
	m_state = s;
}

void Player::DrawMesh() const
{
	if (m_state == State::Walk)
		m_anim->Draw(m_val * 2);
	else
		m_mesh->Draw();
}
