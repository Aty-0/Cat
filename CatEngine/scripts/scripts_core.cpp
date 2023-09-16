#include "scripts_core.h"
#include "io/resource_manager.h"

#pragma warning(push)
#pragma warning(disable : 26812)
#pragma warning(disable : 26495)
#define BOOST_PYTHON_STATIC_LIB 
#include <boost/python.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>
#include <Python.h>
#pragma warning(pop)

// Core
#include "core/utils/logger.h"
#include "core/utils/game_time.h"
#include "core/game_window.h"
#include "core/input_manager.h"
// Game
#include "game/game_object.h"

#include "game/components/component.h"
#include "game/components/transform.h"
#include "game/components/camera.h"
#include "game/components/drawable.h"

#include "game/scene/scene.h"
#include "game/scene/scene_manager.h"

#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

#include <Python.h>



namespace cat::scripts
{
	inline void log_info(std::string msg)
	{
		INFO("Py: %s", msg.c_str());
	}

	inline void log_verb(std::string msg)
	{
		VERB("Py: %s", msg.c_str());
	}

	inline void log_warn(std::string msg)
	{
		WARN("Py: %s", msg.c_str());
	}

	inline void log_err(std::string msg)
	{
		ERR("Py: %s", msg.c_str());
	}

	inline void log_fatal(std::string msg)
	{
		FATAL("Py: %s", msg.c_str());
	}

	inline void default_callback_wrap(const boost::python::object& f)
	{
		return boost::python::call<void>(f.ptr());
	}

	inline boost::shared_ptr<glm::vec4> make_vec4(float x, float y, float z, float w)
	{
		return boost::make_shared<glm::vec4>(x, y, z, w);
	}

	inline boost::shared_ptr<glm::vec3> make_vec3(float x, float y, float z)
	{
		return boost::make_shared<glm::vec3>(x, y, z);
	}

	inline boost::shared_ptr<glm::vec2> make_vec2(float x, float y)
	{
		return boost::make_shared<glm::vec2>(x, y);
	}

	inline boost::shared_ptr<glm::vec1> make_vec1(float x)
	{
		return boost::make_shared<glm::vec1>(x);
	}

	struct vec1_convert_list
	{
		static PyObject* convert(const glm::vec1& value)
		{
			boost::python::list result;
			result.append(value.x);
			return boost::python::incref(result.ptr());
		}
	};

	struct vec2_convert_list 
	{
		static PyObject* convert(const glm::vec2& value)
		{
			boost::python::list result;			
			result.append(value.x);		
			result.append(value.y);		
			return boost::python::incref(result.ptr());
		}
	};

	struct vec3_convert_list 
	{
		static PyObject* convert(const glm::vec3& value)
		{
			boost::python::list result;
			result.append(value.x);
			result.append(value.y);
			result.append(value.z);
			return boost::python::incref(result.ptr());
		}
	};

	struct vec4_convert_list
	{
		static PyObject* convert(const glm::vec4& value)
		{
			boost::python::list result;
			result.append(value.x);
			result.append(value.y);
			result.append(value.z);
			result.append(value.w);
			return boost::python::incref(result.ptr());
		}
	};

	// void print_varnames(boost::python::object fn)
	// {
	// 	namespace python = boost::python;
	// 	typedef python::stl_input_iterator<std::string> iterator;
	// 
	// 	std::vector<std::string> var_names(
	// 		iterator(fn.attr("func_code").attr("co_varnames")),
	// 		iterator());
	// 
	// 	BOOST_FOREACH(const std::string & varname, var_names)
	// 		INFO("DEBUG PY: var name: %s", varname.c_str());
	// }
	inline boost::python::enum_<core::input_key_code> export_keys()
	{
		boost::python::enum_<core::input_key_code> keys = boost::python::enum_<core::input_key_code>("input_key_code");

		for (auto key : core::utils::MAP_OF_KEYS)
		{
			keys.value(key.first.c_str(), key.second);
		}

		return keys;
	}

