#include "Application.h"
#include "ModuleScene.h"
#include "ModuleLoader.h"

#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Scene";
}


ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init(JSON_Object* config)
{
	DrawCubeVertexArray();
	//ilInit();



	//glGenBuffers(1, (GLuint*) &(index_id));
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, index_array.data(), GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

bool ModuleScene::Start()
{
	//glGenBuffers(1, (GLuint*) &(vertex_id));
	//glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, vert_array.data(), GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//GenerateCheckerTexture();

	return true;
}

update_status ModuleScene::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleScene::GenerateCheckerTexture()
{
	for (int i = 0; i < checker_size; i++) {
		for (int j = 0; j < checker_size; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &id_image);
	glBindTexture(GL_TEXTURE_2D, id_image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checker_size, checker_size,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
}


void ModuleScene::Draw()
{
	DrawAxis();
	DrawGrid(gridsize);
	glBindTexture(GL_TEXTURE_2D, id_image);

	//DrawCubeDirectMode();
	int result = 0;

	if (models.size() > 0)
	{
		model* tmp = models[current_model_index];

		for (auto item = tmp->meshes.begin(); item != tmp->meshes.end(); item++)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*item)->id_index);
			glVertexPointer(3, GL_FLOAT, 0, &(*item)->vertices[0]);

			if (textures.size() > 0)
			{
				glBindTexture(GL_TEXTURE_2D, *textures[current_texture_index]);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, 0, &(*item)->uvs[0]);
			}

			glDrawElements(GL_TRIANGLES, (*item)->num_indices, GL_UNSIGNED_INT, NULL);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			if (textures.size() > 0)
			{
				glBindTexture(GL_TEXTURE_2D, 0);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}

			glDisableClientState(GL_VERTEX_ARRAY);
		
		}
	}
	
}

void ModuleScene::DrawGrid(int halfsize)
{
	glLineWidth(0.2f);
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
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	//
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
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