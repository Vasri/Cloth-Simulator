#pragma once

#include "Primitive.h"

class PrimitiveSphere : public Primitive {
public:
	PrimitiveSphere();
	PrimitiveSphere(vec3 c, float r);

	bool TestSegment(const Segment &s, Intersection &i);

	vec3 GetCenter();
	void SetCenter(vec3 c);

	float GetRadius();
	void SetRadius(float r);

private:
	vec3 Center;
	float Radius;
};