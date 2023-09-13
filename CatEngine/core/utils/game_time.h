#pragma once
#include "core/common.h"

#include <chrono>
#include <time.h>
#include <sys/timeb.h>

namespace cat::core::utils
{
	class CATENGINE_API game_time : public cat::core::utils::singleton<game_time>
	{
	public:
		game_time();
		~game_time();

		void tick();

		[[nodiscard]] inline const std::int32_t get_fps() const;
		[[nodiscard]] inline const float get_delta_time() const;
		[[nodiscard]] inline const float get_time() const;
											  
	private:								  
		std::int32_t m_fps;
		std::int32_t m_frame_count;
		float m_deltaTime;
		std::chrono::steady_clock::time_point m_time_lastUpdate;
		std::chrono::steady_clock::time_point m_time;
		std::chrono::steady_clock::time_point m_time_begin;
	};
}