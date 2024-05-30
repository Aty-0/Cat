#include "game_time.h"

namespace cat::core::utils
{
	game_time::game_time() :
		m_time(std::chrono::high_resolution_clock::now()),
		m_time_begin(std::chrono::high_resolution_clock::now()),
		m_fps(0),
		m_frame_count(0),
		m_deltaTime(0.0f)
	{

	}

	game_time::~game_time()
	{

	}

	void game_time::tick()
	{
		m_frame_count++;

		m_time = std::chrono::high_resolution_clock::now();
		m_deltaTime = 1.0e-6f * std::chrono::duration_cast<std::chrono::microseconds>(m_time - m_time_lastUpdate).count();
		
		if (m_time - m_time_begin >= std::chrono::seconds{ 1 })
		{
			m_fps = m_frame_count;
			m_time_begin = m_time;
			m_frame_count = 0;
		}

		m_time_lastUpdate = m_time;
	}

	const std::int32_t game_time::getFps() const
	{ 
		return m_fps;
	}

	const float game_time::getDeltaTime() const
	{ 
		return m_deltaTime;
	}

	const float game_time::getTime() const
	{ 
		return static_cast<float>(glfwGetTime());
	}
}