#pragma once

#include <list>

class Matrix;
class Vec2;

class Mesh 
{
public:
	virtual ~Mesh() {}

	virtual void Draw() const = 0;

protected:
};

class Mesh1 : public Mesh
{
public:
	virtual void Draw() const override;
};

void DrawPlayer();
