#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {
	ParticleSystem(14, 10);
}

ParticleSystem::ParticleSystem(int width, int height) {
	/*
		MUST:
			-initialize cloth
			 - set particles
			 - set springdampers
			-build triangles
	*/

	NumParticles_Width = width;
	NumParticles_Height = height;

	ground = PrimitivePlane(vec3(0.0f, -2.0f * NumParticles_Height, 0.0f));
	
	//MakeCloth(NumParticles_Width, NumParticles_Height, vec3(0.0f, 20.0f, 0.0f));
	MakeCloth(NumParticles_Width, NumParticles_Height, vec3(0.0f, 0.0f, 0.0f));

	//MakeBox(2, 2, 5, vec3(0.0f, 15.0f, -7.0f));

	// set up ropes
	/*Springs.push_back(new SpringDamper(GetParticle(0, 0), BoxParticles[3]));
	Springs.push_back(new SpringDamper(GetParticle(width - 1, 0), BoxParticles[2]));
	Springs.push_back(new SpringDamper(GetParticle(0, height - 1), BoxParticles[6]));
	Springs.push_back(new SpringDamper(GetParticle(width - 1, height - 1), BoxParticles[7]));
	*/

	// fix top of cloth
	for (int i = 0; i < NumParticles_Width; i++) {
		ClothParticles[i]->setFixed(true);
	}
}

void ParticleSystem::MakeCloth(int width, int height, vec3 middle) {
	ClothParticles.resize(NumParticles_Width*NumParticles_Height);

	// create particles from (0,0,0) to (width,0,height)
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			vec3 pos = vec3((float)i, 0.0f, -1.0f * float(j)) - vec3(width/2, 0, 0) + middle;
			ClothParticles[j*width + i] = new Particle(pos, 1.0f, false);
		}
	}

	// set PrimitiveTriangles
	for (int i = 0; i < width - 1; i++) {
		for (int j = 0; j < height - 1; j++) {
			Triangles.push_back(new PrimitiveTriangle(GetParticle(i + 1, j), GetParticle(i, j), GetParticle(i, j + 1),
				calcIdx(i + 1, j), calcIdx(i, j), calcIdx(i, j + 1)));
			Triangles.push_back(new PrimitiveTriangle(GetParticle(i + 1, j + 1), GetParticle(i + 1, j), GetParticle(i, j + 1),
				calcIdx(i + 1, j + 1), calcIdx(i + 1, j), calcIdx(i, j + 1)));
		}
	}

	// set SpringDampers
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			// structure springs
			if (i < (width - 1)) {
				Springs.push_back(new SpringDamper(GetParticle(i, j), GetParticle(i + 1, j)));
				springIdx.push_back(calcIdx(i, j));
				springIdx.push_back(calcIdx(i + 1, j));
			}
			if (j < (height - 1)) {
				Springs.push_back(new SpringDamper(GetParticle(i, j), GetParticle(i, j + 1)));
				springIdx.push_back(calcIdx(i, j));
				springIdx.push_back(calcIdx(i, j + 1));
			}
			// shear springs
			if (i < (width - 1) && (j < height - 1)) {
				Springs.push_back(new SpringDamper(GetParticle(i, j), GetParticle(i + 1, j + 1)));
				Springs.push_back(new SpringDamper(GetParticle(i + 1, j), GetParticle(i, j + 1)));
				springIdx.push_back(calcIdx(i, j));
				springIdx.push_back(calcIdx(i + 1, j + 1));
				springIdx.push_back(calcIdx(i + 1, j));
				springIdx.push_back(calcIdx(i, j + 1));
			}
		}
	}

	// set bending springs
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (i < width - 2) {
				Springs.push_back(new SpringDamper(GetParticle(i, j), GetParticle(i + 2, j)));
			}
			if (j < height - 2) {
				Springs.push_back(new SpringDamper(GetParticle(i, j), GetParticle(i, j + 2)));
			}
			if (i < width - 2 && j < height - 2) {
				Springs.push_back(new SpringDamper(GetParticle(i, j), GetParticle(i + 2, j + 2)));
				Springs.push_back(new SpringDamper(GetParticle(i + 2, j), GetParticle(i, j + 2)));
			}
		}
	}

	cloth.MakeCloth(ClothParticles, Triangles);
}

