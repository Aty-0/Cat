#include "input_manager.h"
#include "core/engine.h"
#include "core/game_window.h"
#include "core/utils/logger.h"
#include <chrono>

namespace cat::core
{
	input_manager::input_manager()
	{

	}

	input_manager::~input_manager()
	{
		clear_listeners();
		m_window = nullptr;
	}

	void input_manager::init()
	{
		const auto window = core::game_window::get_instance();

		m_window = window->get_GLFW_window();

		// Set callbacks
		glfwSetKeyCallback(m_window, on_keyboard_buttons_click);
		glfwSetMouseButtonCallback(m_window, on_mouse_buttons_click);		
		glfwSetCursorPosCallback(m_window, on_mouse_move);		
		glfwSetScrollCallback(m_window, on_scroll);
		
		glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
	}
	
	void input_manager::parse_mouse_sp_listeners(input_key_state state)
	{
		for (const auto& currentEvent : m_listeners_storage)
		{
			if (state == currentEvent->state)
			{
				if (currentEvent->func != nullptr)
					currentEvent->func();

				break;
			}
		}
	}

	void input_manager::on_scroll(GLFWwindow* window, double xoffset, double yoffset)
	{
		static const auto input_manager = input_manager::get_instance();
		input_manager->m_scroll_coords = { static_cast<float>(xoffset), static_cast<float>(xoffset) };
		input_manager->parse_mouse_sp_listeners(input_key_state::Scroll);
	}

	void input_manager::on_mouse_move(GLFWwindow* window, double xpos, double ypos)
	{			
		static const auto input_manager = input_manager::get_instance();
		input_manager->m_mouse_coords = { static_cast<float>(xpos), static_cast<float>(ypos) };
		input_manager->parse_mouse_sp_listeners(input_key_state::Move);
	}

	void input_manager::add_listener_sp(input_key_state keyState, input_device device, input_function inputevent)
	{
		if (device == input_device::Keyboard ||
			device == input_device::Unknown)
			return;

		m_listeners_storage.push_back(std::move(std::make_unique<input_event>(input_event(input_key_code::KEYBOARD_UNKNOWN, 
			keyState, device, inputevent))));
	}

	void input_manager::unsubscribe_listener(input_key_code code, input_key_state keyState)
	{
		VERB("input_manager::unsubscribe_listener Code:%d State:%d ", code, keyState);
		for (std::vector<std::unique_ptr<input_event>>::iterator currentEvent = m_listeners_storage.begin(); currentEvent != m_listeners_storage.end();)
		{
			if (code == currentEvent->get()->code && keyState == currentEvent->get()->state)
			{
				m_listeners_storage.erase(currentEvent);
				break;
			}

			currentEvent++;
		}
	}

	void input_manager::add_listener(input_key_code code, input_key_state keyState, input_device device, input_function inputevent)
	{
		VERB("Added event to input manager Code:%d State:%d Device:%d ", code, keyState, device);
		m_listeners_storage.push_back(std::move(std::make_unique<input_event>(input_event(code, keyState, device, inputevent))));
	}

	void input_manager::on_mouse_buttons_click(GLFWwindow* window, std::int32_t button, std::int32_t action, std::int32_t mods)
	{
		static const auto input_manager = input_manager::get_instance();
		input_manager->parse_listeners(window, button, 0, action, mods);
	}

	void input_manager::on_keyboard_buttons_click(GLFWwindow* window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods)
	{
		//VERB("is pressed [%s] state is: %i", glfwGetKeyName(key, scancode), action);
		static const auto input_manager = input_manager::get_instance();
		input_manager->parse_listeners(window, key, scancode, action, mods);
	}

	void input_manager::update_key(input_key_code code, input_key_state state)
	{
		switch (state)
		{
		case input_key_state::Press:
			m_keyboard_key_down[static_cast<std::int32_t>(code)] = true;
			break;
		case input_key_state::Release:
			m_keyboard_key_up[static_cast<std::int32_t>(code)] = true;
			break;
		}
	}

