#pragma once

#include "Primitive.h"

class PrimitivePlane : public Primitive {
public:
	PrimitivePlane();
	PrimitivePlane(vec3 p);

	bool TestSegment(const Segment &s, Intersection &i);

	void SetPoint(vec3 &p);
	void SetNormal(vec3 &n);

	vec3 GetNormal();

	float GetDistance();

private:
	vec3 point;
	vec3 normal;
	float distance;
	float length = 2;
};