#pragma once
#include "core/common.h"
#include "graphics/piece.h"

#include "game/components/component.h"
namespace cat::graphics
{
	class renderer;
}

namespace cat::game::components
{
	class CATENGINE_API drawable : public component
	{
	public:
		drawable();
		virtual ~drawable();

		[[nodiscard]] inline glm::vec4 getColor() const;
		void onRender(graphics::renderer* render) override;

		void setColor(glm::vec4 color);
	protected:
		graphics::piece* m_piece;
		glm::vec4 m_color;
	};
}