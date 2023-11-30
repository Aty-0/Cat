#include "camera.h"

#include "core/engine.h"
#include "core/game_window.h"
#include "core/input_manager.h"
#include "core/utils/game_time.h"
#include "game/game_object.h"

namespace cat::game::components
{
	camera::camera() :
		m_zoom(DefaultZoom),
		m_speed(DefaultSpeed),
		m_projection(glm::mat4(1.0f)),
		m_view(glm::mat4(1.0f))
	{

	}

	camera::~camera()
	{

	}

	void camera::move_right()
	{
		static const auto transform = get_owner()->get_transform();
		static const auto time = core::utils::game_time::get_instance();

		transform->set_position(glm::vec3(transform->get_position().x + get_speed() *
			time->get_delta_time(), transform->get_position().y, transform->get_position().z));
	}

	void camera::move_left()
	{
		static const auto transform = get_owner()->get_transform();
		static const auto time = core::utils::game_time::get_instance();

		transform->set_position(glm::vec3(transform->get_position().x - get_speed() *
			time->get_delta_time(), transform->get_position().y, transform->get_position().z));
	}

	void camera::move_up()
	{
		static const auto transform = get_owner()->get_transform();
		static const auto time = core::utils::game_time::get_instance();

		transform->set_position(glm::vec3(transform->get_position().x, transform->get_position().y + get_speed() *
			time->get_delta_time(), transform->get_position().z));
	}

	void camera::move_down()
	{
		static const auto transform = get_owner()->get_transform();
		static const auto time = core::utils::game_time::get_instance();

		transform->set_position(glm::vec3(transform->get_position().x, transform->get_position().y - get_speed() *
			time->get_delta_time(), transform->get_position().z));
	}

	void camera::on_render(graphics::renderer* renderer)
	{
		static const auto window = core::game_window::get_instance();
		const auto transform = get_owner()->get_transform();

		auto aspect_ratio = window->get_width() / window->get_height();

		auto cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
		auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		m_view = glm::lookAt(transform->get_position(), transform->get_position() + cameraFront, cameraUp);
		m_projection = glm::mat4(1.0f);
		m_projection = glm::ortho(1.0f + m_zoom, -1.0f - m_zoom, 
			(-1.0f * aspect_ratio) - m_zoom, (1.0f * aspect_ratio) + m_zoom, -1000.0f, 1000.0f);
	}

	glm::mat4& camera::get_view()
	{
		return m_view;
	}

	glm::mat4& camera::get_projection()
	{
		return m_projection;
	}

	float camera::get_speed() const
	{
		return m_speed;
	}

	float camera::get_zoom() const
	{
		return m_zoom;
	}
}