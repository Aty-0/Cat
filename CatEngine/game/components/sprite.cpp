#include "sprite.h"
#include "game/game_object.h"
#include "core/game_window.h"

namespace cat::game::components
{
	sprite::sprite()
	{
		m_piece = new graphics::piece();
		set_texture("default_texture");
	}

	void sprite::set_texture(const char* texture_name)
	{
		m_piece->setTexture(0, texture_name);

		// TODO: Not work when we are change resolution
		rescale();
	}

	void sprite::rescale()
	{
		const auto texture = m_piece->getTexture(0);
		CAT_ASSERT(texture != nullptr);

		const auto owner = get_owner();
		if (owner != nullptr)
		{
			// Only for 2D
			const auto game_window = core::game_window::get_instance();
			const float ar = static_cast<float>(texture->get_width() / texture->get_height());
			//const float ar_gw = static_cast<float>(game_window->get_width() / game_window->get_height());
			//const float ar2 = static_cast<float>(texture->get_height() / texture->get_width());
			//owner->get_transform()->set_scale_factor({ ar_gw + ar, ar_gw + ar2, 1 });
			owner->get_transform()->set_scale_factor({ ar, ar, 1 });
		}
	}
}