#pragma once

#include "Jig/Types.h"

#include <memory>

class Matrix;

class Sprite
{
public:
	Sprite();
	~Sprite();

	virtual void Update(double tDelta);

	void Draw() const;

	void SetPos(const Jig::Vec3& v) { m_pos = v; }
	const Jig::Vec3& GetPos() const { return m_pos; }
	
	void SetRotation(double val) { m_rotation = val; }

protected:
	virtual void DrawMesh() const = 0;

private:
	Jig::Vec3 m_pos;
	double m_rotation;
};

