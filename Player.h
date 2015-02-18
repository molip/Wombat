#pragma once

#include "Sprite.h"

class Player : public Sprite
{
public:
	enum class State { Stand, WalkUp, WalkDown };

	Player();
	~Player();

	virtual void Update(float tDelta) override;

	void SetState(State s);

private:
	virtual void DrawMesh() const override;

	State m_state;
};

