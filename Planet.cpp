#include "Planet.h"
#include "Player.h"
#include "Sprite.h"
#include "Texture.h"
#include "Mesh.h"
#include "Item.h"

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

Planet::Planet() : m_pPlayer(new Player), m_meshType(MeshType::Polar), m_bWireframe(false), m_bHitTest(true),
	m_nSpriteRotateX(-1)
{
    s_pInstance = this;
	
	if (!m_texture.loadFromFile("../res/craters.png"))
		throw;
	m_texture.setRepeated(true);
	m_texture.setSmooth(true);

	auto& tex = Texture::Get("../res/tree1.png");
	for (int i = 0; i < 50; ++i)
	{
		float width = (float(rand() % 3) + 1) / 10;
		m_objs.push_back(std::unique_ptr<Sprite>(new Sprite(width, 1)));
		m_objs.back()->SetFootprintScale(Size2f(0.4f, 0.4f));
		m_objs.back()->SetPos(GetRandomPos(600));
		m_objs.back()->SetAnimation(tex);
	}

	//m_objs.push_back(std::unique_ptr<Sprite>(new Sprite(1, 1)));
	//m_objs.back()->SetPos(Vec3(0, 1, 0).Normalised());
	//m_objs.back()->SetAnimation("../res/cookery.png");

	for (int i = 0; i < 100; ++i)
	{
		m_items.push_back(std::unique_ptr<Item>(new Item));
		m_items.back()->SetPos(GetRandomPos(600));
	}

	//m_items.push_back(std::unique_ptr<Item>(new Item));
	//m_items.back()->SetPos(Vec3(0, 1, 0));

	CreateMesh();
}

Planet::~Planet()
{
	s_pInstance = nullptr;
}

bool Planet::HitTest(const Sprite& s1, const Sprite& s2, const Point3& p2) const
{
	Rect r1 = s1.GetFootprintRect();
	Rect r2 = s2.GetFootprintRect(p2);
	return r1.intersects(r2);
} 

bool Planet::HitTest(const Sprite& s1, const Sprite& s2) const
{
	return s1.GetFootprintRect().intersects(s2.GetFootprintRect());
}

void Planet::Update(float tDelta)
{
	m_pPlayer->Update(tDelta);

	for (auto& p : m_items)
		p->Update(tDelta);

	int x = 0, y = 0;

	x -= sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	x += sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    y += sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    y -= sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

	m_pPlayer->SetState(y > 0 ? Player::State::WalkUp : x || y ? Player::State::WalkDown : Player::State::Stand);

	x *= 4;
	y *= 4;

	Point3 oldPos = m_pPlayer->GetPos();
	Point3 newPos = oldPos + Point3(x, y, 0);
	
	for (auto& p : m_objs)
	{
		Rect overlap;
		Rect r1 = p->GetFootprintRect();
		Rect r2 = m_pPlayer->GetFootprintRect(newPos);

		if (r1.intersects(r2, overlap))
		{
			if (x < 0)
				newPos.x += overlap.width + 1; 
			else if (x > 0)
				newPos.x -= overlap.width + 1; 
			
			if (y < 0)
				newPos.y += overlap.height + 1; 
			else if (y > 0)
				newPos.y -= overlap.height + 1; 
		}
	}
	m_pPlayer->SetPos(newPos);

/*
	auto doMove = [&] (const Point3& newPos) -> bool
	{
		if (m_bHitTest)
			for (auto& p : m_objs)
				if (HitTest(*p, *m_pPlayer, newPos))
					return false;

		return true;
	};

	Point3 posPlayer = m_pPlayer->GetPos();

	x *= 8;
	y *= 8;

	if (x && doMove(posPlayer + Point3(x, 0, 0)))
		posPlayer.x += x;

	if (y && doMove(posPlayer + Point3(0, y, 0)))
		posPlayer.y += y;

	m_pPlayer->SetPos(posPlayer);
*/

	for (auto it = m_items.begin(); it != m_items.end(); )
		if (HitTest(**it, *m_pPlayer))
			it = m_items.erase(it);
		else
			++it;
}

void Planet::Draw(sf::RenderWindow& win) const
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//	glTranslatef(0, 0, -1);

	DrawPlanet();
	DrawSprites();
}

void Planet::OnKeyPressed(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Tab)
	{
		m_meshType = MeshType((int(m_meshType) + 1) % (int)MeshType::_count);
		CreateMesh();
	}
	else if (key == sf::Keyboard::W)
		m_bWireframe = !m_bWireframe;
	else if (key == sf::Keyboard::H)
		m_bHitTest = !m_bHitTest;
	else if (key == sf::Keyboard::R)
	{
		if (++m_nSpriteRotateX == 1)
			m_nSpriteRotateX = -1;
		std::cout << "Planet::m_nSpriteRotateX: " << m_nSpriteRotateX << std::endl;
	}
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

	if (m_bWireframe)
		glPolygonMode(GL_FRONT, GL_LINE);
	
	m_pMesh->Draw();
	
	glPolygonMode(GL_FRONT, GL_FILL);

}

void Planet::DrawSprites() const
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
				
	struct Comp 
	{
		bool operator() (const Sprite* pL, const Sprite* pR) const { return *pL < *pR; };
	};
	std::multiset<const Sprite*, Comp> set;

	set.insert(m_pPlayer.get());

	for (auto& p : m_objs)
	{
		set.insert(p.get());
	}

	for (auto& p : m_items)
	{
		set.insert(p.get());
	}

	for (auto p : set)
		p->Draw();

	glPopMatrix();
}