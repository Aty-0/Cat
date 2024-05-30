#pragma once
#include "scripts/script_common.h"

namespace cat::scripts
{
	struct CATENGINE_API script
	{
		sol::state lua;
	};

	class CATENGINE_API script_core : public core::utils::singleton<scripts::script_core>
	{
	public:
		script_core();
		~script_core();

		void clear();

		// run script by name 
		bool run(const char* name, bool safe = true);
		
		bool runFunc(const char* name, const char* func_name);

		[[nodiscard]] inline script* getScript(const char* name) const;

	private:
		std::vector<std::pair<std::string, script*>> m_scripts;

		void addCatAPI(sol::state& lua);
	};
}