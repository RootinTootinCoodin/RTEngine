#include "Application.h"
#include "ModuleDebug.h"
#include "ModuleLoader.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ResourceMesh.h"
#include "ModuleResource.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
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
	//TODO: Make normals great again
	//glLineWidth(2.0f);
	//glColor3f(1, 0, 0);

	//glBegin(GL_LINES);
	//if (mesh->normals != nullptr)
	//{
	//	for (uint i = 0; i < mesh->num_vertices * 3; i += 3)
	//	{
	//		glVertex3f(mesh->vertices[i] - mesh->normals[i], mesh->vertices[i + 1] - mesh->normals[i + 1], mesh->vertices[i + 2] - mesh->normals[i + 2]);
	//		glVertex3f(mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2]);
	//	}
	//}
	//glEnd();
	//glColor3f(1, 1, 1);
	//glLineWidth(STANDARD_LINE_SIZE);
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

void ModuleDebug::DrawFrustum(Frustum & frustum) const
{
	if (frustum.IsFinite())
	{
		glLineWidth(2.0f);
		glColor3f(0.5f, 0.5f, 0.5f);
		glBegin(GL_LINES);

		glVertex3f(frustum.CornerPoint(0).x, frustum.CornerPoint(0).y, frustum.CornerPoint(0).z);
		glVertex3f(frustum.CornerPoint(1).x, frustum.CornerPoint(1).y, frustum.CornerPoint(1).z);

		glVertex3f(frustum.CornerPoint(0).x, frustum.CornerPoint(0).y, frustum.CornerPoint(0).z);
		glVertex3f(frustum.CornerPoint(2).x, frustum.CornerPoint(2).y, frustum.CornerPoint(2).z);

		glVertex3f(frustum.CornerPoint(0).x, frustum.CornerPoint(0).y, frustum.CornerPoint(0).z);
		glVertex3f(frustum.CornerPoint(4).x, frustum.CornerPoint(4).y, frustum.CornerPoint(4).z);

		glVertex3f(frustum.CornerPoint(7).x, frustum.CornerPoint(7).y, frustum.CornerPoint(7).z);
		glVertex3f(frustum.CornerPoint(6).x, frustum.CornerPoint(6).y, frustum.CornerPoint(6).z);

		glVertex3f(frustum.CornerPoint(6).x, frustum.CornerPoint(6).y, frustum.CornerPoint(6).z);
		glVertex3f(frustum.CornerPoint(2).x, frustum.CornerPoint(2).y, frustum.CornerPoint(2).z);

		glVertex3f(frustum.CornerPoint(7).x, frustum.CornerPoint(7).y, frustum.CornerPoint(7).z);
		glVertex3f(frustum.CornerPoint(5).x, frustum.CornerPoint(5).y, frustum.CornerPoint(5).z);

		glVertex3f(frustum.CornerPoint(7).x, frustum.CornerPoint(7).y, frustum.CornerPoint(7).z);
		glVertex3f(frustum.CornerPoint(3).x, frustum.CornerPoint(3).y, frustum.CornerPoint(3).z);

		glVertex3f(frustum.CornerPoint(3).x, frustum.CornerPoint(3).y, frustum.CornerPoint(3).z);
		glVertex3f(frustum.CornerPoint(1).x, frustum.CornerPoint(1).y, frustum.CornerPoint(1).z);

		glVertex3f(frustum.CornerPoint(1).x, frustum.CornerPoint(1).y, frustum.CornerPoint(1).z);
		glVertex3f(frustum.CornerPoint(5).x, frustum.CornerPoint(5).y, frustum.CornerPoint(5).z);

		glVertex3f(frustum.CornerPoint(3).x, frustum.CornerPoint(3).y, frustum.CornerPoint(3).z);
		glVertex3f(frustum.CornerPoint(2).x, frustum.CornerPoint(2).y, frustum.CornerPoint(2).z);

		glVertex3f(frustum.CornerPoint(4).x, frustum.CornerPoint(4).y, frustum.CornerPoint(4).z);
		glVertex3f(frustum.CornerPoint(5).x, frustum.CornerPoint(5).y, frustum.CornerPoint(5).z);

		glVertex3f(frustum.CornerPoint(6).x, frustum.CornerPoint(6).y, frustum.CornerPoint(6).z);
		glVertex3f(frustum.CornerPoint(4).x, frustum.CornerPoint(4).y, frustum.CornerPoint(4).z);

		glEnd();
		glLineWidth(STANDARD_LINE_SIZE);
		glColor3f(1, 1, 1);
	}
}


void ModuleDebug::CreatePrimitive(par_shapes_mesh_s * data, char* name,primitiveType type)
{
	GameObject* game_object = App->scene->root->AddChildren(name);
	ComponentMesh* mesh_comp = (ComponentMesh*)game_object->AddComponent(MESH);
	ResourceMesh* _primitive = nullptr;

	//It's big brain time
	switch (type)
	{
	case PLANE:
		 _primitive = (ResourceMesh*)App->resource->createNewResource(RES_MESH, 1111111111);
		 mesh_comp->AssignResourceUUID(1111111111);
		break;
	case CUBE:
		_primitive = (ResourceMesh*)App->resource->createNewResource(RES_MESH, 2222222222);
		mesh_comp->AssignResourceUUID(2222222222);


		break;
	case SPHERE:
		_primitive = (ResourceMesh*)App->resource->createNewResource(RES_MESH, 3333333333);
		mesh_comp->AssignResourceUUID(3333333333);


		break;
	case HEMISPHERE:
		 _primitive = (ResourceMesh*)App->resource->createNewResource(RES_MESH, 4444444444);
		 mesh_comp->AssignResourceUUID(4444444444);


		break;
	case CYLINDER:
		_primitive = (ResourceMesh*)App->resource->createNewResource(RES_MESH, 5555555555);
		mesh_comp->AssignResourceUUID(5555555555);


		break;
	case CONE:
		 _primitive = (ResourceMesh*)App->resource->createNewResource(RES_MESH, 6666666666);
		 mesh_comp->AssignResourceUUID(6666666666);


		break;
	case KLEIN:
		 _primitive = (ResourceMesh*)App->resource->createNewResource(RES_MESH, 7777777777);
		 mesh_comp->AssignResourceUUID(7777777777);


		break;
	case TREFOIL:
	 _primitive = (ResourceMesh*)App->resource->createNewResource(RES_MESH, 8888888888);
	 mesh_comp->AssignResourceUUID(8888888888);


		break;
	default:
		break;
	}
	
	ComponentMaterial* _material = (ComponentMaterial*)game_object->AddComponent(MATERIAL);
	_material->AssignResourceUUID(1212121212);

	if (_primitive)
	{
		// Set vertices
		_primitive->num_vertices = data->npoints * 3; // Set vertex number
		_primitive->vertices = new float[_primitive->num_vertices]; // Allocate memory
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
		LOG("%d indices.", _primitive->num_indices);

		// Set UVs
		_primitive->uvs = new float[data->npoints * 2]; // Allocate memory
		for (int i = 0; i < data->npoints * 2; i++) // Fill data
		{
			_primitive->uvs[i] = data->tcoords[i];
		}



		App->renderer3D->GenerateBufferForMesh(_primitive);
	}
}


