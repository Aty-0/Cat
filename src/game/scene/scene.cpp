#include "scene.h"

namespace cat::game::scene
{
	scene::scene() :
		m_name("Untitled"),
		m_storage()
	{

	}

	scene::scene(const std::string& name, const scene_go_storage& storage) : m_name(name), 
		m_storage(storage)
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
			if (it->second->getPrefix() != -1 || it->second->getType() != CAT_ENGINE_GAMEOBJECT_TYPE)
			{
				INFO("[Scene] [%s] Delete [%s] Name: %s UUID: %s",
					m_name.c_str(),
					core::utils::getClassNameStr(it->second.get()).c_str(),
					it->second.get()->getName().c_str(),
					it->second.get()->getUUID().getIDStr().c_str());

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

	inline scene_go_storage& scene::getStorage()
	{
		return m_storage;
	}

	inline std::string scene::getName() const
	{
		return m_name;
	}

}