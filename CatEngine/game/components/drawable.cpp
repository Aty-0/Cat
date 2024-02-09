#include "game/components/drawable.h"
#include "graphics/renderer.h"
#include "graphics/vertex.h"
#include "core/utils/logger.h"
#include "game/game_object.h"
#include "core/game_window.h"

namespace cat::game::components
{
	drawable::drawable() : 
		m_color(glm::vec4(1,1,1,1))
	{
		std::vector<graphics::vertex> vb_data = { { glm::vec3(0.5f,  1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, -1.0f) },      // top right
												{ glm::vec3(0.5f, -1.0f, 0.0f),   glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },      // bottom right
												{ glm::vec3(-1.0f, -1.0f, 0.0f),  glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(-1.0f, 0.0f) },      // bottom left
												{ glm::vec3(-1.0f,  1.0f, 0.0f),  glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(-1.0f, -1.0f) }       // top left 
		};
		
		m_vertex_buffer = std::make_shared<graphics::vertex_buffer>(*new graphics::vertex_buffer());
		
		m_vertex_buffer->gen();
		m_vertex_buffer->set_buffer_data<graphics::vertex>(vb_data, GL_STATIC_DRAW);
		
		std::vector<std::uint32_t> ib_data =
		{
			0, 1, 3,  // first triangle
			1, 2, 3   // second triangle
		};
		
		m_index_buffer = std::make_shared<graphics::index_buffer>(*new graphics::index_buffer());
		m_index_buffer->gen();
		m_index_buffer->set_buffer_data<std::uint32_t>(ib_data, GL_STATIC_DRAW);
		
		m_vertex_buffer->set_attrib(0, static_cast<std::uint32_t>(ib_data.size() / 2), GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, pos)));
		m_vertex_buffer->set_attrib(1, static_cast<std::uint32_t>(ib_data.size() / 2), GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, color)));
		m_vertex_buffer->set_attrib(2, static_cast<std::uint32_t>(ib_data.size() / 2), GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, uv)));
		
		m_vertex_buffer->unbind_all();
		
		m_shader = std::make_shared<graphics::shader>(*new graphics::shader());
		CAT_ASSERT(m_shader->load("sprite"));
		
		m_texture = std::make_shared<graphics::texture>(*new graphics::texture());
		
		set_texture("default_texture");		
	}

	drawable::~drawable()
	{
		m_shader.reset();	
		m_texture.reset();
		m_vertex_buffer->clear();
		m_vertex_buffer.reset();
		m_index_buffer->clear();
		m_index_buffer.reset();
	}

	void drawable::rescale()
	{
		const auto owner = get_owner();
		if (owner != nullptr)
		{
			const auto game_window = core::game_window::get_instance();
			const float ar_gw = static_cast<float>(game_window->get_width() / game_window->get_height());
			const float ar = static_cast<float>(m_texture->get_width() / m_texture->get_height());
			const float ar2 = static_cast<float>(m_texture->get_height() / m_texture->get_width());
			owner->get_transform()->set_scale_factor({ ar_gw + ar, ar_gw + ar2, 1 });
		}
	}

	void drawable::set_texture(const char* texture_name)
	{
		CAT_ASSERT(m_texture->load(texture_name));
	}

	void drawable::on_render(graphics::renderer* render) 
	{
		// TODO: Do this when we are change res for window and change texture 
		rescale();
		
		m_shader->bind();
		const auto transform = get_owner()->get_transform();
		auto& world_mat = transform->get_world_matrix();
		world_mat = transform->get_matrix_transformation();
		m_shader->set_mat4("transform.world", world_mat);
		m_shader->set_vec4("drawable.color", m_color);
		
		m_texture->bind();
		m_vertex_buffer->bind();
		
		render->draw_elements(6, GL_TRIANGLES);
		m_texture->unbind();
	}

	void drawable::set_color(glm::vec4 color)
	{
		m_color = color;
	}

	glm::vec4 drawable::get_color() const
	{
		return m_color;
	}
}