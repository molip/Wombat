#pragma once

#include "Jig/Types.h"

#include <memory>

class Matrix;

class Sprite
{
public:
	Sprite();
	~Sprite();

	virtual void Update(float tDelta);

	void Draw() const;

	void SetPos(const Jig::Point3f& v) { m_pos = v; }
	const Jig::Point3f& GetPos() const { return m_pos; }
	
	void SetRotation(float val) { m_rotation = val; }

protected:
	virtual void DrawMesh() const = 0;

private:
	Jig::Point3f m_pos;
	float m_rotation;
};

