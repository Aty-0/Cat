#include "game/components/drawable.h"
#include "graphics/renderer.h"
#include "graphics/vertex.h"
#include "core/utils/logger.h"
#include "game/game_object.h"

namespace cat::game::components
{
	drawable::drawable() : 
		m_color(glm::vec4(1,1,1,1))
	{
		std::vector<graphics::vertex> vb_data = { { glm::vec3(1.0f,  1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, -1.0f) },      // top right
												{ glm::vec3(1.0f, -1.0f, 0.0f),   glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },      // bottom right
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

		graphics::buffer::unbind_all();

		m_shader = std::make_shared<graphics::shader>(*new graphics::shader());

		CAT_ASSERT(m_shader->load("sprite"));
	}

	drawable::~drawable()
	{
		m_shader.reset();		
		m_vertex_buffer->clear();
		m_vertex_buffer.reset();
		m_index_buffer->clear();
		m_index_buffer.reset();
	}

	void drawable::on_render(graphics::renderer* render) 
	{
		m_shader->bind();
		const auto transform = get_owner()->get_transform();
		auto& world_mat = transform->get_world_matrix();
		world_mat = transform->get_matrix_transformation();
		m_shader->set_mat4("transform.world", world_mat);
		m_shader->set_vec4("drawable.color", m_color);

		m_vertex_buffer->bind();

		render->draw_elements(6, GL_TRIANGLES);
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