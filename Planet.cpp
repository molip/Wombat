#include "Planet.h"
#include "Player.h"
#include "Sprite.h"
#include "Mesh.h"

#include "Jig/Material.h"
#include "Jig/Texture.h"

#include <SFML/OpenGL.hpp>

#include <iostream>
#include <set>

using namespace Jig;

namespace 
{
	Point3 GetRandomPos(int range)
	{
		return Point3(int(range * rand() / (double)RAND_MAX), int(range * rand() / (double)RAND_MAX), 0);
	}
}

Planet* Planet::s_pInstance;

Planet::Planet() : m_pPlayer(new Player), m_meshType(MeshType::Polar), m_bWireframe(false), m_bHitTest(true), m_rotation(0), m_rotationTarget(0), 
m_dirX(1, 0, 0), m_dirZ(0, 0, 1), m_vAngle(30), m_hAngle(0), m_zoom(10)
{
    s_pInstance = this;
	
	if (!m_texture.loadFromFile("../res/tiles.png"))
		throw;
	m_texture.setRepeated(true);
	m_texture.setSmooth(true);

	CreateMesh();
}

Planet::~Planet()
{
	s_pInstance = nullptr;
}

bool Planet::HitTest(const Sprite& s1, const Sprite& s2, const Point3& p2) const
{
	return false;
} 

bool Planet::HitTest(const Sprite& s1, const Sprite& s2) const
{
	return false;
}

void Planet::Update(double tDelta)
{
	int x = 0, z = 0;

	x -= sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	x += sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    z -= sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    z += sf::Keyboard::isKeyPressed(sf::Keyboard::S);

	m_pPlayer->SetState(z == 0 && x == 0 ? Player::State::Stand : Player::State::Walk);
	m_pPlayer->Update(tDelta);

	double moveDelta = tDelta * 20;

	Vec3 pos = m_pPlayer->GetPos();
	pos += x * moveDelta * m_dirX;
	pos += z * moveDelta * m_dirZ;
	Vec3 newPos = pos;
	
	double rotationDelta = tDelta * 200;
	if (m_rotationTarget > m_rotation)
		m_rotation += std::min(rotationDelta, m_rotationTarget - m_rotation);
	else if (m_rotationTarget < m_rotation)
		m_rotation -= std::min(rotationDelta, m_rotation - m_rotationTarget);

	double vRotationDelta = tDelta * 25;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_vAngle += vRotationDelta;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_vAngle -= vRotationDelta;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		m_hAngle += vRotationDelta;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m_hAngle -= vRotationDelta;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown))
		m_zoom += vRotationDelta;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp))
		m_zoom -= vRotationDelta;

	if (x < 0)
		m_pPlayer->SetRotation(90 - (double)m_rotationTarget);
	else if (x > 0)
		m_pPlayer->SetRotation(-90 - (double)m_rotationTarget);
	else if (z < 0)
		m_pPlayer->SetRotation(0 - (double)m_rotationTarget);
	else if (z > 0)
		m_pPlayer->SetRotation(180 - (double)m_rotationTarget);

	for (auto& p : m_objs)
	{
		//Rect overlap;
		//Rect r1 = p->GetFootprintRect();
		//Rect r2 = m_pPlayer->GetFootprintRect(newPos);

		//if (r1.intersects(r2, overlap))
		//{
		//	if (x < 0)
		//		newPos.x += overlap.width + 1; 
		//	else if (x > 0)
		//		newPos.x -= overlap.width + 1; 
		//	
		//	if (y < 0)
		//		newPos.y += overlap.height + 1; 
		//	else if (y > 0)
		//		newPos.y -= overlap.height + 1; 
		//}
	}
	m_pPlayer->SetPos(newPos);
}

void Planet::Draw(sf::RenderWindow& win) const
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	float amb[] = { 0.2f, 0.2f, 0.2f, 0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslated(0, -0.8, 0);
	glTranslated(0, 0, -m_zoom);

	glRotated(m_vAngle, 1, 0, 0);
	glRotated(m_hAngle + m_rotation, 0, 1, 0);

	glTranslated(-m_pPlayer->GetPos().x, 0, -m_pPlayer->GetPos().z);

	float light0[] = { -50, 40, 30, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0);

	if (m_bWireframe)
		glPolygonMode(GL_FRONT, GL_LINE);

	DrawPlanet();
	DrawSprites();

	glPolygonMode(GL_FRONT, GL_FILL);
}

void Planet::OnKeyPressed(sf::Keyboard::Key key)
{
	auto RotateCCW = [](Vec3& vec) { vec = Vec3(vec.z, 0, -vec.x); };
	auto RotateCW = [](Vec3& vec) { vec = Vec3(-vec.z, 0, vec.x); };

	if (key == sf::Keyboard::Tab)
		m_bWireframe = !m_bWireframe;
	else if (key == sf::Keyboard::H)
		m_bHitTest = !m_bHitTest;
	else if (key == sf::Keyboard::E)
		m_rotationTarget -= 90, RotateCCW(m_dirX), RotateCCW(m_dirZ);
	else if (key == sf::Keyboard::Q)
		m_rotationTarget += 90, RotateCW(m_dirX), RotateCW(m_dirZ);
}

void Planet::CreateMesh()
{
	switch (m_meshType)
	{
	case MeshType::Polar: m_pMesh.reset(new Mesh1); break;
	}
}

void Planet::DrawPlanet() const
{
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE);
	sf::Texture::bind(&m_texture);

	Material().Apply();

	m_pMesh->Draw();
}

void Planet::DrawSprites() const
{
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
				
	for (auto& p : m_objs)
		p->Draw();

	m_pPlayer->Draw();

	glPopMatrix();
}