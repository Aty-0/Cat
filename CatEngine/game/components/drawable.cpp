#include "game/components/drawable.h"
#include "graphics/renderer.h"
#include "game/game_object.h"

namespace cat::game::components
{
	drawable::drawable() : 
		m_color(glm::vec4(1,1,1,1))
	{		

	}

	drawable::~drawable()
	{
		core::utils::safe_delete(m_piece);
	}

	void drawable::on_render(graphics::renderer* render) 
	{		
		if (m_piece == nullptr)
		{
			return;
		}

		m_piece->begin();

		const auto shader = m_piece->getShader();
		const auto transform = get_owner()->get_transform();
		auto& world_mat = transform->get_world_matrix();
		world_mat = transform->get_matrix_transformation();
		shader->set_mat4("transform.world", world_mat);
		shader->set_vec4("drawable.color", m_color);
		
		m_piece->end(render);
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