#pragma once
#include "core/common.h"

#include "graphics/index_buffer.h"
#include "graphics/vertex_buffer.h"
#include "graphics/shader.h"
#include "graphics/texture.h"

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

		std::shared_ptr<graphics::shader> m_shader;
		std::shared_ptr<graphics::vertex_buffer> m_vertex_buffer;
		std::shared_ptr<graphics::index_buffer>  m_index_buffer;
		std::shared_ptr<graphics::texture>		 m_texture;
		glm::vec4 m_color;
	};
}