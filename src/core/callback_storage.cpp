#include "core/callback_storage.h"

namespace cat::core
{
	callback_storage::callback_storage()
		: m_storage()
	{

	}

	callback_storage::callback_storage(const default_callback& callback)
		: m_storage()
	{
		add(callback);
	}	

	void callback_storage::update(default_callback callback)
	{
		remove(callback);
		add(callback);
	}

	void callback_storage::add(default_callback callback)
	{
		m_storage.push_back(callback);
	}

	void callback_storage::remove(default_callback callback)
	{
		for (std::vector<default_callback>::iterator it = m_storage.begin(); it != m_storage.end();)
		{
			// TODO: Is it save ?
			if (it->target_type().name() == callback.target_type().name())
			{
				m_storage.erase(it);
				break;
			}

			++it;
		}
	}

	void callback_storage::clear()
	{
		m_storage.clear();
	}

	void callback_storage::runAll()
	{
		for (const auto& callback : m_storage)
		{
			if (callback != nullptr)
			{
				callback();
			}
		}
	}

	std::size_t callback_storage::size() const
	{
		return m_storage.size();
	}
}