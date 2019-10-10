#ifndef __MODULE_RENDERER_3D_H__
#define __MODULE_RENDERER_3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#define MAX_LIGHTS 8

class mesh;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(JSON_Object* config);
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void BindFramebuffer();
	void UnbindFramebuffer();

	void UpdateFaceCullSetting(int state);
	void SetFaceCull(bool state);
	void SetDepthTest(bool state);
	void SetLighting(bool state);
	void SetColorMaterial(bool state);
	void SetTexture2D(bool state);
	void SetWireframe(bool state);

	bool GenerateBufferForMesh(mesh* mesh);
private:
	void GenerateFramebuffer();
public:

	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	uint framebuffer = 0;
	uint framebuffer_texture = 0;
	uint cull_mode = 0;
	bool vsync = false;
	bool cullface_enabled = true;
	bool depth_test_enabled = true;
	bool lighting_enabled = false;
	bool color_material_enabled = true;
	bool texture2D_enabled = true;
	bool wireframe_enabled = false;
};

#endif // __MODULE_RENDERER_H__