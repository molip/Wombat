#include "Player.h"
#include "Jig/ObjMesh.h"

Player::Player() : m_val(0)
{
	SetState(State::Stand);
	m_mesh.reset(new Jig::ObjMesh("../res/player.obj"));
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
	float angle = ::sin(m_val * 20) * 50;

	m_mesh->DrawObject("Body");

	glPushMatrix();
	glTranslatef(0, 0.6f, 0);
	glRotatef(angle, 1, 0, 0);
	glTranslatef(0, -0.6f, 0);
	m_mesh->DrawObject("Left_leg");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.6f, 0);
	glRotatef(-angle, 1, 0, 0);
	glTranslatef(0, -0.6f, 0);
	m_mesh->DrawObject("Right_leg");
	glPopMatrix();
}

