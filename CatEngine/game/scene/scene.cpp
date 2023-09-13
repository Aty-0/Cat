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

	// FIX ME:  Reducing perfomance
	//			Need to make better algorithm, currently it is very bad way to do this.

	std::string scene::make_name_unique(std::string name)
	{
		auto newName = std::string();

		// How much objects we found with the same name
		auto count = 0;

		scene_go_storage::iterator it = { };

		while (true)
		{
			count != 0 ? newName = name + std::to_string(count) : newName = name;
			it = std::find_if(
				m_storage.begin(), m_storage.end(),
				[&newName](std::pair<std::pair<std::string, boost::uuids::uuid>,
					std::shared_ptr<game::game_object>> const& elem) {
						return elem.first.first == newName;
				});

			if (it == m_storage.end())
			{
				break;
			}

			count++;
		}

		return newName;
	}

	void scene::rename(std::string object_name, std::string new_object_name)
	{
		CAT_NO_IMPL();
	}

	void scene::rename(boost::uuids::uuid object_id, std::string new_object_name)
	{
		CAT_NO_IMPL();
	}

	void scene::del(std::string object_name)
	{
		scene_go_storage::iterator it = std::find_if(
			m_storage.begin(),
			m_storage.end(),
			[&object_name](std::pair<std::pair<std::string, boost::uuids::uuid>,
				std::shared_ptr<game::game_object>> const& elem) {
					return elem.first.first == object_name;
			});

		if (it == m_storage.end() || it->second->get_prefix() == -1 || it->second->get_type() == CAT_ENGINE_GAMEOBJECT_TYPE)
		{
			ERR("[Scene] [%s] Can't delete object Name: %s",
				m_name.c_str(),
				object_name.c_str());
			return;
		}

		INFO("[Scene] [%s] Delete [%s] Name: %s UUID: %s",
			m_name.c_str(),
			core::utils::get_class_name_string(it->second.get()).c_str(),
			it->second.get()->get_name().c_str(),
			it->second.get()->get_uuid_in_str().c_str());

		m_storage.erase(it);
	}

	void scene::del(boost::uuids::uuid object_id)
	{
		scene_go_storage::iterator it = std::find_if(
			m_storage.begin(),
			m_storage.end(),
			[&object_id](std::pair<std::pair<std::string, boost::uuids::uuid>,
				std::shared_ptr<game::game_object>> const& elem) {
					return elem.first.second == object_id;
			});

		if (it == m_storage.end() || it->second->get_prefix() == -1 || it->second->get_type() == CAT_ENGINE_GAMEOBJECT_TYPE)
		{
			ERR("[Scene] [%s] Can't delete object UUID: %s",
				m_name.c_str(),
				core::uuid::convert_uuid_to_str(object_id).c_str());
			return;
		}

		INFO("[Scene] [%s] Delete [%s] Name: %s UUID: %s",
			m_name.c_str(),
			core::utils::get_class_name_string(it->second.get()).c_str(),
			it->second.get()->get_name().c_str(),
			it->second.get()->get_uuid_in_str().c_str());

		m_storage.erase(it);
	}

	void scene::replace(std::string object_name, game::game_object* object)
	{
		CAT_NO_IMPL();
	}

	void scene::replace(boost::uuids::uuid object_id, game::game_object* object)
	{
		CAT_NO_IMPL();
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
					it->second.get()->get_uuid_in_str().c_str());

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