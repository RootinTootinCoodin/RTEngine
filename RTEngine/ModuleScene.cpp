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

update_status ModuleScene::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleScene::Draw()
{
	//DrawCubeDirectMode();
	DrawCubeVertexArray();
}

void ModuleScene::DrawCubeDirectMode()
{

	glBegin(GL_TRIANGLES);
	

	//
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	//
	//glVertex3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(1.0f, -1.0f, 1.0f);
	//glVertex3f(-1.0f, -1.0f, 1.0f);

	//
	//glVertex3f(1.0f, -1.0f, 1.0f);
	//glVertex3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(1.0f, -1.0f, -1.0f);

	glEnd();

	glLineWidth(5.0f);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);

	glEnd();
}

void ModuleScene::DrawCubeVertexArray()
{
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);
	vert_array.push_back(1.0f);

	vert_array.push_back(-1.0f);
	vert_array.push_back(-1.0f);
	vert_array.push_back(1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, vert_array.size());
	glDisableClientState(GL_VERTEX_ARRAY);
}