	template<typename T>
	inline boost::python::class_<T, boost::noncopyable> create_singleton_class(const char* name)
	{
		return boost::python::class_<T, boost::noncopyable>(name,
			boost::python::no_init).
			def("get_instance", &T::get_instance,
				boost::python::return_value_policy<boost::python::reference_existing_object>()).
			staticmethod("get_instance");
	}

	// Add template defs for specific type
	template<typename T>	
	inline void add_template_defs(boost::python::class_<game::scene::scene_manager, boost::noncopyable>& inst)
	{		
		inst.def("create_game_object", &game::scene::scene_manager::create_game_object<T>,
			boost::python::return_value_policy<boost::python::reference_existing_object>());
		inst.def("create_game_object_ninit", &game::scene::scene_manager::create_game_object_ninit<T>,
			boost::python::return_value_policy<boost::python::reference_existing_object>());
		inst.def("get_game_object_name", &game::scene::scene_manager::get_game_object_name<T>,
			boost::python::return_value_policy<boost::python::reference_existing_object>());
	}

	inline boost::python::class_<game::scene::scene_manager, boost::noncopyable> add_scene_manager()
	{
		auto sm_class = create_singleton_class<game::scene::scene_manager>("scene_manager");	
		add_template_defs<game::game_object>(sm_class);

		return sm_class;
	}

	// TODO: copy_const_reference
	template<typename T>
	inline void add_template_defs(boost::python::class_<game::game_object, boost::noncopyable>& inst)
	{
		const auto name_of_class = core::utils::get_class_name_string<T>();

		inst.def(("add_component_" + name_of_class).c_str(), &game::game_object::add_component<T>,
			boost::python::return_value_policy<boost::python::reference_existing_object>());

		inst.def(("delete_componenet_" + name_of_class).c_str(), &game::game_object::delete_component<T>);

		inst.def(("init_component_" + name_of_class).c_str(), &game::game_object::init_component<T>,
			boost::python::return_value_policy<boost::python::reference_existing_object>());

		inst.def(("create_component_" + name_of_class).c_str(), &game::game_object::create_component<T>,
			boost::python::return_value_policy<boost::python::reference_existing_object>());

		inst.def(("get_component_" + name_of_class).c_str(), &game::game_object::get_component<T>,
			boost::python::return_value_policy<boost::python::reference_existing_object>());

		inst.def(("get_component_in_children_" + name_of_class).c_str(), &game::game_object::get_component<T>,
			boost::python::return_value_policy<boost::python::reference_existing_object>());

		inst.def(("get_component_in_parent_" + name_of_class).c_str(), &game::game_object::get_component<T>,
			boost::python::return_value_policy<boost::python::reference_existing_object>());
	}

	inline boost::python::class_<game::game_object, boost::noncopyable> add_game_object_class()
	{
		auto gm_class = boost::python::class_<game::game_object, boost::noncopyable>("game_object",
			boost::python::no_init).
			add_property("name", &game::game_object::get_name).
			add_property("get_prefix", &game::game_object::get_prefix).
			add_property("get_type", &game::game_object::get_type).
			add_property("is_visible", &game::game_object::is_visible).
			add_property("is_enabled", &game::game_object::is_enabled).
			def("set_name", &game::game_object::set_name).
			def("set_prefix", &game::game_object::set_prefix).
			def("set_type", &game::game_object::set_type).
			def("set_visible", &game::game_object::set_visible).
			def("set_enabled", &game::game_object::set_enabled).
			def("get_transform", &game::game_object::get_transform,
				boost::python::return_value_policy<boost::python::reference_existing_object>());

		// Add all existing components here:
		add_template_defs<game::components::component>(gm_class);
		add_template_defs<game::components::camera>(gm_class);
		add_template_defs<game::components::transform>(gm_class);
		add_template_defs<game::components::drawable>(gm_class);



		return gm_class;
	}

