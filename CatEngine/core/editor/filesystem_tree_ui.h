#pragma once
#include "core/common.h"

namespace cat::core::editor
{
	class CATENGINE_API filesystem_tree_ui : public core::utils::singleton<filesystem_tree_ui>
	{
	public:
		filesystem_tree_ui() = default;
		~filesystem_tree_ui() { }

		void render();

	};
}

