#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleLoader.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>



#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "GL/lib/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Renderer3D";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(JSON_Object* config)
{

	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	glewInit();


	if(ret == true)
	{
		//Use Vsync
		if(vsync && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST); depth_test_enabled = true;
		glEnable(GL_CULL_FACE); cullface_enabled = true;
		//glCullFace(GL_BACK);
		lights[0].Active(true);

		glEnable(GL_COLOR_MATERIAL); color_material_enabled = true;
		glEnable(GL_TEXTURE_2D); texture2D_enabled = true;

	}
	// Projection matrix for
	OnResize(App->window->width, App->window->height);
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glBindTexture(GL_TEXTURE_2D, framebuffer_texture);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glLoadMatrixf(App->camera->editorCamera->GetViewMatrix());

	lights[0].SetPos(App->camera->cameraPos.x, App->camera->cameraPos.y, App->camera->cameraPos.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	App->scene->Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ImGui::Render();

	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(App->camera->editorCamera->GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GenerateFramebuffer(width, height);

}

void ModuleRenderer3D::BindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void ModuleRenderer3D::UnbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ModuleRenderer3D::UpdateFaceCullSetting(int state)
{
	switch (state)
	{
		case 0:
			glCullFace(GL_BACK);
			cull_mode = 0;
			break;
		case 1:
			glCullFace(GL_FRONT);
			cull_mode = 1;
			break;
		case 2:
			glCullFace(GL_FRONT_AND_BACK);
			cull_mode = 2;
			break;
	}
}

void ModuleRenderer3D::SetFaceCull()
{
	if (cullface_enabled)
	{
		glEnable(GL_CULL_FACE);
	}

	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void ModuleRenderer3D::SetDepthTest()
{
	if (!depth_test_enabled)
	{
		glEnable(GL_DEPTH_TEST);
	}

	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

void ModuleRenderer3D::SetLighting()
{
	if (lighting_enabled)
	{
		glEnable(GL_LIGHTING);
	}

	else
	{
		glDisable(GL_LIGHTING);
	}
}

void ModuleRenderer3D::SetColorMaterial()
{
	if (!color_material_enabled)
	{
		glEnable(GL_COLOR_MATERIAL);
	}

	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}
}

void ModuleRenderer3D::SetTexture2D()
{
	if (texture2D_enabled)
	{
		glEnable(GL_TEXTURE_2D);
	}

	else
	{
		glDisable(GL_TEXTURE_2D);
	}
}

void ModuleRenderer3D::SetWireframe()
{
	if (wireframe_enabled)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}



bool ModuleRenderer3D::GenerateBufferForMesh(ComponentMesh * mesh)
{
	glGenBuffers(1, (GLuint*)&(mesh->id_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*mesh->num_indices, mesh->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	return true;
}

void ModuleRenderer3D::GenerateFramebuffer(uint width, uint height)
{
	//Create the framebuffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &framebuffer_texture);
	glBindTexture(GL_TEXTURE_2D, framebuffer_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_texture, 0);

	glGenRenderbuffers(1, &renderbuffer_object);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_object);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_object);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
