#include "Application.h"
#include "ModuleScene.h"
#include "ModuleLoader.h"
#include "ModuleImGui.h"
#include "UIInspector.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"


#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <random>



ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Scene";
	root = new GameObject("Root", nullptr);
}


ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init(JSON_Object* config)
{


	return true;
}

bool ModuleScene::Start()
{
	//glGenBuffers(1, (GLuint*) &(vertex_id));
	//glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, vert_array.data(), GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	DefaultTexture();
	GenerateCheckerTexture();

	return true;
}

update_status ModuleScene::Update(float dt)
{
	root->RecursiveHierarchyChildren();
	return UPDATE_CONTINUE;
}

void ModuleScene::GenerateCheckerTexture()
{
	texture* checker = new texture;
	checker->width = checker_size;
	checker->height = checker_size;

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
	glGenTextures(1, &checker->id_texture);
	glBindTexture(GL_TEXTURE_2D, checker->id_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checker_size, checker_size,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	textures.push_back(checker);
}

void ModuleScene::DefaultTexture()
{
	texture* white = new texture;
	white->width = checker_size;
	white->height = checker_size;

	for (int i = 0; i < checker_size; i++) {
		for (int j = 0; j < checker_size; j++) {
			checkImage[i][j][0] =255;
			checkImage[i][j][1] =255;
			checkImage[i][j][2] =255;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &white->id_texture);
	glBindTexture(GL_TEXTURE_2D, white->id_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checker_size, checker_size,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	textures.push_back(white);
}


void ModuleScene::Draw()
{
	if (drawaxis)
		DrawAxis();

	DrawGrid(gridsize);
	glBindTexture(GL_TEXTURE_2D, id_image);

	std::vector<GameObject*> gameObjects;
	root->RecursiveGetChildren(&gameObjects);

	for (auto item = gameObjects.begin(); item != gameObjects.end(); item++)
	{
		if (ComponentMesh* mesh = (ComponentMesh*)(*item)->GetComponent(MESH))
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
			glVertexPointer(3, GL_FLOAT, 0, &mesh->vertices[0]);

			ComponentMaterial* material = (ComponentMaterial*)(*item)->GetComponent(MATERIAL);
			if (material)
			{
				glBindTexture(GL_TEXTURE_2D, material->id_texture);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, 0, &mesh->uvs[0]);
			}

			glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			if (material)
			{
				glBindTexture(GL_TEXTURE_2D, 0);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}

			glDisableClientState(GL_VERTEX_ARRAY);

		}
	}


	glLineWidth(3.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (auto item = gameObjects.begin(); item != gameObjects.end(); item++)
	{
		if (showedges || (*item) == selected_go)
		{
			if (ComponentMesh* mesh = (ComponentMesh*)(*item)->GetComponent(MESH))
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
				glVertexPointer(3, GL_FLOAT, 0, &mesh->vertices[0]);

				glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				glDisableClientState(GL_VERTEX_ARRAY);
			}
		}
	}
	glLineWidth(STANDARD_LINE_SIZE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
}

void ModuleScene::DrawGrid(int halfsize)
{
	glLineWidth(0.5f);
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

	glLineWidth(STANDARD_LINE_SIZE);

}

void ModuleScene::DrawAxis()
{
	glLineWidth(5.0f);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f); //
	glVertex3f(0.0f + axis_x, 0.0f + axis_y, 0.0f + axis_z);
	glVertex3f(1.0f + axis_x, 0.0f + axis_y, 0.0f + axis_z);

	glColor3f(0.0f, 1.0f, 0.0f); //
	glVertex3f(0.0f + axis_x, 0.0f + axis_y, 0.0f + axis_z);
	glVertex3f(0.0f + axis_x, 1.0f + axis_y, 0.0f + axis_z);

	glColor3f(0.0f, 0.0f, 1.0f); //
	glVertex3f(0.0f + axis_x, 0.0f + axis_y, 0.0f + axis_z);
	glVertex3f(0.0f + axis_x, 0.0f + axis_y, 1.0f + axis_z);

	glEnd();
	glLineWidth(STANDARD_LINE_SIZE);

}



