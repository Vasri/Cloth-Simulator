#include "PrimitiveSphere.h"

PrimitiveSphere::PrimitiveSphere() {
	Center = vec3(0.0f);
	Radius = 0.2f;
}

PrimitiveSphere::PrimitiveSphere(vec3 c, float r) {
	Center = c;
	Radius = r;
}

bool PrimitiveSphere::TestSegment(const Segment &s, Intersection &i) {
	vec3 v = s.B - Center;
	float l = (float) glm::length(v) * glm::length(v);

	if (l < Radius*Radius) {
		vec3 x = glm::normalize(v) * Radius + Center;
		i.Position = x;
		i.Normal = v;
		return true;
	}
	else return false;
}

vec3 PrimitiveSphere::GetCenter() {
	return Center;
}

void PrimitiveSphere::SetCenter(vec3 c) {
	Center = c;
}

float PrimitiveSphere::GetRadius() {
	return Radius;
}

void PrimitiveSphere::SetRadius(float r) {
	Radius = r;
}