#include <iostream>
#include "engine/engine.h"
#include "render/renderer.h"
#include "render/shader.h"
#include "render/texture.h"
#include "engine/camera.h"
#include "render/model.h"
#include "render/shader_manager.h"
#include "render/texture_manager.h"
#include "render/material_manager.h"
#include "glad/glad.h"
#include <glm/ext/matrix_transform.inl>

MaterialManager* Singleton<MaterialManager>::singleton = nullptr;
ShaderManager* Singleton<ShaderManager>::singleton = nullptr;
TextureManager* Singleton<TextureManager>::singleton = nullptr;

void create_light_cube(ShaderProgram* shader, const Vector3& position, float scale = 1.0f, Vector3 color = Vector3(1.0f))
{
	//     7-------6
	//    /|      /|
	//   / |     / |
	//  4--+----5  |
	//  |  3----+--2
	//  | /     | /
	//  |/      |/
	//  0-------1
	float vertices[] =
	{
		// x      y      z
		-0.5f, -0.5f,  0.5f, color.r, color.g, color.b,
		 0.5f, -0.5f,  0.5f, color.r, color.g, color.b,
		 0.5f, -0.5f, -0.5f, color.r, color.g, color.b, 
		-0.5f, -0.5f, -0.5f, color.r, color.g, color.b,
		-0.5f,  0.5f,  0.5f, color.r, color.g, color.b,
		 0.5f,  0.5f,  0.5f, color.r, color.g, color.b,
		 0.5f,  0.5f, -0.5f, color.r, color.g, color.b,
		-0.5f,  0.5f, -0.5f, color.r, color.g, color.b,
	};
	std::vector<unsigned int> indices = {
		0, 1, 5, 5, 4, 0, // front
		2, 3, 7, 7, 6, 2, // back
		0, 4, 7, 7, 3, 0, // left
		1, 2, 6, 6, 5, 1, // right
		4, 5, 6, 6, 7, 4, // top
		0, 3, 2, 2, 1, 0  // bottom
	};
	Mesh::VertexFormat vf;
	vf.push_back({ 3, Mesh::VertexAttr::ElementType::Float, false });
	vf.push_back({ 3, Mesh::VertexAttr::ElementType::Float, false });

	Material* material = MaterialManager::get_singleton().get_material("light_cube");
	if (!material)
	{
		material = MaterialManager::get_singleton().create_material("light_cube", shader);
	}
	Mesh* mesh = new Mesh(vf, vertices, 8, indices, material);
	auto model = new Model(std::vector<Mesh*>{mesh});
	model->set_position(position);
	model->set_scale(Vector3(scale));
	Renderer::get_singleton().add_model(model);
}

