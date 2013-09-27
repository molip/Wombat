#pragma once

#include "Matrix.h"
#include "Vector.h"
#include "Types.h"

#include "SFML/Graphics.hpp"

#include <memory>

class Sprite;
class Player;
class Mesh;
class Item;

class Planet
{
public:
	Planet();
	~Planet();

	void Update(float tDelta);

	void Draw(sf::RenderWindow& win) const;

	void OnKeyPressed(sf::Keyboard::Key key);

	static Planet* Instance() { return s_pInstance; }

	int m_nSpriteRotateX;

private:
	void DrawPlanet() const;
	void DrawSprites() const;
	void CreateMesh();

	bool HitTest(const Sprite& s1, const Sprite& s2, const Point3& p2) const;
	bool HitTest(const Sprite& s1, const Sprite& s2) const;

	sf::Texture m_texture;
	bool m_bWireframe, m_bHitTest;

	enum class MeshType { Polar, _count };

	std::vector<std::unique_ptr<Sprite>> m_objs;
	std::vector<std::unique_ptr<Item>> m_items;
	std::unique_ptr<Player> m_pPlayer;
	std::unique_ptr<Mesh> m_pMesh;
	MeshType m_meshType;

	static Planet* s_pInstance;
};