	// TODO: Make function helper where we are already have singleton instance 
	BOOST_PYTHON_MODULE(CAT_API)
	{
		// boost::python::def("print_varnames", &print_varnames);

		// GLM Math defs
		boost::python::to_python_converter<glm::vec4, vec4_convert_list>();
		boost::python::class_<glm::vec4, boost::shared_ptr<glm::vec4>, boost::noncopyable>("vec4", boost::python::no_init).
			 def("__init__", boost::python::make_constructor(&make_vec4)).
			 def_readwrite("x", &glm::vec4::x).
			 def_readwrite("y", &glm::vec4::y).
			 def_readwrite("z", &glm::vec4::z).
			 def_readwrite("w", &glm::vec4::w);

		boost::python::to_python_converter<glm::vec3, vec3_convert_list>();
		boost::python::class_<glm::vec3, boost::shared_ptr<glm::vec3>, boost::noncopyable>("vec3", boost::python::no_init).
			def("__init__", boost::python::make_constructor(&make_vec3)).
			def_readwrite("x", &glm::vec3::x).
			def_readwrite("y", &glm::vec3::y).
			def_readwrite("z", &glm::vec3::z);

		boost::python::to_python_converter<glm::vec2, vec2_convert_list>();
		boost::python::class_<glm::vec2, boost::shared_ptr<glm::vec2>, boost::noncopyable>("vec2", boost::python::no_init).
			def("__init__", boost::python::make_constructor(&make_vec2)).
			def_readwrite("x", &glm::vec2::x).
			def_readwrite("y", &glm::vec2::y);

		boost::python::to_python_converter<glm::vec1, vec1_convert_list>();
		boost::python::class_<glm::vec1, boost::shared_ptr<glm::vec1>, boost::noncopyable>("vec1", boost::python::no_init).
			def("__init__", boost::python::make_constructor(&make_vec1)).
			def_readwrite("x", &glm::vec1::x);

		// TODO: Matrix 

		// Logger def's
		boost::python::def("info", &log_info);
		boost::python::def("verb", &log_verb);
		boost::python::def("err", &log_err);
		boost::python::def("warn", &log_err);
		boost::python::def("fatal", &log_fatal);
		///////////////////////////////////////////////
		// Core:
		///////////////////////////////////////////////
		boost::python::def("default_callback_wrap", default_callback_wrap);
		// TODO: If sometime implement change title func in game window class,
		//		 we need to add it to script api 
		create_singleton_class<core::game_window>("game_window").
			add_property("width", &core::game_window::get_width).
			add_property("height", &core::game_window::get_height).
			add_property("left", &core::game_window::get_left).
			add_property("top", &core::game_window::get_top).
			def("set_width", &core::game_window::set_width).
			def("set_height", &core::game_window::set_height).
			def("set_left", &core::game_window::set_left).
			def("set_top", &core::game_window::set_top);

		// Export keys for input manager
		export_keys();
		

		boost::python::enum_<core::input_key_state>("input_key_state").
			value("Press", core::input_key_state::Press).
			value("Release", core::input_key_state::Release).
			value("Hold", core::input_key_state::Hold).
			value("Repeat", core::input_key_state::Repeat).
			value("Unknown", core::input_key_state::Unknown);
		
		boost::python::enum_<core::input_device>("input_device").
			value("Keyboard", core::input_device::Keyboard).
			value("Mouse", core::input_device::Mouse).
			value("Joystick", core::input_device::Joystick).
			value("Unknown", core::input_device::Unknown);

		create_singleton_class<core::input_manager>("input_manager").
			def("add_listener",+ 
			[](core::input_manager& self, core::input_key_code code,
				core::input_key_state keyState,
				core::input_device device, boost::python::object object) {
			self.add_listener(code, keyState, device, object);
			}).
			def("clear_listeners", &core::input_manager::clear_listeners). 
			add_property("mouse_pos", &core::input_manager::get_mouse_pos);

		create_singleton_class<core::utils::game_time>("game_time").
			add_property("delta_time",&core::utils::game_time::get_delta_time).
			add_property("fps", &core::utils::game_time::get_fps).
			add_property("time", &core::utils::game_time::get_time);
		///////////////////////////////////////////////
		// Game:
		///////////////////////////////////////////////

		add_game_object_class();

		{
			boost::python::scope scope_component = boost::python::class_<game::components::component, boost::noncopyable>("component", boost::python::init<>()).
				add_property("name", &game::components::component::get_name).
				def("get_owner", &game::components::component::get_owner,
					boost::python::return_value_policy<boost::python::reference_existing_object>());
		}

		boost::python::class_<game::components::drawable, boost::noncopyable>("drawable", boost::python::init<>()).
			add_property("name", &game::components::component::get_name).
			def("get_owner", &game::components::component::get_owner,
				boost::python::return_value_policy<boost::python::reference_existing_object>());
		// TODO: Scale factor
		//		 Matrix 
		boost::python::class_<game::components::transform, boost::noncopyable>("transform", boost::python::init<>()).
			add_property("name", &game::components::component::get_name).
			add_property("position", &game::components::transform::get_position).
			add_property("rotation", &game::components::transform::get_rotation).
			add_property("scale", &game::components::transform::get_scale).
			add_property("scale_factor", &game::components::transform::get_scale_factor).
			def("set_position", &game::components::transform::set_position).
			def("set_rotation", &game::components::transform::set_rotation).
			def("set_scale", &game::components::transform::set_scale).
			def("set_parent", &game::components::transform::set_parent).
			def("get_child", &game::components::transform::get_child,
				boost::python::return_value_policy<boost::python::reference_existing_object>()).
			def("get_parent", &game::components::transform::get_parent,
				boost::python::return_value_policy<boost::python::reference_existing_object>()).
			def("is_child_of", &game::components::transform::is_child_of).			
			def("get_owner", &game::components::component::get_owner,
				boost::python::return_value_policy<boost::python::reference_existing_object>());

		// Scene
		add_scene_manager();


	}

