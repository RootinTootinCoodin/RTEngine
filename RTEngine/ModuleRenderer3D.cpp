#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleLoader.h"
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
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		//glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}
	// Projection matrix for
	OnResize(App->window->width, App->window->height);
	GenerateFramebuffer();
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glBindTexture(GL_TEXTURE_2D, framebuffer_texture);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());


	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::Update(float dt)
{

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	int result = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &result);
	App->scene->Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ImGui::Render();

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &result);


	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	App->app_log("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

void ModuleRenderer3D::SetFaceCull(bool state)
{
	if (state)
	{
		glEnable(GL_CULL_FACE);
		cullface_enabled = true;
	}

	else
	{
		glDisable(GL_CULL_FACE);
		cullface_enabled = false;
	}
}

void ModuleRenderer3D::SetDepthTest(bool state)
{
	if (state)
	{
		glEnable(GL_DEPTH_TEST);
		depth_test_enabled = true;
	}

	else
	{
		glDisable(GL_DEPTH_TEST);
		depth_test_enabled = false;
	}
}

void ModuleRenderer3D::SetLighting(bool state)
{
	if (state)
	{
		glEnable(GL_LIGHTING);
		lighting_enabled = true;
	}

	else
	{
		glDisable(GL_LIGHTING);
		lighting_enabled = false;
	}
}

void ModuleRenderer3D::SetColorMaterial(bool state)
{
	if (state)
	{
		glEnable(GL_COLOR_MATERIAL);
		color_material_enabled = true;
	}

	else
	{
		glDisable(GL_COLOR_MATERIAL);
		color_material_enabled = false;
	}
}

void ModuleRenderer3D::SetTexture2D(bool state)
{
	if (state)
	{
		glEnable(GL_TEXTURE_2D);
		texture2D_enabled = true;
	}

	else
	{
		glDisable(GL_TEXTURE_2D);
		texture2D_enabled = false;
	}
}

void ModuleRenderer3D::SetWireframe(bool state)
{
	if (state)
	{
		switch (cull_mode)
		{
			case 0:
				glPolygonMode(GL_BACK, GL_LINE);
				break;
			case 1:
				glPolygonMode(GL_FRONT, GL_LINE);
				break;
			case 2:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;
		}
		wireframe_enabled = true;
	}

	else
	{
		switch (cull_mode)
		{
		case 0:
			glPolygonMode(GL_BACK, GL_FILL);
			break;
		case 1:
			glPolygonMode(GL_FRONT, GL_FILL);
			break;
		case 2:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}
		wireframe_enabled = false;
	}
}

bool ModuleRenderer3D::GenerateBufferForMesh(mesh * mesh)
{
	glGenBuffers(1, (GLuint*)&(mesh->id_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*)&(mesh->id_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*mesh->num_indices, mesh->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	return true;
}

void ModuleRenderer3D::GenerateFramebuffer()
{
	//Create the framebuffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &framebuffer_texture);
	glBindTexture(GL_TEXTURE_2D, framebuffer_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->window->width, App->window->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_texture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
