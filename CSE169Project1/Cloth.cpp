#include "Cloth.h"
#include <math.h>

Cloth::Cloth() {
	deltaTime = 0.0f;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			Particle* p = new Particle();
			p->setPosition(i * ratio - (ratio * size / 2.0f), 0.2f, j * ratio - (ratio * size / 2.0f));
			p->updateLast(p->getPosition());
			particles[i][j] = p;
		}
	}

	for (int i = 0; i < size-1; i++) {
		for (int j = 0; j < size-1; j++) {
			SpringDamper* s1 = new SpringDamper(particles[i][j], particles[i+1][j], ratio);
			SpringDamper* s2 = new SpringDamper(particles[i][j], particles[i][j+1], ratio);
			SpringDamper* s3 = new SpringDamper(particles[i][j], particles[i+1][j+1], ratio);

			springs.push_back(s1);
			springs.push_back(s2);
			springs.push_back(s3);
		}
	}

	for (int i = 1; i < size; i++) {
		for (int j = 0; j < size-1; j++) {
			SpringDamper* s = new SpringDamper(particles[i][j], particles[i-1][j+1], (float)sqrt(2) * ratio);
			springs.push_back(s);
		}
	}

	for (int j = 0; j < size-1; j++) {
		SpringDamper* s1 = new SpringDamper(particles[size-1][j], particles[size-1][j+1], ratio, largeDamper, largeSpring);
		SpringDamper* s2 = new SpringDamper(particles[j][size-1], particles[j+1][size-1], ratio, largeDamper, largeSpring);
		springs.push_back(s1);
		springs.push_back(s2);
	}

	for (int i = 0; i < size-1; i++) {
		for (int j = 0; j < size-1; j++) {
			PrimitiveTriangle t = PrimitiveTriangle();
			t.setVerts(particles[i][j], particles[i][j+1], particles[i+1][j+1]);
			triangles.push_back(t);
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			PrimitiveTriangle t = PrimitiveTriangle();
			t.setVerts(particles[i][j], particles[i][j-1], particles[i-1][j-1]);
			triangles.push_back(t);
		}
	}

	int i = 0;
	while (i < size - 2) {
		int j = 0;
		while (j < size - 2) {
			SpringDamper* s1 = new SpringDamper(particles[i][j], particles[i+2][j], ratio*3, largeDamper, largeSpring);
			SpringDamper* s2 = new SpringDamper(particles[i][j], particles[i][j+2], ratio*3, largeDamper, largeSpring);
			
			bendSprings.push_back(s1);
			bendSprings.push_back(s2);

			j += 2;
		}
		i += 2;
	}

	int j = 0;
	while (j < size - 2) {
		SpringDamper* s1 = new SpringDamper(particles[size-1][j], particles[size-1][j+2], ratio*3, largeDamper, largeSpring);
		SpringDamper* s2 = new SpringDamper(particles[j][size-1], particles[j+2][size-1], ratio*3, largeDamper, largeSpring);
		
		bendSprings.push_back(s1);
		bendSprings.push_back(s2);

		j += 2;
	}

	for (int i = 0; i < size; i++) {
		particles[i][size-1]->setFixed(true);
	}
}

void Cloth::Update(vec3 v_air) {
	
	// Gravitational force
	vec3 gravity(0.0f, -9.8f, 0.0f);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			vec3 f_gravity = gravity * particles[i][j]->getMass();
			particles[i][j]->ApplyForce(f_gravity);
		}
	}

	for (int i = 0; i < springs.size(); i++) {
		springs[i]->ComputeForce();
	}

	for (int i = 0; i < bendSprings.size(); i++) {
		// bendingSprings[i]->ComputeForce();
	}

	for (int i = 0; i < triangles.size(); i++) {
		triangles[i].ComputeForce(v_air);
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			particles[i][j]->Update(deltaTime);
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			vec3 a = particles[i][j]->getLastPosition();
			vec3 b = particles[i][j]->getPosition();

			particles[i][j]->updateLast(particles[i][j]->getPosition());
		}
	}
}

void Cloth::Draw(bool mode) {
	glPushMatrix();
	glPopMatrix();

	if (!mode) {
		glBegin(GL_TRIANGLES);
		GLfloat purple[] = { 0.5f, 0.0f, 0.6f, 1.0f };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, purple);
		for (int i = 0; i < triangles.size(); i++) {
			vec3 p1 = triangles[i].getVertex(0)->getPosition();
			vec3 p2 = triangles[i].getVertex(1)->getPosition();
			vec3 p3 = triangles[i].getVertex(2)->getPosition();
			vec3 n = triangles[i].getNormal();

			glNormal3f(n.x, n.y, n.z);
			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p3.x, p3.y, p3.z);
		}
		GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
		glEnd();

		return;
	}

	glDisable(GL_LIGHTING);
	glColor3f(0, 0, 1);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			particles[i][j]->Draw();
		}
	}

	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	for (int i = 0; i < springs.size(); i++) {
		vec3 p1 = springs[i]->P1->getPosition();
		vec3 p2 = springs[i]->P2->getPosition();
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
	}
	glEnd();

	glEnable(GL_LIGHTING);
}

void Cloth::Reset() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			particles[i][j]->Reset();
			particles[i][j]->setPosition(i*ratio - (ratio*size / 2.0f), 0.2f, j*ratio - (ratio*size / 2.0f));
		}
	}
}

void Cloth::drag(float x, float y, float z) {
	for (int i = 0; i < size; i++) {
		vec3 Last = particles[i][size - 1]->getPosition();
		particles[i][size - 1]->setPosition(Last.x + x, Last.y + y, Last.z + z);
	}
}