void ParticleSystem::MakeBox(float length, float width, float depth, vec3 center) {
	boxMin = vec3(center.x - (length / 2), center.y - (length / 2), center.z - (length / 2));
	boxMax = vec3(center.x + (length / 2), center.y + (length / 2), center.z + (length / 2));
	BoxParticles.reserve(8);

	// front
	BoxParticles.push_back(new Particle(vec3(boxMin.x, boxMin.y, boxMax.z), 5.0f, false));
	BoxParticles.push_back(new Particle(vec3(boxMax.x, boxMin.y, boxMax.z), 5.0f, false));
	BoxParticles.push_back(new Particle(vec3(boxMax.x, boxMax.y, boxMax.z), 5.0f, false));
	BoxParticles.push_back(new Particle(vec3(boxMin.x, boxMax.y, boxMax.z), 5.0f, false));

	// back
	BoxParticles.push_back(new Particle(vec3(boxMax.x, boxMin.y, boxMin.z), 5.0f, false));
	BoxParticles.push_back(new Particle(vec3(boxMin.x, boxMin.y, boxMin.z), 5.0f, false));
	BoxParticles.push_back(new Particle(vec3(boxMin.x, boxMax.y, boxMin.z), 5.0f, false));
	BoxParticles.push_back(new Particle(vec3(boxMax.x, boxMax.y, boxMin.z), 5.0f, false));

	// set springs
	Springs.push_back(new SpringDamper(BoxParticles[0], BoxParticles[7]));
	Springs.push_back(new SpringDamper(BoxParticles[1], BoxParticles[6]));
	Springs.push_back(new SpringDamper(BoxParticles[2], BoxParticles[5]));
	Springs.push_back(new SpringDamper(BoxParticles[3], BoxParticles[4]));

	box.MakeBox(boxMin, boxMax);
}

void ParticleSystem::MakeRope(vec3 start, vec3 end) {
	int length = (int) glm::length(start - end);
	vec3 dv = (end - start) / (float)length;

	// set particles
	for (int i = 0; i < length; i++) {
		vec3 pos = (float) i * dv;
		ClothParticles.push_back(new Particle(pos, 1.0f, false));
	}

	// set springs
	int pSize = ClothParticles.size();
	for (int i = 1; i < length; i++) {
		Springs.push_back(new SpringDamper(ClothParticles[pSize - i], ClothParticles[pSize - i - 1]));
	}
}

void ParticleSystem::Move(vec3 dir) {
	for (int i = 0; i < NumParticles_Width; i++) {
		ClothParticles[i]->moveParticle(dir);
	}
}

void ParticleSystem::Update(float deltaTime, vec3 v_air) {
	// Compute forces

	// gravity
	vec3 gravity(0.0f, -9.8f, 0.0f);
	for (int i = 0; i < NumParticles_Width*NumParticles_Height; i++) {
		vec3 force = gravity * ClothParticles[i]->getMass();	// f = m * g
		ClothParticles[i]->ApplyForce(force);
	}
	for (int i = 0; i < BoxParticles.size(); i++) {
		vec3 force = gravity * ClothParticles[i]->getMass();
		BoxParticles[i]->ApplyForce(force);
	}

	// springs
	for (uint i = 0; i < Springs.size(); i++) {
		Springs[i]->ComputeForce();
	}

	// wind
	for (uint i = 0; i < Triangles.size(); i++) {
		Triangles[i]->ComputeForce(v_air);
	}

	// Integrate
	for (int i = 0; i < NumParticles_Width*NumParticles_Height; i++) {
		ClothParticles[i]->Update(deltaTime);
	}
	for (int i = 0; i < BoxParticles.size(); i++) {
		BoxParticles[i]->Update(deltaTime);
		boxMin = BoxParticles[5]->getPosition();
		boxMax = BoxParticles[2]->getPosition();
	}

	// check collisions
	for (int i = 0; i < NumParticles_Width*NumParticles_Height; i++) {
		Segment s;
		Intersection inter;
		s.A = ClothParticles[i]->getLastPosition();
		s.B = ClothParticles[i]->getPosition();

		//check for collision with ground
		if (ground.TestSegment(s, inter)) {
			ClothParticles[i]->setPosition(inter.Position.x, inter.Position.y, inter.Position.z);
			vec3 v = ClothParticles[i]->getVelocity();
			v.x = 0.7f * v.x;
			v.y = -0.0001f * v.y;
			v.z = 0.7f * v.z;
			ClothParticles[i]->setVelocity(v.x, v.y, v.z);
		}

		ClothParticles[i]->updateLast(ClothParticles[i]->getPosition());
	}

	cloth.MakeCloth(ClothParticles, Triangles);
	box.MakeBox(boxMin, boxMax);
}

void ParticleSystem::Draw(const mat4 &viewProjMat, uint shader) {
	// ideally should iterate/draw primitives (triangles, etc)

	// zero out normals each frame
	for (int i = 0; i < NumParticles_Width * NumParticles_Height; i++) {
		ClothParticles[i]->resetNormal();
	}

	// calculate normals for drawing
	for (uint i = 0; i < Triangles.size(); i++) {
		Triangles[i]->P1->addNormal(Triangles[i]->getNormal());
		Triangles[i]->P2->addNormal(Triangles[i]->getNormal());
		Triangles[i]->P3->addNormal(Triangles[i]->getNormal());
	}

	// draw triangles
	cloth.Draw(mat4(1.0f), viewProjMat, shader);
	box.Draw(mat4(1.0f), viewProjMat, shader);
}