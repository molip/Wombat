#pragma once

#include "Jig/Vector.h"
#include "Jig/Types.h"

#include "SFML/Graphics.hpp"

#include <memory>

class Sprite;
class Player;
class Mesh;

class Planet
{
public:
	Planet();
	~Planet();

	void Update(double tDelta);

	void Draw(sf::RenderWindow& win) const;

	void OnKeyPressed(sf::Keyboard::Key key);

	static Planet* Instance() { return s_pInstance; }

private:
	void DrawPlanet() const;
	void DrawSprites() const;
	void CreateMesh();

	bool HitTest(const Sprite& s1, const Sprite& s2, const Jig::Point3& p2) const;
	bool HitTest(const Sprite& s1, const Sprite& s2) const;

	sf::Texture m_texture;
	bool m_bWireframe, m_bHitTest;

	enum class MeshType { Polar, _count };

	std::vector<std::unique_ptr<Sprite>> m_objs;
	std::unique_ptr<Player> m_pPlayer;
	std::unique_ptr<Mesh> m_pMesh;
	MeshType m_meshType;

	static Planet* s_pInstance;

	double m_rotation;
	int m_rotationTarget;
	Jig::Vec3 m_dirX, m_dirZ;
	double m_vAngle, m_hAngle, m_zoom;
};

