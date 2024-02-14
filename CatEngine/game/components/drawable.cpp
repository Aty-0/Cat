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
		m_piace = new graphics::piece();

		set_texture("default_texture");		
	}

	drawable::~drawable()
	{
		core::utils::safe_delete(m_piace);
	}

	void drawable::rescale()
	{
		//const auto owner = get_owner();
		//if (owner != nullptr)
		//{
		//	const auto game_window = core::game_window::get_instance();
		//	const float ar_gw = static_cast<float>(game_window->get_width() / game_window->get_height());
		//	const float ar = static_cast<float>(m_texture->get_width() / m_texture->get_height());
		//	const float ar2 = static_cast<float>(m_texture->get_height() / m_texture->get_width());
		//	owner->get_transform()->set_scale_factor({ ar_gw + ar, ar_gw + ar2, 1 });
		//}
	}

	void drawable::set_texture(const char* texture_name)
	{
		//CAT_ASSERT(m_texture->load(texture_name));
	}

	void drawable::on_render(graphics::renderer* render) 
	{
		// TODO: Do this when we are change res for window and change texture 
		rescale();
		
		m_piace->begin();

		const auto shader = m_piace->getShader();
		const auto transform = get_owner()->get_transform();
		auto& world_mat = transform->get_world_matrix();
		world_mat = transform->get_matrix_transformation();
		shader->set_mat4("transform.world", world_mat);
		shader->set_vec4("drawable.color", m_color);
		
		m_piace->end(render);
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