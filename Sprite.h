#pragma once

#include "Animation.h"
#include "Types.h"

#include <memory>

class Matrix;

class Sprite
{
public:
	Sprite();
	~Sprite();

	virtual void Update(float tDelta);

	void Draw() const;

	void SetPos(const Point3f& v) { m_pos = v; }
	const Point3f& GetPos() const { return m_pos; }
	
protected:
	virtual void DrawMesh() const = 0;

private:
	Point3f m_pos;
};