	typedef struct script { };

	script_core::script_core()
	{

	}

	script_core::~script_core()
	{

	}

	// https://stackoverflow.com/questions/1418015/how-to-get-python-exception-text
	std::string script_core::handle_pyerror()
	{
		using namespace boost::python;
		using namespace boost;

		PyObject* exc, * val, * tb;
		object formatted_list, formatted;
		PyErr_Fetch(&exc, &val, &tb);
		handle<> hexc(exc), hval(allow_null(val)), htb(allow_null(tb));
		object traceback(import("traceback"));
		if (!tb) 
		{
			object format_exception_only(traceback.attr("format_exception_only"));
			formatted_list = format_exception_only(hexc, hval);
		}
		else 
		{
			object format_exception(traceback.attr("format_exception"));
			formatted_list = format_exception(hexc, hval, htb);
		}
		formatted = str("\n").join(formatted_list);
		return extract<std::string>(formatted);
	}

	// Define script resource type for resource manager
	struct script { };

	bool script_core::run(const char* name)
	{
		// Import your module to embedded Python
		if (PyImport_AppendInittab("CAT_API", 		
			PyInit_CAT_API) == -1)
		{
			ERR("Failed to add CAT_API to the interpreter's "
				"builtin modules");

			return false;
		}

		// Initialise Python
		Py_Initialize();

		static const auto rm = io::resource_manager::get_instance();
		const char* data = rm->read<const char*, script>(name, { "py" });
		
		if (data && !data[0])
		{
			ERR("file is not loaded or it's is empty!");
			return false;
		}

		try
		{
			boost::python::exec(data);
		}
		catch(boost::python::error_already_set)
		{
			boost::python::handle_exception();
			PyObject* ptype;
			PyObject* pvalue;
			PyObject* ptraceback;
			PyErr_Fetch(&ptype, &pvalue, &ptraceback);

			if (pvalue != nullptr)
			{
				const char* err_msg = boost::python::extract<const char*>(pvalue);
				ERR("Script exec failed: %s", err_msg);
			}
			else
			{
				ERR("Script exec failed: No details...");
			}

			return false;
		}

		// Nice, kekw	
		// From boost docs:
		// Boost.Python doesn't support Py_Finalize yet, so don't call it!
		//Py_Finalize();

		return true;
	}
}