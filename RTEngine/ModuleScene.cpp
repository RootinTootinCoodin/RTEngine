#include "Application.h"
#include "ModuleScene.h"

#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init(JSON_Object* config)
{
	DrawCubeVertexArray();
	DrawGrid(10);

	glGenBuffers(1, (GLuint*) &(vertex_id));
	glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, vert_array.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//glGenBuffers(1, (GLuint*) &(index_id));
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, index_array.data(), GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

bool ModuleScene::Start()
{
	return true;
}

update_status ModuleScene::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleScene::Draw()
{
	DrawAxis();
	DrawGrid(gridsize);
	//DrawCubeDirectMode();

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	glVertexPointer(3, GL_FLOAT, 0, vert_array.data());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleScene::DrawGrid(int halfsize)
{
	glBegin(GL_LINES);
	glColor3f(0.75f, 0.75f, 0.75f);
	for (int i = -halfsize; i <= halfsize; i++)
	{
		glVertex3f((float)i, 0, (float)-halfsize);
		glVertex3f((float)i, 0, (float)halfsize);

		glVertex3f((float)-halfsize, 0, (float)i);
		glVertex3f((float)halfsize, 0, (float)i);
	}
	glEnd();
}

void ModuleScene::DrawAxis()
{
	glLineWidth(5.0f);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 1.0f, 0.0f);
	glVertex3f(3.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(2.0f, 1.0f, 0.0f);
	glVertex3f(2.0f, 2.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(2.0f, 1.0f, 0.0f);
	glVertex3f(2.0f, 1.0f, 1.0f);

	glEnd();
}

void ModuleScene::DrawCubeDirectMode()
{
	glBegin(GL_TRIANGLES);

	// Front
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	//
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	//
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Left
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	//
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	// Back
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	//
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	// Top
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	//
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Bottom
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	//
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glEnd();
}

void ModuleScene::DrawCubeVertexArray()
{
	//FRONT
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);

	//
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);

	//BACK
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);

	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);

	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);

	//
	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);

	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);

	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);

	//RIGHT
	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);

	//
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);

	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);

	//LEFT
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);

	//
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);

	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);

	//TOP
	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);

	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);

	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);

	//
	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);

	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);

	//BOTTOM
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);

	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);

	//
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(-1.0f);

	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);
}
