#pragma once

#include "SpringDamper.h"
#include "PrimitiveTriangle.h"
//#include "ParticleSystem.h"

static const int size = 10;
static const float ratio = 0.05f;
static const int largeSpring = 400;
static const int largeDamper = 12;

class Cloth {
public: 
	Cloth();
	void Draw(bool mode);
	void Update(vec3 v_air);
	void Reset();

	float xMin();
	float xMax();
	float yMin();
	float yMax();

	void drag(float x, float y, float z);

private: 
	Particle* particles[size][size];
	float deltaTime;
	std::vector<SpringDamper*> springs;
	std::vector<SpringDamper*> bendSprings;

	std::vector<PrimitiveTriangle> triangles;
};