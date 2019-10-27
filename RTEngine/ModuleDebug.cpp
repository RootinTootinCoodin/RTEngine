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

void ModuleDebug::DrawNormals(ComponentMesh * mesh)
{
	glLineWidth(2.0f);
	glColor3f(1, 0, 0);

	glBegin(GL_LINES);

	for (uint i = 0; i < mesh->num_vertices * 3; i += 3)
	{
		glVertex3f(mesh->vertices[i] - mesh->normals[i], mesh->vertices[i+1] - mesh->normals[i+1], mesh->vertices[i+2] - mesh->normals[i+2]);
		glVertex3f(mesh->vertices[i], mesh->vertices[i+1], mesh->vertices[i+2]);
	}
	glEnd();
	glColor3f(1, 1, 1);
	glLineWidth(STANDARD_LINE_SIZE);
}

void ModuleDebug::DrawAABB(AABB & aabb) const
{
	if (aabb.IsFinite())
	{
		glLineWidth(2.0f);
		glColor3f(0, 1, 0);
		glBegin(GL_LINES);

		glVertex3f(aabb.CornerPoint(0).x, aabb.CornerPoint(0).y, aabb.CornerPoint(0).z);
		glVertex3f(aabb.CornerPoint(1).x, aabb.CornerPoint(1).y, aabb.CornerPoint(1).z);

		glVertex3f(aabb.CornerPoint(0).x, aabb.CornerPoint(0).y, aabb.CornerPoint(0).z);
		glVertex3f(aabb.CornerPoint(2).x, aabb.CornerPoint(2).y, aabb.CornerPoint(2).z);

		glVertex3f(aabb.CornerPoint(0).x, aabb.CornerPoint(0).y, aabb.CornerPoint(0).z);
		glVertex3f(aabb.CornerPoint(4).x, aabb.CornerPoint(4).y, aabb.CornerPoint(4).z);

		glVertex3f(aabb.CornerPoint(7).x, aabb.CornerPoint(7).y, aabb.CornerPoint(7).z);
		glVertex3f(aabb.CornerPoint(6).x, aabb.CornerPoint(6).y, aabb.CornerPoint(6).z);

		glVertex3f(aabb.CornerPoint(6).x, aabb.CornerPoint(6).y, aabb.CornerPoint(6).z);
		glVertex3f(aabb.CornerPoint(2).x, aabb.CornerPoint(2).y, aabb.CornerPoint(2).z);

		glVertex3f(aabb.CornerPoint(7).x, aabb.CornerPoint(7).y, aabb.CornerPoint(7).z);
		glVertex3f(aabb.CornerPoint(5).x, aabb.CornerPoint(5).y, aabb.CornerPoint(5).z);

		glVertex3f(aabb.CornerPoint(7).x, aabb.CornerPoint(7).y, aabb.CornerPoint(7).z);
		glVertex3f(aabb.CornerPoint(3).x, aabb.CornerPoint(3).y, aabb.CornerPoint(3).z);

		glVertex3f(aabb.CornerPoint(3).x, aabb.CornerPoint(3).y, aabb.CornerPoint(3).z);
		glVertex3f(aabb.CornerPoint(1).x, aabb.CornerPoint(1).y, aabb.CornerPoint(1).z);

		glVertex3f(aabb.CornerPoint(1).x, aabb.CornerPoint(1).y, aabb.CornerPoint(1).z);
		glVertex3f(aabb.CornerPoint(5).x, aabb.CornerPoint(5).y, aabb.CornerPoint(5).z);

		glVertex3f(aabb.CornerPoint(3).x, aabb.CornerPoint(3).y, aabb.CornerPoint(3).z);
		glVertex3f(aabb.CornerPoint(2).x, aabb.CornerPoint(2).y, aabb.CornerPoint(2).z);

		glVertex3f(aabb.CornerPoint(4).x, aabb.CornerPoint(4).y, aabb.CornerPoint(4).z);
		glVertex3f(aabb.CornerPoint(5).x, aabb.CornerPoint(5).y, aabb.CornerPoint(5).z);

		glVertex3f(aabb.CornerPoint(6).x, aabb.CornerPoint(6).y, aabb.CornerPoint(6).z);
		glVertex3f(aabb.CornerPoint(4).x, aabb.CornerPoint(4).y, aabb.CornerPoint(4).z);

		glEnd();
		glLineWidth(STANDARD_LINE_SIZE);
		glColor3f(1, 1, 1);
	}
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