	void input_manager::update()
	{		
		for (auto& up : m_keyboard_key_up)
			up = false;

		for (auto& down : m_keyboard_key_down)
			down = false;
		
		// Parse events for Hold action because if we are parse it in callback
		// We will be have stuttering movements 
		// And there we are update keys for array of key statements 
		for (auto key : core::utils::LIST_OF_KEYS)
		{
			// Skip unknown buttons 
			if (key == input_key_code::KEYBOARD_UNKNOWN)
			{
				continue;
			}
			const auto key_int = static_cast<std::int32_t>(key);

			// Skip mouse buttons because we don't support it Hold action for it 
			// Because update_key using simple bool array which directed on keyboard 
			if (key_int <= GLFW_MOUSE_BUTTON_LAST)
			{
				continue;
			}

			const auto cur_state = static_cast<input_key_state>(glfwGetKey(m_window, key_int));
			// update current key
			update_key(key, cur_state);


			for (const auto& currentEvent : m_listeners_storage)
			{
				auto invoke = false;
				if (key == currentEvent->code)
				{
					if (currentEvent->state == input_key_state::Hold)
					{
						invoke = (m_keyboard_key_down[key_int] ==
							(true || (cur_state == input_key_state::Press)));
					}
					// Skip other key states
					else if (currentEvent->state == input_key_state::Press 
						|| currentEvent->state == input_key_state::Release)
					{
						continue;
					}

					// Invoke callback
					if (invoke)
					{
						if (currentEvent->state == input_key_state::Hold)
						{
							if (currentEvent->func != nullptr)
								currentEvent->func();
						}
						break;
					}
				}
			}
		}
	}

	void input_manager::clear_listeners()
	{
		m_listeners_storage.clear();
		m_listeners_storage.shrink_to_fit();
	}

	void input_manager::hideCursor()
	{		
		m_last_mouse_coords = m_mouse_coords;
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void input_manager::unhideCursor()
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPos(m_window, m_last_mouse_coords.x, m_last_mouse_coords.y);
	}

	void input_manager::parse_listeners(GLFWwindow* window, std::int32_t key_or_buttons, std::int32_t scancode, std::int32_t action, std::int32_t mods)
	{
		// Skip unknown buttons 
		if (key_or_buttons == static_cast<std::int32_t>(input_key_code::KEYBOARD_UNKNOWN))
			return;

		for (const auto& currentEvent : m_listeners_storage)
		{
			if (key_or_buttons == static_cast<std::int32_t>(currentEvent->code)
				&& action == static_cast<std::int32_t>(currentEvent->state))
			{
				if (currentEvent->func != nullptr)
					currentEvent->func();

				break;
			}
		}
	}

	bool input_manager::get_key_state(input_key_code code, input_key_state keyState, input_device device)
	{
		switch (device)
		{
		case input_device::Keyboard:
			return glfwGetKey(m_window, static_cast<std::int32_t>(code)) == static_cast<std::int32_t>(keyState);
		case input_device::Mouse:
			return glfwGetMouseButton(m_window, static_cast<std::int32_t>(code)) == static_cast<std::int32_t>(keyState);
			break;
		case input_device::Joystick:
			break;
		}

		return false;
	}

	// FIXME: This function won't work correctly if no "special" listeners in storage
	glm::vec2 input_manager::get_scroll_pos() const
	{
		return m_scroll_coords;
	}

	// FIXME: This function won't work correctly if no "special" listeners in storage
	glm::vec2 input_manager::get_mouse_pos() const
	{ 			
		// static const auto window = core::game_window::get_instance();
		// double x = 0.0;
		// double y = 0.0;
		// glfwGetCursorPos(window->get_GLFW_window(), &x, &y);
		// return glm::vec2(static_cast<float>(x), static_cast<float>(y));
		
		return m_mouse_coords;
	}
}