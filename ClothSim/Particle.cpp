#include "Particle.h"

Particle::Particle() {
	Mass = MASS_CONST;
	Position = vec3(0.0f);
	LastPosition = vec3(0.0f);
	Velocity = vec3(0.0f);
	Force = vec3(0.0f);
	fixed = false;
	Normal = vec3(0.0f);
}

Particle::Particle(vec3 position, float mass, bool f) {
	Mass = mass;
	LastPosition = position;
	Position = position;
	Velocity = vec3(0.0f);
	Force = vec3(0.0f);
	fixed = f;
	Normal = vec3(0.0f);
}

void Particle::addNormal(vec3 norm) {
	Normal = Normal + norm;
}

vec3 Particle::getNormal() {
	return Normal;
}

void Particle::resetNormal() {
	Normal = vec3(0.0f);
}

// During simulation, always want to apply force before updating
void Particle::ApplyForce(vec3 &f) {
	Force = Force + f;
}

vec3 Particle::getForce() {
	return Force;
}

void Particle::Update(float deltaTime) {
	if (fixed) {
		Force = vec3(0.0f);
		return;
	}
	// Compute acceleration
	vec3 Accel = Force / Mass;

	// Compute new position and velocity
	Velocity = Velocity + Accel * deltaTime;
	LastPosition = Position;	// lastposition stores old position when new position updates
	Position = Position + Velocity * deltaTime;

	// Zero out Force Vector
	Force = vec3(0.0f);
}

void Particle::Draw() {
	glPointSize(3);
	glBegin(GL_POINTS);
	glVertex3f(Position.x, Position.y, Position.z);
	glEnd();
}

void Particle::Reset() {
	Force = vec3(0.0f);
	Velocity = vec3(0.0f);
}

void Particle::moveParticle(vec3 v) {
	Position = Position + v;
}

void Particle::updatePosition(vec3 v) {
	if (!fixed) {
		Position = Position + v;
	}
}

void Particle::updateLast(vec3 last) {
	LastPosition = last;
}

vec3 Particle::getLastPosition() {
	return LastPosition;
}

void Particle::setVelocity(float x, float y, float z) {
	Velocity = vec3(x, y, z);
}

vec3 Particle::getVelocity() {
	return Velocity;
}

void Particle::setPosition(float x, float y, float z) {
	Position = vec3(x, y, z);
}

vec3 Particle::getPosition() {
	return Position;
}

void Particle::setFixed(bool f) {
	fixed = f;
}

float Particle::getMass() {
	return Mass;
}
