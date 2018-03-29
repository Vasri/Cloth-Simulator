#include "PrimitivePlane.h"

PrimitivePlane::PrimitivePlane() {
	point = vec3(0, 0, 0);
	normal = vec3(0, 1, 0);
}

PrimitivePlane::PrimitivePlane(vec3 p) {
	point = p;
	normal = vec3(0, 1, 0);
	distance = glm::dot(normal, point);
}

bool PrimitivePlane::TestSegment(const Segment &s, Intersection &i) {
	vec3 v(s.B - point);

	float l = glm::dot(v, normal);

	if (l < 0) {
		vec3 dir = s.B - s.A;
		dir = glm::normalize(dir);
		float dn = glm::dot(dir, normal);
		float t = (distance - glm::dot(s.A, normal)) / dn;
		vec3 x = s.A + (t*dir);

		i.Position = x;
		i.Normal = v;
		return true;
	}
	else return false;
}

void PrimitivePlane::SetPoint(vec3 &p) {
	point = p;
	distance = glm::dot(normal, point);
}

void PrimitivePlane::SetNormal(vec3 &n) {
	normal = n;
}

vec3 PrimitivePlane::GetNormal() {
	return normal;
}

float PrimitivePlane::GetDistance() {
	return distance;
}