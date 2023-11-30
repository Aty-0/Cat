#include "scene.h"

namespace cat::game::scene
{
	scene::scene() :
		m_name("Untitled"),
		m_storage()
	{

	}

	scene::~scene()
	{

	}
	
	void scene::clear()
	{
		INFO("[Scene] [%s] Try to clear scene...", m_name.c_str());

		for (scene_go_storage::iterator it = m_storage.begin(); it != m_storage.end();)
		{
			if (it->second->get_prefix() != -1 || it->second->get_type() != CAT_ENGINE_GAMEOBJECT_TYPE)
			{
				INFO("[Scene] [%s] Delete [%s] Name: %s UUID: %s",
					m_name.c_str(),
					core::utils::get_class_name_string(it->second.get()).c_str(),
					it->second.get()->get_name().c_str(),
					it->second.get()->get_id_str().c_str());

				it = m_storage.erase(it);
			}
			else
				it++;
		}

		m_storage.shrink_to_fit();
	}

	void scene::destroy()
	{
		INFO("[Scene] [%s] Destroy ", m_name.c_str());
		m_storage.erase(m_storage.begin(), m_storage.end());
		m_storage.shrink_to_fit();
	}

	inline scene_go_storage& scene::get_storage()
	{
		return m_storage;
	}

	inline std::string scene::get_name() const
	{
		return m_name;
	}

}