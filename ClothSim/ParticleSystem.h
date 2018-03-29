#pragma once

#include "PrimitiveTriangle.h"
#include "PrimitivePlane.h"
#include "PrimitiveSphere.h"
#include "SpringDamper.h"
#include "Model.h"

static int NumParticles_Width;
static int NumParticles_Height;
static vec3 boxMin;
static vec3 boxMax;

class ParticleSystem {
public:
	ParticleSystem();
	ParticleSystem(int width, int height);

	void Update(float deltaTime, vec3 v_air);
	void Draw(const mat4 &viewProjMat, uint shader);

	// Creation Rules
	void MakeBox(float length, float width, float depth, vec3 middle);
	void MakeRope(vec3 start, vec3 end);
	void MakeCloth(int length, int height, vec3 center);

	void Move(vec3 dir);

private:
	std::vector <Particle*> ClothParticles;
	std::vector <Particle*> RopeParticles;
	std::vector <Particle*> BoxParticles;
	std::vector<PrimitiveTriangle*> Triangles;
	std::vector<SpringDamper*> Springs;
	std::vector<int> springIdx;

	int clothSize;

	// helper to get index of each particle
	uint calcIdx(int x, int y) { return y * NumParticles_Width + x; };
	// helper to get particle at a coordinate
	Particle* GetParticle(int x, int y) { return ClothParticles[y * NumParticles_Width + x]; };

	Model cloth;
	Model box;
	PrimitivePlane ground;
};