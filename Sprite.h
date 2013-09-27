#pragma once

#include "Animation.h"
#include "Types.h"

//#include "SFML/Graphics.hpp"

#include <memory>
class Matrix;

class Sprite
{
public:
	Sprite(float scale, float alpha = 1);
	~Sprite();

	virtual void Update(float tDelta);

	void Draw() const;

	void SetPos(const Point3& v) { m_pos = v; }
	const Point3& GetPos() const { return m_pos; }
	
	void SetFootprintSize(const Size2& size) { m_footprint = size; }
	void SetFootprintScale(const Size2f& scale) { m_footprintScale = scale; }
	void SetScale(float scale) { m_scale = scale; }

	Rect GetFootprintRect() const;
	Rect GetFootprintRect(const Point3& pos) const;

	bool operator <(const Sprite& rhs) const;

	void SetAnimation(std::unique_ptr<Animation> pAnim) { m_pAnim = std::move(pAnim); }
	void SetAnimation(const sf::Texture& texture, int nFrames = 1, float fps = 0);
	void SetAnimation(const char* sPath, int nFrames = 1, float fps = 0);
	const Animation* GetAnimation() const { return m_pAnim.get(); }

protected:
	float m_rotation;

private:
	std::unique_ptr<Animation> m_pAnim;
	Point3 m_pos;
	Size2 m_footprint;
	Size2f m_footprintScale;
	float m_scale, m_alpha;
};

