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
		~drawable();

		[[nodiscard]] inline glm::vec4 get_color() const;
		void on_render(graphics::renderer* render) override;

		void set_color(glm::vec4 color);
		void set_texture(const char* texture_name);
	protected:
		void rescale();

		graphics::piece* m_piace;		
		glm::vec4 m_color;
	};
}