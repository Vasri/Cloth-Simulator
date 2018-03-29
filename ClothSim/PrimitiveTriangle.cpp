#include "PrimitiveTriangle.h"

PrimitiveTriangle::PrimitiveTriangle() {
	Velocity = vec3(0.0f);
}

PrimitiveTriangle::PrimitiveTriangle(Particle* p1, Particle* p2, Particle* p3, int ind1, int ind2, int ind3) {
	P1 = p1;
	P2 = p2;
	P3 = p3;
	i1 = ind1;
	i2 = ind2;
	i3 = ind3;
	Velocity = vec3(0.0f);
}

void PrimitiveTriangle::setVerts(Particle* p1, Particle* p2, Particle* p3) {
	P1 = p1;
	P2 = p2;
	P3 = p3;
}

Particle* PrimitiveTriangle::getVertex(int idx) {
	switch(idx) {
		case 0: 
			return P1;
		case 1:	
			return P2;
		case 2: 
			return P3;
		default:
			return P1;
	}
}

void PrimitiveTriangle::initNorm() {
	vec3 ra = (P2->getPosition() - P1->getPosition());
	vec3 rb = (P3->getPosition() - P1->getPosition());

	Normal = glm::normalize(glm::cross(ra, rb));
}

vec3 PrimitiveTriangle::getNormal() {
	return Normal;
}

vec3 PrimitiveTriangle::getVelocity() {
	return Velocity;
}

void PrimitiveTriangle::ComputeForce(vec3 v_air) {
	Velocity = (P1->getVelocity() + P2->getVelocity() + P3->getVelocity()) / 3.0f;	// velocity of surface

	vec3 v = Velocity - v_air;	// relative velocity

	float rho = 1.225f;	// density of medium (air)
	float c_drag = 1.28f;	// coeff of drag (flat plate)

	vec3 ra = P2->getPosition() - P1->getPosition();
	vec3 rb = P3->getPosition() - P1->getPosition();
	vec3 raXrb = glm::cross(ra, rb);	// normalize to get Normal
	Normal = glm::normalize(raXrb);

	// f = -0.5 * rho * |v|^2 * c * a * n
	// n = cross / |cross|
	// a = 0.5 * |cross| * dot(v, n) / |v|

	// we can take algebraic shortcuts and combine v2an = v.length() * 0.5f * dot (v, n) * cross

	vec3 v2an = glm::length(v) * 0.5f * glm::dot(v, Normal) * raXrb;
	vec3 aero = -0.5f * rho * c_drag * v2an;

	//printf("aero: %f, %f, %f\n", aero.x, aero.y, aero.z);

	P1->ApplyForce(aero);
	P2->ApplyForce(aero);
	P3->ApplyForce(aero);
}

bool PrimitiveTriangle::TestSegment(const Segment &s, Intersection &i) {
	vec3 v0 = P1->getPosition();
	vec3 v1 = P2->getPosition();
	vec3 v2 = P3->getPosition();

	float da = glm::dot((s.A - v0), Normal);
	float db = glm::dot((s.B - v0), Normal);

	if (da > 0 && db > 0) {
		return false;
	}
	else if (da < 0 && db < 0) {
		return false;
	}

	if ((da - db == 0)) {
		return false;
	}

	vec3 x = ((da * s.A) - (db * s.B)) / (da - db);

	vec3 x_ = x - v0;
	vec3 e1 = v1 - v0;
	vec3 e2 = v2 - v0;

	float denominator = glm::dot(e1, e2);

	if (denominator == 0) {
		return false;
	}

	float alpha = glm::dot(x_, e2) / denominator;
	float beta = glm::dot(x_, e1) / denominator;

	if (alpha < 0 || beta < 0 || (alpha + beta) > 1) {
		return false;
	}

	vec3 temp = s.B - v0;

	float d = glm::dot(temp, Normal);

	if (d < 0) {
		d = -1 * d;
		x = x * (1 - d / glm::length(x));
	}

	i.Position = x;
	i.Normal = Normal;

	return true;
}

bool PrimitiveTriangle::TestTriangle(PrimitiveTriangle &t, Intersection &i) {
	vec3 v0 = P1->getPosition();
	vec3 v1 = P2->getPosition();
	vec3 v2 = P3->getPosition();

	vec3 n2 = v1 - v0;
	vec3 cross = v2 - v0;

	n2 = glm::cross(n2, cross);

	float d2 = glm::dot(-n2, v0);

	float di[3];
	for (int i = 0; i < 3; i++) {
		di[i] = glm::dot(n2, t.getVertex(i)->getPosition()) + d2;
	}

	if (di[0] > 0 && di[1] > 0 && di[2] > 0) {
		return false;
	}
	else if (di[0] < 0 && di[1] < 0 && di[2] < 0) {
		return false;
	}

	return true;
}