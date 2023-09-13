#pragma once
#include "game/scene/scene.h"

namespace cat::game::scene
{
	template<typename T>
	inline T* scene::add(T* g)
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value, "This is not GameObject or GameObject based class");

		// Check object on valid
		if (g == nullptr)
		{
			ERR("[Scene] [%s] Can't add this game object because it is nullptr!", m_name.c_str());
			return nullptr;
		}
		else if (g->get_name().empty() || g->get_uuid_in_str().empty())
		{
			ERR("[Scene] [%s] Can't add this game object because it is not initialized!", m_name.c_str());
			return nullptr;
		}

		// Check objects on the same name 
		g->set_name(make_name_unique(g->get_name()));
		
		VERB("[Scene] [%s] Added [%s] Name: %s UUID: %s at Position (%f %f %f)",
			m_name.c_str(), core::utils::get_class_name_string(g).c_str(),
			g->get_name().c_str(), g->get_uuid_in_str().c_str(),
			g->get_transform()->get_position().x, g->get_transform()->get_position().y, g->get_transform()->get_position().z);
		
		// Add object to storage
		const auto go_shared = std::make_shared<T>(*g);
		m_storage.push_back(std::move(std::make_pair(std::make_pair(g->get_name(), g->get_uuid()), go_shared)));
		
		return go_shared.get();
	}

	template<typename T>
	inline T* scene::clone(std::string object_name, std::string new_name)
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value, "This is not GameObject or GameObject based class");

		VERB("[Scene] [%s] Clone object Name: %s New name: %s", m_name.c_str(), object_name.c_str(),
			new_name.empty() ? "No" : new_name.c_str());

		 scene_go_storage::iterator it = std::find_if(m_storage.begin(), m_storage.end(),
		 	[&object_name](std::pair<std::pair<std::string, boost::uuids::uuid>, std::shared_ptr<game::game_object>> const& elem)
		 	{ return elem.first.first == object_name; });
		 
		 if (it == m_storage.end())
		 {
		 	ERR("[Scene] [%s] Can't clone object!", m_name.c_str());
		 	return nullptr;
		 }
		 
		 if (dynamic_cast<T*>(it->second.get()) != it->second.get())
		 {
		 	ERR("[Scene] [%s] Wrong type! Can't clone object", m_name.c_str());
		 	return nullptr;
		 }
		 
		 const auto new_obj = new T(*dynamic_cast<T*>(it->second.get()));
		 const auto name = new_name.empty() ? new_obj->get_name() + "_clone" : new_name;
		 new_obj->init(name, new_obj->get_type(), new_obj->get_prefix(), "REGENERATE");
		 return add<T>(new_obj);
	}

	template<typename T>
	inline T* scene::clone(boost::uuids::uuid object_id, std::string new_name)
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value, "This is not GameObject or GameObject based class");

		VERB("[Scene] [%s] Clone object Name: %s New name: %s", m_name.c_str(), core::uuid::convert_uuid_to_str(object_id).c_str(),
			new_name.empty() ? "No" : new_name.c_str());

		 scene_go_storage::iterator it = std::find_if(m_storage.begin(), m_storage.end(),
		 	[&object_id](std::pair<std::pair<std::string, boost::uuids::uuid>, std::shared_ptr<game::game_object>> const& elem)
		 	{ return elem.first.second == object_id; });
		 
		 if (it == m_storage.end())
		 {
		 	ERR("[Scene] [%s] Can't clone object!", m_name.c_str());
		 	return nullptr;
		 }
		 
		 if (dynamic_cast<T*>(it->second.get()) != it->second.get())
		 {
		 	ERR("[Scene] [%s] Wrong type! Can't clone object", m_name.c_str());
		 	return nullptr;
		 }
		 
		 const auto new_obj = new T(*dynamic_cast<T*>(it->second.get()));
		 const auto name = new_name.empty() ? new_obj->get_name() + "_clone" : new_name;
		 new_obj->init(name, new_obj->get_type(), new_obj->get_prefix(), "REGENERATE");
		 
		 return add<T>(new_obj);
	}

}