bool init_boxes()
{
	Texture* diffuse_texture = TextureManager::get_singleton().load_texture("asset/container2.png");
	assert(diffuse_texture);
	Texture* specular_texture = TextureManager::get_singleton().load_texture("asset/container2_specular.png");
	assert(specular_texture);

	ShaderProgram* shader = ShaderManager::get_singleton().get_program("mesh");
	assert(shader && shader->valid());

	Material* material = MaterialManager::get_singleton().get_material("boxes");
	if (!material)
	{
		material = MaterialManager::get_singleton().create_material("boxes", shader, { diffuse_texture }, { specular_texture });
	}
	material->set_cull_face_type(CullFaceType::NONE);

	float vertices[] =
	{
		// x      y      z     nx     ny     nz     u     v
		// back
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		// front
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		// left
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		// right
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		// bottom
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		// top
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	};
	std::vector<unsigned int> indices = {
		 0,  1,  2,  2,  3,  0,
		 4,  5,  6,  6,  7,  4,
		 8,  9, 10, 10, 11,  8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};
	Vector3 positions[] = {
		Vector3( 0.0f,  0.0f,  0.0f),
		Vector3( 2.0f,  5.0f, -9.0f),
		Vector3(-1.5f, -2.2f, -2.5f),
		Vector3(-3.8f, -2.0f, -9.3f),
		Vector3( 2.4f, -0.4f, -3.5f),
		Vector3(-1.7f,  3.0f, -7.5f),
		Vector3( 1.3f, -2.0f, -2.5f),
		Vector3( 1.5f,  2.0f, -2.5f),
		Vector3( 1.5f,  0.2f, -1.5f),
		Vector3(-1.3f,  1.0f, -1.5f)
	};

	Mesh::VertexFormat vf;
	vf.push_back({ 3, Mesh::VertexAttr::ElementType::Float, false });
	vf.push_back({ 3, Mesh::VertexAttr::ElementType::Float, false });
	vf.push_back({ 2, Mesh::VertexAttr::ElementType::Float, false });

	static Mesh::DrawHandler pre_handler = [](Mesh& mesh, const Matrix4& model)
	{
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		// 1st. render pass, draw objects as normal, writing to the stencil buffer
		// --------------------------------------------------------------------
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);		
	};	

	static Mesh::DrawHandler post_handler = [](Mesh& mesh, const Matrix4& model)
	{
		// 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
		// Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
		// the objects' size differences, making it look like borders.
		// -----------------------------------------------------------------------------------------------------------------------------
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);

		Material* border = MaterialManager::get_singleton().get_material("border");
		if (!border)
		{
			ShaderProgram* shader = ShaderManager::get_singleton().load("border", "src/shader/border_vertex.shader", "src/shader/border_fragment.shader");
			assert(shader && shader->valid());

			border = MaterialManager::get_singleton().create_material("border", shader, {}, {});
			border->set_enable_depth_test(false);
		}

		Material* mat = mesh.get_material();
		mesh.set_material(border);
		const float scale = 1.1f;
		mesh.draw(glm::scale(model, glm::vec3(scale, scale, scale)));
		mesh.set_material(mat);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);

		glEnable(GL_STENCIL_TEST);
	};
	for (auto box_position : positions)
	{
		Mesh* mesh = new Mesh(vf, vertices, 24, indices, material);
		mesh->set_pre_draw_handler(&pre_handler);
		mesh->set_post_draw_handler(&post_handler);

		auto model = new Model(std::vector<Mesh*>{mesh});
		model->set_position(box_position);
		model->set_rotation(Vector3(20.0f, -20.0f, -10.0f));
		Renderer::get_singleton().add_model(model);
	}

	return true;
}

bool init_windows()
{
	Material* material = MaterialManager::get_singleton().get_material("window");
	if (!material)
	{
		ShaderProgram* shader = ShaderManager::get_singleton().get_program("window");
		if (!shader)
		{
			shader = ShaderManager::get_singleton().load("window", "src/shader/window_vertex.shader", "src/shader/window_fragment.shader");
		}
		assert(shader && shader->valid());

		Texture* texture = TextureManager::get_singleton().load_texture("asset/blending_transparent_window.png");
		assert(texture);

		material = MaterialManager::get_singleton().create_material("window", shader, { texture }, {});
		
		material->set_translucence(true);
		material->set_enable_alpha_blend(true);
	}
	material->set_cull_face_type(CullFaceType::NONE);

	float vertices[] =
	{
		// x      y      z     u     v
		 -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
		  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
		  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
		 -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	};
	const std::vector<unsigned int> indices = {
		 0,  1,  2,  2,  3,  0
	};
	Vector3 positions[] = {
		Vector3(0.0f,  0.0f,  0.0f),
		Vector3(0.1f,  0.3f, -1.0f),
		Vector3(0.5f, -0.2f,  2.5f)
	};

	Mesh::VertexFormat vf;
	vf.push_back({ 3, Mesh::VertexAttr::ElementType::Float, false });
	vf.push_back({ 2, Mesh::VertexAttr::ElementType::Float, false });

	for (auto pos : positions)
	{
		Mesh* mesh = new Mesh(vf, vertices, 4, indices, material);

		auto model = new Model(std::vector<Mesh*>{mesh});
		model->set_position(pos);
		//model->set_rotation(Vector3(20.0f, -20.0f, -10.0f));
		Renderer::get_singleton().add_model(model);
	}

	return true;
}

