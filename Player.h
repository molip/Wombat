#pragma once

#include "Sprite.h"

class ObjMesh;
class Player : public Sprite
{
public:
	enum class State { Stand, Walk };

	Player();
	~Player();

	virtual void Update(float tDelta) override;

	void SetState(State s);

private:
	virtual void DrawMesh() const override;

	State m_state;
	std::unique_ptr<ObjMesh> m_mesh;
	float m_val;
};

