#pragma once

#include "Particle.h"

static const float SPRING_CONSTANT = 100;
static const float DAMP_CONSTANT = 10.0f;

class SpringDamper {
public:
	SpringDamper();
	SpringDamper(Particle* p1, Particle* p2);

	void React();
	void ComputeForce();

	Particle* P1;
	Particle* P2;
private:
	float SpringConstant, DampFactor;
	float RestLength;
};