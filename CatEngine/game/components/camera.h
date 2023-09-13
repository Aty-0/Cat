#pragma once
#include "core/common.h"
#include "game/components/component.h"

namespace cat::game::components
{
	class CATENGINE_API camera : public component
	{
	public:
		camera();
		virtual ~camera();

		[[nodiscard]] inline glm::mat4& get_view();
		[[nodiscard]] inline glm::mat4& get_projection();
		[[nodiscard]] inline float get_speed() const;
		[[nodiscard]] inline float get_zoom() const;
					 
		void move_right();
		void move_left();
		void move_up();
		void move_down();

		void on_render(graphics::renderer* renderer) override;

	private:
		glm::mat4 m_view;
		glm::mat4 m_projection;
		float	  m_zoom;
		float	  m_speed;
	public:
		static constexpr auto EngineCameraName = "_MainCamera";
		static constexpr auto DefaultZoom = 7.3f;
		static constexpr auto DefaultSpeed = 10.0f;
	};
}