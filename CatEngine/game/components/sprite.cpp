#include "sprite.h"
#include "game/game_object.h"
#include "core/game_window.h"

namespace cat::game::components
{
	sprite::sprite()
	{
		m_piece = new graphics::piece();
		setTexture("default_texture");
	}

	void sprite::setTexture(const char* texture_name)
	{
		m_piece->setTexture(0, texture_name);

		// TODO: Not work when we are change resolution
		rescale();
	}

	void sprite::rescale()
	{
		const auto texture = m_piece->getTexture(0);
		CAT_ASSERT(texture != nullptr);

		const auto owner = getOwner();
		if (owner != nullptr)
		{
			// Only for 2D
			const auto game_window = core::game_window::getInstance();
			const float ar = static_cast<float>(texture->getWidth() / texture->getHeight());
			//const float ar_gw = static_cast<float>(game_window->getWidth() / game_window->getHeight());
			//const float ar2 = static_cast<float>(texture->getHeight() / texture->getWidth());
			//owner->getTransform()->setScaleFactor({ ar_gw + ar, ar_gw + ar2, 1 });
			owner->getTransform()->setScaleFactor({ ar, ar, 1 });
		}
	}
}