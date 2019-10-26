#include "Application.h"
#include "ModuleDebug.h"
#include "ModuleLoader.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "par_shapes/par_shapes.h"

ModuleDebug::ModuleDebug(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "debug";
}

ModuleDebug::~ModuleDebug()
{
}

bool ModuleDebug::Init(JSON_Object * config)
{
	return true;
}

bool ModuleDebug::CleanUp()
{
	return true;
}

void ModuleDebug::CreatePrimitive(par_shapes_mesh_s * data, char* name)
{
	GameObject* game_object = App->scene->root->AddChildren(name);
	ComponentMesh* _primitive = (ComponentMesh*)game_object->AddComponent(MESH);
	ComponentMaterial* _material = (ComponentMaterial*)game_object->AddComponent(MATERIAL);
	_material->CopyTextureToThis(App->scene->textures[0]);
	// Set vertices
	_primitive->num_vertices = data->npoints * 3; // Set vertex number
	_primitive->vertices = new float[_primitive->num_vertices * 3]; // Allocate memory
	for (int i = 0; i < _primitive->num_vertices; i++) // Fill data
	{
		_primitive->vertices[i] = data->points[i];
	}
	LOG("New mesh (primitive) with %d vertices,", _primitive->num_vertices);

	// Set index
	_primitive->num_indices = data->ntriangles * 3; // Set index number
	_primitive->indices = new uint[_primitive->num_indices]; // Allocate memory
	for (int i = 0; i < _primitive->num_indices; i++) // Fill data
	{
		_primitive->indices[i] = (uint)data->triangles[i];
	}
	LOG("%d indices.", _primitive->num_vertices);

	// Set UVs
	_primitive->num_uvs = data->npoints * 2; // Set UV number
	_primitive->uvs = new float[data->npoints * 2]; // Allocate memory
	for (int i = 0; i < _primitive->num_uvs; i++) // Fill data
	{
		_primitive->uvs[i] = data->tcoords[i];
	}

	// Get and render normals
	//RenderNormals(data);

	model* new_model = new model;

	App->renderer3D->GenerateBufferForMesh(_primitive);
}

void ModuleDebug::RenderNormals(par_shapes_mesh_s* data)
{
	glLineWidth(2.0f);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL); App->renderer3D->color_material_enabled = true;

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.5f, 0.5f);

	for (int i = 0; data->normals[i] != NULL; i = i+3)
	{
		glVertex3d(data->points[i], data->points[i + 1], data->points[i + 2]);
		glVertex3f(data->normals[i], data->normals[i+1], data->normals[i+2]);
	}

	glEnd();
	glLineWidth(STANDARD_LINE_SIZE);

}
