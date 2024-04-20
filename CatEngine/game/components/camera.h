#pragma once
#include "core/common.h"
#include "game/components/component.h"

namespace cat::core
{
	class input_manager;
}

namespace cat::game::components
{
	class CATENGINE_API camera : public component
	{
	public:
		camera();
		virtual ~camera();

		[[nodiscard]] inline glm::mat4& getView();
		[[nodiscard]] inline glm::mat4& getProjection();
		[[nodiscard]] inline float getSpeed() const;
		[[nodiscard]] inline float getZoom() const;
		[[nodiscard]] inline glm::vec3 getFront() const;
					 
		void moveRight();
		void moveLeft();
		void moveUp();
		void moveDown();
		
		void onRender(graphics::renderer* renderer) override;
	private:
		void enableMouseControl();
		void disableMouseControl();
		void trackMouse();


		glm::mat4 m_view;
		glm::mat4 m_projection;
		glm::vec3 m_front;
		glm::vec3 m_right;

		bool	  m_track_mouse;
		float	  m_zoom;
		float	  m_speed;
		
		core::input_manager* m_input;
	public:
		static constexpr auto EngineCameraName = "_MainCamera";
		static constexpr auto DefaultZoom = 7.3f;
		static constexpr auto DefaultSpeed = 10.0f;
	};
}