#pragma once

#include "Particle.h"
#include "Primitive.h"

class PrimitiveTriangle : public Primitive {
public:
	PrimitiveTriangle();
	PrimitiveTriangle(Particle* p1, Particle* p2, Particle* p3, int ind1, int ind2, int ind3);
	void setVerts(Particle* p1, Particle* p2, Particle* p3);
	Particle* getVertex(int idx);

	void initNorm();
	
	vec3 getNormal();
	vec3 getVelocity();

	void ComputeForce(vec3 v_air);

	bool TestSegment(const Segment &s, Intersection &i);
	bool TestTriangle(PrimitiveTriangle &t, Intersection &i);

	Particle* P1;
	Particle* P2;
	Particle* P3;

	int i1, i2, i3;

private:
	vec3 Normal;
	vec3 Velocity;

	//Model Tri;
};