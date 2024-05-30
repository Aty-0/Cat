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
		m_input = core::input_manager::getInstance();

		m_input->addListener(core::input_key_state::Move, core::input_device::Mouse, std::bind(&camera::trackMouse, this));
		m_input->addListener(core::input_key_code::KEYBOARD_W, core::input_key_state::Hold, core::input_device::Keyboard, std::bind(&game::components::camera::moveUp, this));
		m_input->addListener(core::input_key_code::KEYBOARD_S, core::input_key_state::Hold, core::input_device::Keyboard, std::bind(&game::components::camera::moveDown, this));
		m_input->addListener(core::input_key_code::KEYBOARD_A, core::input_key_state::Hold, core::input_device::Keyboard, std::bind(&game::components::camera::moveLeft, this));
		m_input->addListener(core::input_key_code::KEYBOARD_D, core::input_key_state::Hold, core::input_device::Keyboard, std::bind(&game::components::camera::moveRight, this));
		m_input->addListener(core::input_key_code::KEYBOARD_LEFT_SHIFT, core::input_key_state::Hold,
			core::input_device::Keyboard, std::bind(&game::components::camera::enableMouseControl, this));
		//im->add_listener(core::input_key_code::KEYBOARD_Z, core::input_key_state::Release,
		//	core::input_device::Keyboard, std::bind(&game::components::camera::disable_mouse_move, this));


	}

	camera::~camera()
	{

	}

	void camera::disableMouseControl()
	{
		m_track_mouse = false;
		m_input->unhideCursor();
	}

	void camera::enableMouseControl()
	{
		m_track_mouse = true;
		m_input->hideCursor();
	}

	void camera::trackMouse()
	{
		if (!m_track_mouse)
			return;

		auto xpos = m_input->getMousePos().x;
		auto ypos = m_input->getMousePos().y;
		
		static float last_x = xpos;
		static float last_y = ypos;

		const auto transform = getOwner()->getTransform();
		auto xoffset = xpos - last_x;
		auto yoffset = last_y - ypos; // reversed since y-coordinates go from bottom to top

		last_x = xpos;
		last_y = ypos;
		
		// TODO: 
		const float mouse_sensitivity = 0.1f;

		xoffset *= mouse_sensitivity;
		yoffset *= mouse_sensitivity;

		auto r = transform->getRotation();
		
		// when pitch is out of bounds, screen doesn't get flipped
		if (r.y > 89.0f)
			r.y = 89.0f;
		if (r.y < -89.0f)
			r.y = -89.0f;

		transform->setRotation({ r.x + xoffset, r.y + yoffset, r.z });
	}

	void camera::moveRight()
	{
		static const auto transform = getOwner()->getTransform();
		static const auto time = core::utils::game_time::getInstance();
		auto pos = transform->getPosition();
		pos += m_right * getSpeed() * time->getDeltaTime();
		transform->setPosition(pos);

		//transform->setPosition(glm::vec3(transform->getPosition().x + getSpeed() *
		//	time->getDeltaTime(), transform->getPosition().y, transform->getPosition().z));
	}

	void camera::moveLeft()
	{
		static const auto transform = getOwner()->getTransform();
		static const auto time = core::utils::game_time::getInstance();
		auto pos = transform->getPosition();
		pos -= m_right * getSpeed() * time->getDeltaTime();
		transform->setPosition(pos);

		//transform->set_position(glm::vec3(transform->get_position().x - get_speed() *
		// 	time->get_delta_time(), transform->get_position().y, transform->get_position().z));
	}

	void camera::moveUp()
	{
		static const auto transform = getOwner()->getTransform();
		static const auto time = core::utils::game_time::getInstance();
		auto pos = transform->getPosition();
		pos += m_front * getSpeed() * time->getDeltaTime();
		transform->setPosition(pos);

		//transform->set_position(glm::vec3(transform->get_position().x, transform->get_position().y, 
		//	transform->get_position().z + get_speed() *
		//	time->get_delta_time()));
	}

	void camera::moveDown()
	{
		static const auto transform = getOwner()->getTransform();
		static const auto time = core::utils::game_time::getInstance();
		auto pos = transform->getPosition();
		pos -= m_front * getSpeed() * time->getDeltaTime();
		transform->setPosition(pos);

		//transform->set_position(glm::vec3(transform->get_position().x, transform->get_position().y, 
		//	transform->get_position().z - get_speed() *
		//	time->get_delta_time()));
	}

	void camera::onRender(graphics::renderer* renderer)
	{
		// FIXME: 
		if (m_input->getKeyState(core::input_key_code::KEYBOARD_LEFT_SHIFT, core::input_key_state::Release,
				core::input_device::Keyboard) && m_track_mouse)
		{
			disableMouseControl();
		}

		static const auto window = core::game_window::getInstance();
		const auto transform = getOwner()->getTransform();

		const auto aspect_ratio = static_cast<float>(window->getWidth() / window->getHeight());
		
		static auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		const auto WUp = glm::vec3(0.0f, 1.0f, 0.0f);

		m_view = glm::lookAt(transform->getPosition(), transform->getPosition() + m_front, cameraUp);

		// Orthographic mode
		//m_projection = glm::ortho(1.0f + m_zoom, -1.0f - m_zoom, 
		//	(-1.0f * aspect_ratio) - m_zoom, (1.0f * aspect_ratio) + m_zoom, -1000.0f, 1000.0f);
		auto r = transform->getRotation();

		glm::vec3 front = { };

		front.x = cos(glm::radians(r.x)) * cos(glm::radians(r.y));
		front.y = sin(glm::radians(r.y));
		front.z = sin(glm::radians(r.x)) * cos(glm::radians(r.y));
		
		m_front = glm::normalize(front);
		m_right = glm::normalize(glm::cross(m_front, WUp));  
		cameraUp = glm::normalize(glm::cross(m_right, m_front));

		m_projection = glm::perspective(glm::radians(90.0f), aspect_ratio, 0.1f, 1000.0f);


	}

	glm::vec3 camera::getFront() const
	{
		return m_front;
	}

	glm::mat4& camera::getView()
	{
		return m_view;
	}

	glm::mat4& camera::getProjection()
	{
		return m_projection;
	}

	float camera::getSpeed() const
	{
		return m_speed;
	}

	float camera::getZoom() const
	{
		return m_zoom;
	}
}