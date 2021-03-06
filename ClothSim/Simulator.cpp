#include "Simulator.h"

////////////////////////////////////////////////////////////////////////////////

static Simulator *SIM=0;
bool fillMode = true;

// controls I want: 
//	wind speed/direction
//	zoom

float windX = 0.0f;
float windY = 0.0f;
float windZ = 0.0f;
float zoom = 30.0f;

void ATBar() {
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(800, 600);

	TwBar *myBar;
	myBar = TwNewBar("Wind Controls");
	TwAddVarRW(myBar, "windX", TW_TYPE_FLOAT, &windX, "label='x velocity' min=-10 max=10 group='Wind Controls' visible=true step=0.5 precision=1");
	TwAddVarRW(myBar, "windY", TW_TYPE_FLOAT, &windY, "label='y velocity' min=-10 max=10 group='Wind Controls' visible=true step=0.5 precision=1");
	TwAddVarRW(myBar, "windZ", TW_TYPE_FLOAT, &windZ, "label='z velocity' min=-10 max=10 group='Wind Controls' visible=true step=0.5 precision=1");
	TwAddVarRW(myBar, "zoom", TW_TYPE_FLOAT, &zoom, "min=10 max=90");
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	SIM=new Simulator("Cloth",argc,argv);
	
	// AntTweakBar
	ATBar();

	glutMainLoop();
	delete SIM;

	return 0;
}

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{SIM->Draw();}
static void idle()										{SIM->Update();}
static void resize(int x, int y)						{ SIM->Resize(x, y); TwWindowSize(x, y); }
static void keyboard(unsigned char key, int x, int y)	{ if (!TwEventKeyboardGLUT(key,x,y)) { SIM->Keyboard(key, x, y); } }
static void mousebutton(int btn, int state, int x, int y) { if (!TwEventMouseButtonGLUT(btn,state,x,y)) { SIM->MouseButton(btn, state, x, y); } }
static void mousemotion(int x, int y)					{ if (!TwEventMouseMotionGLUT(x,y)) { SIM->MouseMotion(x, y); } }

Simulator::Simulator(const char *windowTitle,int argc,char **argv) {
	WinX=800;
	WinY=600;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 100, 100 );
	WindowHandle = glutCreateWindow( windowTitle );
	glutSetWindowTitle( windowTitle );
	glutSetWindow( WindowHandle );

	// Background color
	glClearColor( 0., 0., 0., 1. );

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( (keyboard) );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );

	// Initialize GLEW
	glewInit();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	// Initialize components
	Program=new ShaderProgram("Model.glsl",ShaderProgram::eRender);
	
	air_velocity = vec3(0.0f);
	pSys = new ParticleSystem(15,15);
	
	Cam=new Camera;
	Cam->SetAspect(float(WinX)/float(WinY));
	printf("done loading\n\n");
}

Simulator::~Simulator() {
	delete Program;
	delete Cam;

	glFinish();
	glutDestroyWindow(WindowHandle);
}

void Simulator::Update() {
	air_velocity = vec3(windX, windY, windZ);

	// Update the components in the world
	deltaTime = 0.0017f;
	for (int i = 0; i < 5; i++) {
		pSys->Update(deltaTime, air_velocity);
	}

	Cam->SetDistance(zoom);
	Cam->Update();

	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

void Simulator::Reset() {
	Cam->Reset();
	Cam->SetAspect(float(WinX)/float(WinY));
}

void Simulator::Draw() {
	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pSys->Draw(Cam->GetViewProjectMtx(), Program->GetProgramID());

	// Finish drawing scene
	glFinish();

	TwDraw();
	glutSwapBuffers();
}

void Simulator::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	TwTerminate();
	exit(0);
}

void Simulator::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam->SetAspect(float(WinX)/float(WinY));
}

void Simulator::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
		case 't':		// toggle wireframe on/off
			fillMode = !fillMode;
			if (fillMode) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				printf("toggle fill\n");
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				printf("toggle wireframe\n");
			}
			break;
		case 'w':
			// decrease z of cloth
			pSys->Move(vec3(0.0f, 0.0f, -1.0f));
			break;
		case 'a':
			// decrease x of cloth
			pSys->Move(vec3(-1.0f, 0.0f, 0.0f));
			break;
		case 's':
			// increase z of cloth
			pSys->Move(vec3(0.0f, 0.0f, 1.0f));
			break;
		case 'd':
			// increase x of cloth
			pSys->Move(vec3(1.0f, 0.0f, 0.0f));
			break;
		case 'r':
			pSys->Move(vec3(0.0f, 1.0f, 0.0f));
			break;
		case 'f':
			pSys->Move(vec3(0.0f, -1.0f, 0.0f));
			break;
	}
}

void Simulator::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

void Simulator::MouseMotion(int nx,int ny) {
	int maxDelta=100;
	int dx = glm::clamp(nx - MouseX,-maxDelta,maxDelta);
	int dy = glm::clamp(-(ny - MouseY),-maxDelta,maxDelta);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam->SetAzimuth(Cam->GetAzimuth()+dx*rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline()-dy*rate,-90.0f,90.0f));
	}
	if(RightDown) {
		const float rate=0.005f;
		float dist=glm::clamp(Cam->GetDistance()*(1.0f-dx*rate),0.01f,1000.0f);
		zoom = dist;
		Cam->SetDistance(dist);
	}
}