#include "Planet.h"
#include "Player.h"
#include "Sprite.h"
#include "Texture.h"
#include "Mesh.h"

#include <SFML/OpenGL.hpp>

#include <iostream>
#include <set>

namespace 
{
	Point3 GetRandomPos(int range)
	{
		return Point3(int(range * rand() / (double)RAND_MAX), int(range * rand() / (double)RAND_MAX), 0);
	}
}

Planet* Planet::s_pInstance;

Planet::Planet() : m_pPlayer(new Player), m_meshType(MeshType::Polar), m_bWireframe(false), m_bHitTest(true), m_rotation(0), m_rotationTarget(0), 
m_dirX(1, 0, 0), m_dirZ(0, 0, 1), m_vAngle(30), m_hAngle(0)
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

void Planet::Update(float tDelta)
{
	m_pPlayer->Update(tDelta);

	int x = 0, z = 0;

	x -= sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	x += sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    z -= sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    z += sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

	m_pPlayer->SetState(z > 0 ? Player::State::WalkUp : x || z ? Player::State::WalkDown : Player::State::Stand);

	float moveDelta = tDelta * 100;

	Point3f pos = m_pPlayer->GetPos();
	pos += x * moveDelta * m_dirX;
	pos += z * moveDelta * m_dirZ;
	Point3f newPos = pos;
	
	float rotationDelta = tDelta * 200;
	if (m_rotationTarget > m_rotation)
		m_rotation += std::min(rotationDelta, m_rotationTarget - m_rotation);
	else if (m_rotationTarget < m_rotation)
		m_rotation -= std::min(rotationDelta, m_rotation - m_rotationTarget);

	float vRotationDelta = tDelta * 25;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_vAngle += vRotationDelta;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_vAngle -= vRotationDelta;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_hAngle += vRotationDelta;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_hAngle -= vRotationDelta;

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
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	float amb[] = { 0.2f, 0.2f, 0.2f, 0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, -5, -100);

	glRotatef(m_vAngle, 1, 0, 0);
	glRotatef(m_hAngle + m_rotation, 0, 1, 0);

	glTranslatef(-m_pPlayer->GetPos().x, 0, -m_pPlayer->GetPos().z);

	float light0[] = { -50, 40, 30, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0);

	DrawPlanet();
	DrawSprites();
}

void Planet::OnKeyPressed(sf::Keyboard::Key key)
{
	auto RotateCCW = [](Vec3& vec) { vec = Vec3(vec.z, 0, -vec.x); };
	auto RotateCW = [](Vec3& vec) { vec = Vec3(-vec.z, 0, vec.x); };

	if (key == sf::Keyboard::Tab)
	{
		m_meshType = MeshType((int(m_meshType) + 1) % (int)MeshType::_count);
		CreateMesh();
	}
	else if (key == sf::Keyboard::W)
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
	
	m_pMesh->Draw();
}

void Planet::DrawSprites() const
{
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE);
	sf::Texture::bind(&m_texture);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
				
	for (auto& p : m_objs)
		p->Draw();

	m_pPlayer->Draw();

	glPopMatrix();
}