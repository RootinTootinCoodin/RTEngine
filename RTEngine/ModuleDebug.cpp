#include "Application.h"
#include "ModuleDebug.h"
#include "ModuleLoader.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
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

void ModuleDebug::CreatePrimitive(par_shapes_mesh_s * data)
{
	mesh* _primitive = new mesh;

	_primitive->num_vertices = data->npoints;
	_primitive->vertices = new float[_primitive->num_vertices];

	memcpy(_primitive->vertices, data->points, sizeof(float) * _primitive->num_vertices);
	LOG("New mesh (primitive) with %d vertices", _primitive->num_vertices);

	_primitive->vertices[50];

	_primitive->has_texture = true;
	_primitive->uvs = new float[_primitive->num_vertices * 2];

	memcpy(_primitive->uvs, data->tcoords, sizeof(float) * _primitive->num_vertices * 2);

	//for (int t = 0; t <= _primitive->num_vertices * 2; t++)
	//{
	//	_primitive->uvs[t] = data->tcoords[t];

	//	if (t % 2 != 0)
	//	LOG("UV: %f, %f", _primitive->uvs[t], _primitive->uvs[t + 1]);
	//}

	_primitive->num_indices = data->ntriangles * 3;
	_primitive->indices = new uint[_primitive->num_indices];

	memcpy(&_primitive->indices, data->triangles, _primitive->num_indices * sizeof(uint));

	App->renderer3D->GenerateBufferForMesh(_primitive);
	App->scene->model.push_back(_primitive);
	App->scene->model_loaded = true;
}
