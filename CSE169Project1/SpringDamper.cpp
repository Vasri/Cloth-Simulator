#include "SpringDamper.h"

SpringDamper::SpringDamper() {
	P1 = NULL;
	P2 = NULL;
	RestLength = 1.1f;
	DampFactor = DAMP_CONSTANT;
	SpringConstant = SPRING_CONSTANT;
}

SpringDamper::SpringDamper(Particle* p1, Particle* p2) {
	P1 = p1;
	P2 = p2;
	vec3 vec = P2->getPosition() - P1->getPosition();
	RestLength = (float) glm::length(vec);
	DampFactor = DAMP_CONSTANT;
	SpringConstant = SPRING_CONSTANT;
}

void SpringDamper::React() {
	vec3 diff = P2->getPosition() - P1->getPosition();
	float dist = (float)glm::length(diff);
	vec3 corr = diff * (1 - RestLength / dist);
	corr = 0.5f * corr;
	P1->updatePosition(corr);
	P1->updatePosition(-1.0f * corr);
}

void SpringDamper::ComputeForce() {
	if (P1->isFixed() && P2->isFixed()) return;
	vec3 e_ = P2->getPosition() - P1->getPosition();
	float l = glm::length(e_);

	vec3 e = e_ / l;

	float v1 = glm::dot(e, P1->getVelocity());
	float v2 = glm::dot(e, P2->getVelocity());
	float fsd = -1.0f * SpringConstant * (RestLength - l) - DampFactor * (v1 - v2);

	vec3 f1 = fsd * e;
	vec3 f2 = -1.0f * f1;

	//printf("spring f1: %f,%f,%f\n", f1.x, f1.y, f1.z);

	P1->ApplyForce(f1);
	P2->ApplyForce(f2);
	//printf("p1 force: (%f,%f,%f)\n", P1->getForce().x, P1->getForce().y, P1->getForce().z);
	//printf("p2 force: (%f,%f,%f)\n", P2->getForce().x, P2->getForce().y, P2->getForce().z);
}
