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
		m_view(glm::mat4(1.0f)),
		m_front(glm::vec3(0.0f, 0.0f, 1.0f)),
		m_right(glm::vec3(0.0f, 0.0f, 0.0f))
	{
		m_input = core::input_manager::get_instance();

		m_input->add_listener_sp(core::input_key_state::Move, core::input_device::Mouse, std::bind(&camera::track_mouse_move, this));
		m_input->add_listener(core::input_key_code::KEYBOARD_W, core::input_key_state::Hold, core::input_device::Keyboard, std::bind(&game::components::camera::move_up, this));
		m_input->add_listener(core::input_key_code::KEYBOARD_S, core::input_key_state::Hold, core::input_device::Keyboard, std::bind(&game::components::camera::move_down, this));
		m_input->add_listener(core::input_key_code::KEYBOARD_A, core::input_key_state::Hold, core::input_device::Keyboard, std::bind(&game::components::camera::move_left, this));
		m_input->add_listener(core::input_key_code::KEYBOARD_D, core::input_key_state::Hold, core::input_device::Keyboard, std::bind(&game::components::camera::move_right, this));
		m_input->add_listener(core::input_key_code::KEYBOARD_LEFT_SHIFT, core::input_key_state::Hold,
			core::input_device::Keyboard, std::bind(&game::components::camera::enable_mouse_move, this));
		//im->add_listener(core::input_key_code::KEYBOARD_Z, core::input_key_state::Release,
		//	core::input_device::Keyboard, std::bind(&game::components::camera::disable_mouse_move, this));


	}

	camera::~camera()
	{

	}

	void camera::disable_mouse_move()
	{
		m_track_mouse = false;
		m_input->unhideCursor();
	}

	void camera::enable_mouse_move()
	{
		m_track_mouse = true;
		m_input->hideCursor();
	}

	void camera::track_mouse_move()
	{
		if (!m_track_mouse)
			return;

		auto xpos = m_input->get_mouse_pos().x;
		auto ypos = m_input->get_mouse_pos().y;
		
		static float last_x = xpos;
		static float last_y = ypos;

		const auto transform = get_owner()->get_transform();
		auto xoffset = xpos - last_x;
		auto yoffset = last_y - ypos; // reversed since y-coordinates go from bottom to top

		last_x = xpos;
		last_y = ypos;
		
		// TODO: 
		const float mouse_sensitivity = 0.1f;

		xoffset *= mouse_sensitivity;
		yoffset *= mouse_sensitivity;

		auto r = transform->get_rotation();
		
		// when pitch is out of bounds, screen doesn't get flipped
		if (r.y > 89.0f)
			r.y = 89.0f;
		if (r.y < -89.0f)
			r.y = -89.0f;

		transform->set_rotation({ r.x + xoffset, r.y + yoffset, r.z });
	}

	void camera::move_right()
	{
		static const auto transform = get_owner()->get_transform();
		static const auto time = core::utils::game_time::get_instance();
		auto pos = transform->get_position();
		pos += m_right * get_speed() * time->get_delta_time();
		transform->set_position(pos);

		//transform->set_position(glm::vec3(transform->get_position().x + get_speed() *
		//	time->get_delta_time(), transform->get_position().y, transform->get_position().z));
	}

	void camera::move_left()
	{
		static const auto transform = get_owner()->get_transform();
		static const auto time = core::utils::game_time::get_instance();
		auto pos = transform->get_position();
		pos -= m_right * get_speed() * time->get_delta_time();
		transform->set_position(pos);

		//transform->set_position(glm::vec3(transform->get_position().x - get_speed() *
		// 	time->get_delta_time(), transform->get_position().y, transform->get_position().z));
	}

	void camera::move_up()
	{
		static const auto transform = get_owner()->get_transform();
		static const auto time = core::utils::game_time::get_instance();
		auto pos = transform->get_position();
		pos += m_front * get_speed() * time->get_delta_time();
		transform->set_position(pos);

		//transform->set_position(glm::vec3(transform->get_position().x, transform->get_position().y, 
		//	transform->get_position().z + get_speed() *
		//	time->get_delta_time()));
	}

	void camera::move_down()
	{
		static const auto transform = get_owner()->get_transform();
		static const auto time = core::utils::game_time::get_instance();
		auto pos = transform->get_position();
		pos -= m_front * get_speed() * time->get_delta_time();
		transform->set_position(pos);

		//transform->set_position(glm::vec3(transform->get_position().x, transform->get_position().y, 
		//	transform->get_position().z - get_speed() *
		//	time->get_delta_time()));
	}

	void camera::on_render(graphics::renderer* renderer)
	{
		// FIXME: 
		if (m_input->get_key_state(core::input_key_code::KEYBOARD_LEFT_SHIFT, core::input_key_state::Release,
				core::input_device::Keyboard) && m_track_mouse)
		{
			disable_mouse_move();
		}

		static const auto window = core::game_window::get_instance();
		const auto transform = get_owner()->get_transform();

		const float aspect_ratio = window->get_width() / window->get_height();
		
		static auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		const auto WUp = glm::vec3(0.0f, 1.0f, 0.0f);


		m_view = glm::lookAt(transform->get_position(), transform->get_position() + m_front, cameraUp);
		// Orthographic mode
		//m_projection = glm::ortho(1.0f + m_zoom, -1.0f - m_zoom, 
		//	(-1.0f * aspect_ratio) - m_zoom, (1.0f * aspect_ratio) + m_zoom, -1000.0f, 1000.0f);
		auto r = transform->get_rotation();

		glm::vec3 front = { };

		front.x = cos(glm::radians(r.x)) * cos(glm::radians(r.y));
		front.y = sin(glm::radians(r.y));
		front.z = sin(glm::radians(r.x)) * cos(glm::radians(r.y));
		
		m_front = glm::normalize(front);
		m_right = glm::normalize(glm::cross(m_front, WUp));  
		cameraUp = glm::normalize(glm::cross(m_right, m_front));

		m_projection = glm::perspective(glm::radians(90.0f), aspect_ratio, 0.1f, 1000.0f);


	}

	glm::vec3 camera::get_front() const
	{
		return m_front;
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