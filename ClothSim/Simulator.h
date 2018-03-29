#pragma once

#include <AntTweakBar.h>
#include "Camera.h"
#include "Shader.h"
#include "ParticleSystem.h"
#include <iostream>

static const float V_AIR = 1.0f;

class Simulator {
public:
	Simulator(const char *windowTitle,int argc,char **argv);
	~Simulator();

	void Update();
	void Reset();
	void Draw();

	void Quit();

	// Event handlers
	void Resize(int x,int y);
	void Keyboard(int key,int x,int y);
	void MouseButton(int btn,int state,int x,int y);
	void MouseMotion(int x,int y);

private:
	// Window management
	int WindowHandle;
	int WinX,WinY;

	// Input
	bool LeftDown,MiddleDown,RightDown;
	int MouseX,MouseY;

	// Interactivity (skeleton joint manipulation)
	int jointNo;
	int numJoints;
	int jointMode;

	float deltaTime;

	// Components
	ShaderProgram *Program;
	Camera *Cam;
	ParticleSystem* pSys;
	vec3 air_velocity;
};