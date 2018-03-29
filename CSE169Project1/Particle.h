#pragma once

#include "Core.h"

static const float MASS_CONST = 1.0f;

class Particle {
public: 
	Particle();
	Particle(vec3 position, float mass, bool fixed);

	void Update(float deltaTime);
	void Draw();
	void Reset();

	void ApplyForce(vec3 &f);
	vec3 getForce();

	vec3 getLastPosition();
	void updateLast(vec3 last);

	vec3 getPosition();
	void setPosition(float x, float y, float z);

	vec3 getVelocity();
	void setVelocity(float x, float y, float z);

	void setFixed(bool f);
	bool isFixed() { return fixed; }

	void addNormal(vec3 norm);
	vec3 getNormal();
	void resetNormal();

	void updatePosition(vec3 v);
	void moveParticle(vec3 v);

	float getMass();

private:
	float Mass;
	vec3 Position;
	vec3 Velocity;
	vec3 Force;
	vec3 Normal;

	vec3 LastPosition;

	bool fixed;
};