bool init_lights()
{
	ShaderProgram* shader = ShaderManager::get_singleton().load("light", "src/shader/light_vertex.shader", "src/shader/light_fragment.shader");
	assert(shader && shader->valid());
	Renderer& renderer = Renderer::get_singleton();

	Light& directional = renderer.get_directional_light();
	directional.type = LightType::Directional;
	directional.ambient = Vector3(0.05f, 0.05f, 0.05f);
	directional.diffuse = Vector3(0.4f, 0.4f, 0.4f);
	directional.specular = Vector3(0.5f, 0.5f, 0.5f);
	directional.directional.direction = Vector3(-0.2f, -1.0f, -0.3f);

	const Vector3 omni_light_positions[] = {
		Vector3( 0.7f,  3.2f,  6.0f),
		Vector3( 2.3f, -3.3f, -4.0f),
		Vector3(-4.0f,  2.0f, -12.0f),
		Vector3( 0.0f,  0.0f, -8.0f)
	};
	const Vector3 omni_light_colors[] = {
		Vector3(1.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.0f, 0.0f, 1.0f),
		Vector3(1.0f, 1.0f, 1.0f)
	};
	for (size_t i = 0; i < sizeof(omni_light_positions) / sizeof(omni_light_positions[0]); ++i)
	{
		Light omni;
		omni.type = LightType::Omni;
		omni.ambient = Vector3(0.05f, 0.05f, 0.05f) * omni_light_colors[i];
		omni.diffuse = Vector3(0.8f, 0.8f, 0.8f) * omni_light_colors[i];
		omni.specular = Vector3(1.0f, 1.0f, 1.0f) * omni_light_colors[i];
		omni.omni.position = omni_light_positions[i];
		omni.omni.constant = 1.0f;
		omni.omni.linear = 0.09f;
		omni.omni.quadratic = 0.032f;
		renderer.add_omni_light(omni);
		create_light_cube(shader, omni_light_positions[i], 0.2f, omni_light_colors[i]);
	}

	const auto* camera = Engine::get_singleton().get_camera();
	Light spot;
	spot.type = LightType::Spot;
	spot.ambient = Vector3(0.0f, 0.0f, 0.0f);
	spot.diffuse = Vector3(1.0f, 1.0f, 1.0f);
	spot.specular = Vector3(1.0f, 1.0f, 1.0f);
	spot.spot.position = camera->get_position();
	spot.spot.direction = camera->get_forward();
	spot.spot.constant = 1.0f;
	spot.spot.linear = 0.09f;
	spot.spot.quadratic = 0.032f;
	spot.spot.innerCutOff = glm::cos(glm::radians(12.5f));
	spot.spot.outerCutOff = glm::cos(glm::radians(15.0f));
	renderer.add_spot_light(spot);

	return true;
}

int main()
{
	std::shared_ptr<Engine> engine = std::make_shared<Engine>();
	std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();
	std::shared_ptr<MaterialManager> material_mgr = std::make_shared<MaterialManager>();
	std::shared_ptr<ShaderManager> shader_mgr = std::make_shared<ShaderManager>();
	std::shared_ptr<TextureManager> texture_mgr = std::make_shared<TextureManager>();

	if (!engine->startup())
		return -1;

	assert(shader_mgr->load("mesh", "src/shader/mesh_vertex.shader", "src/shader/mesh_fragment.shader"));

	if (!init_windows() || !init_lights())	// init_boxes
		return -1;

	auto model = new Model("asset/model/nanosuit/nanosuit.obj");
	model->set_position(Vector3(0.0f, 0.0f, -20.0f));
	model->set_scale(Vector3(0.3f));
	renderer->add_model(model);

	engine->run();

	texture_mgr.reset();
	shader_mgr.reset();
	material_mgr.reset();
	renderer.reset();
	engine.reset();
	
    return 0;
}