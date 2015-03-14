#pragma once

#include "Sprite.h"

namespace Jig
{
	class ObjMesh;
	class MeshAnimation;
}

class Player : public Sprite
{
public:
	enum class State { Stand, Walk };

	Player();
	~Player();

	virtual void Update(double tDelta) override;

	void SetState(State s);

private:
	virtual void DrawMesh() const override;

	State m_state;
	std::unique_ptr<Jig::ObjMesh> m_mesh;
	std::unique_ptr<Jig::MeshAnimation> m_anim;
	double m_val;
};

