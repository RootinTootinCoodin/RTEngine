#include "Application.h"
#include "ModuleScene.h"
#include "ModuleLoader.h"
#include "ModuleImGui.h"
#include "ModuleDebug.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "UIInspector.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ModuleResource.h"
#include "ResourceMesh.h"
#include "Tree.h"


#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <random>



ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Scene";
	root = new GameObject("Root", nullptr,0,nullptr);
	treeArea.SetFromCenterAndSize({ 0.0f, 0.0f, 0.0f }, treeSize);
	quadtree = new Tree(treeArea);
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
	DefaultTexture();
	GenerateCheckerTexture();

	return true;
}

update_status ModuleScene::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{
	root->RecursiveRemoveDirtyFlags();
	root->RecursiveCheckForDelete();
	if (save)
	{
		App->loader->ExportSceneOrModel(root);
		save = false;
	}
	return UPDATE_CONTINUE;
}

void ModuleScene::CreateTree()
{
	if (!quadtree->treeObjects.empty())
		quadtree->Clear();

	std::vector<GameObject*> gameObjects;
	App->scene->root->RecursiveGetChildren(&gameObjects);

	for (auto item = gameObjects.begin(); item != gameObjects.end(); item++)
	{
		if ((*item)->active && (*item)->GetAABB().Intersects(treeArea) && (*item)->is_static && (*item)->GetComponent(MESH) != nullptr)
		{
			quadtree->Insert((*item));
		}
	}
}

void ModuleScene::GenerateCheckerTexture()
{
	texture* checker = new texture;
	checker->name = "Checker Texture";
	checker->bpp = 1;
	checker->depth = 1;
	checker->path = "INTERNAL";
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

	glBindTexture(GL_TEXTURE_2D, 0);
	textures.push_back(checker);
}

void ModuleScene::DefaultTexture()
{
	texture* white = new texture;
	white->name = "Default Texture";
	white->bpp = 1;
	white->depth = 1;
	white->path = "INTERNAL";
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
	glBindTexture(GL_TEXTURE_2D, 0);
	textures.push_back(white);
}


void ModuleScene::Draw()
{
	if (drawaxis)
		DrawAxis();

	DrawGrid(gridsize);

	if (drawEditorFrustum)
		App->debug->DrawFrustum(App->camera->editorCamera->camera);

	if (debugQuad)
		quadtree->root->Draw();

	std::vector<GameObject*> gameObjects;
	root->RecursiveGetChildren(&gameObjects);

	if (!App->renderer3D->wireframe_enabled)
	{
		for (auto item = gameObjects.begin(); item != gameObjects.end(); item++)
		{
			if ((*item)->active)
			{
				glPushMatrix();
				ComponentTransform* transform = (ComponentTransform*)(*item)->GetComponent(TRANSFORM);
				float4x4 matrix = transform->GetGlobalTransformMatrix().Transposed();
				glMultMatrixf(matrix.ptr());

				if (draw_aabb || (*item)->draw_aabb)
					App->debug->DrawAABB((*item)->GetAABB());

				if (ComponentCamera* camera = (ComponentCamera*)(*item)->GetComponent(CAMERA))
				{
					if (drawEditorFrustum)
						App->debug->DrawFrustum(camera->camera);
				}


				if (ComponentMesh* mesh_comp = (ComponentMesh*)(*item)->GetComponent(MESH))
				{
					if (ResourceMesh* mesh = (ResourceMesh*)App->resource->getResource(mesh_comp->getResourceUUID()))
					{
						//if (draw_normals || mesh_comp->draw_normals)
						//	App->debug->DrawNormals(mesh);

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
				glPopMatrix();
			}
		}
	}


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	for (auto item = gameObjects.begin(); item != gameObjects.end(); item++)
	{
		if ((*item)->active)
		{
			glPushMatrix();
			ComponentTransform* transform = (ComponentTransform*)(*item)->GetComponent(TRANSFORM);
			glMultMatrixf(transform->GetGlobalTransformMatrix().Transposed().ptr());

			if (draw_aabb || (*item)->draw_aabb)
				App->debug->DrawAABB((*item)->GetAABB());

			if (showedges || (*item) == selected_go || App->renderer3D->wireframe_enabled)
			{
				if (ComponentMesh* mesh_comp = (ComponentMesh*)(*item)->GetComponent(MESH))
				{
					if (ResourceMesh* mesh = (ResourceMesh*)App->resource->getResource(mesh_comp->getResourceUUID()))
					{
						//if (draw_normals || mesh->draw_normals)
						//	App->debug->DrawNormals(mesh);

						glLineWidth(3.0f);

						glColor3f(0, 0, 1);
						glEnableClientState(GL_VERTEX_ARRAY);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
						glVertexPointer(3, GL_FLOAT, 0, &mesh->vertices[0]);



						glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

						glDisableClientState(GL_VERTEX_ARRAY);
					}
				}
			}
			glPopMatrix();
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



