#pragma once
#include "core/common.h"
#include "core/utils/key_code.h"

// TODO:
// Add support for joystick
// Add event support for scrolling

namespace cat::core
{	
	using input_key_code = core::utils::key_code;
	using input_function = std::function<void()>;
	
	// TODO: Rename to input_device_state
	enum class CATENGINE_API input_key_state
	{
		Press = GLFW_PRESS,
		Release = GLFW_RELEASE,
		Repeat = GLFW_REPEAT,
		Hold = 3,
		Move = 4,
		Scroll = 5,
		Unknown = -1
	};

	enum class CATENGINE_API input_device
	{
		Keyboard,
		Mouse,
		Joystick,
		Unknown = -1
	};

	class CATENGINE_API input_event
	{
	public:
		input_event() : code(input_key_code::KEYBOARD_UNKNOWN),
			state(input_key_state::Unknown),
			device(input_device::Unknown),
			func(nullptr)
		{

		}
		
		explicit input_event(input_key_code _code, input_key_state _keyState, input_device _device, input_function _func) :
			code(_code),
			state(_keyState), 
			device(_device), 
			func(_func)
		{

		}

		input_key_state			state;
		input_device			device;
		input_key_code 			code;
		input_function			func;
	};

	class CATENGINE_API input_manager : public cat::core::utils::singleton<input_manager>
	{
	public:
		input_manager();
		~input_manager();

		void init();
		void update();	

		void unsubscribe_listener(input_key_code code = input_key_code::KEYBOARD_UNKNOWN,
			input_key_state keyState = input_key_state::Unknown);

		// Add key listener 
		void add_listener(input_key_code code = input_key_code::KEYBOARD_UNKNOWN, 
						input_key_state keyState = input_key_state::Unknown, 
						input_device device = input_device::Unknown, input_function inputevent = nullptr);


		// Mostly needed for special behavior like scroll or move
		// TODO: input_device needed ? Have joystick special cases ?
		void add_listener_sp(input_key_state keyState = input_key_state::Unknown, input_device device = input_device::Unknown,
			input_function inputevent = nullptr);

		// Clear listener storage
		void clear_listeners();
		
		// If we need to get key state value
		inline bool get_key_state(input_key_code code, input_key_state keyState, 
								input_device device);
		
		// Get mouse current coordinates
		[[nodiscard]] inline glm::vec2 get_mouse_pos() const;
		[[nodiscard]] inline glm::vec2 get_scroll_pos() const;

		void hideCursor();
		void unhideCursor();

	private:
		void update_key(input_key_code code, input_key_state state);

		void parse_listeners(GLFWwindow* window, std::int32_t key_or_buttons, 
			std::int32_t scancode, std::int32_t action, std::int32_t mods);
		
		// sp - special case
		void parse_mouse_sp_listeners(input_key_state state);
		glm::vec2 m_last_mouse_coords;
		glm::vec2 m_mouse_coords;
		glm::vec2 m_scroll_coords;

		std::vector<std::unique_ptr<input_event>> m_listeners_storage;
		bool m_keyboard_key_up[GLFW_KEY_LAST + 1];
		bool m_keyboard_key_down[GLFW_KEY_LAST + 1];
		GLFWwindow* m_window;

		// GLFW Callbacks
		static void on_scroll(GLFWwindow* window, double xoffset, double yoffset);
		static void on_mouse_move(GLFWwindow* window, double xpos, double ypos);
		static void on_mouse_buttons_click(GLFWwindow* window, std::int32_t button, std::int32_t action, std::int32_t mods);
		static void on_keyboard_buttons_click(GLFWwindow* window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods);

	};
}
