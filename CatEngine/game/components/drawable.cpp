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
		m_piece = new graphics::piece();

		set_texture("default_texture");		
	}

	drawable::~drawable()
	{
		core::utils::safe_delete(m_piece);
	}

	void drawable::rescale()
	{
		const auto texture = m_piece->getTexture(0);
		CAT_ASSERT(texture != nullptr);

		const auto owner = get_owner();
		if (owner != nullptr)
		{
			const auto game_window = core::game_window::get_instance();
			const float ar_gw = static_cast<float>(game_window->get_width() / game_window->get_height());
			const float ar = static_cast<float>(texture->get_width() / texture->get_height());
			const float ar2 = static_cast<float>(texture->get_height() / texture->get_width());
			owner->get_transform()->set_scale_factor({ ar_gw + ar, ar_gw + ar2, 1 });
		}
	}

	void drawable::set_texture(const char* texture_name)
	{	
		m_piece->setTexture(0, texture_name);
	}

	void drawable::on_render(graphics::renderer* render) 
	{
		// TODO: Do this when we are change res for window and change texture 
		rescale();
		
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