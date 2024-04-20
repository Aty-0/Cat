#include "logger.h"
#include "core/engine.h"

#include <filesystem>

#pragma warning(disable:4996)

#ifdef _WIN64
	#include <windows.h>
#endif

#define VA_LIST_OUTPUT(buffer) 	va_list args; \
		va_start(args, text); \
		std::vsnprintf(buffer, sizeof(buffer), text, args); \
		va_end(args);	\

#include "graphics/renderer.h"

namespace cat::core::utils
{
	// fix me:
	static ImGuiTextBuffer console_text_buffer;

	logger::logger() : m_linecount(0), m_log_file(), m_level()
	{
		m_level = logger::log_level::VERB;
		
		createLogFile();
	}

	logger::~logger()
	{
		m_log_file.close();
	}

	std::string logger::getTimeStr(bool useColonForTime) const
	{
		const auto _time = time(0);
		const auto localTime = localtime(&_time);
		const auto fmt = useColonForTime == true ? "%i.%i.%i %i:%i:%i" : "%i.%i.%i %i.%i.%i";
		const auto str = toStr(fmt, (1900 + localTime->tm_year), (1 + localTime->tm_mon), localTime->tm_mday,
			localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

		return str;
	}

	void logger::createLogFile()
	{
		// Create folder "Logs" if it's not exist
		if (!std::filesystem::is_directory("Logs") || !std::filesystem::exists("Logs"))
		{
			if (!std::filesystem::create_directory("Logs"))
			{
				throw std::runtime_error("Can't create directory");
			}
		}

		const auto time = getTimeStr(false);
		const auto path = toStr("Logs/CatLog-%s%s", time.c_str(), ".log");
		
		m_log_file_path = path;
		m_log_file = std::ofstream(m_log_file_path, std::ios_base::app);

		if (!m_log_file)
		{
			throw std::runtime_error("Can't open a log file...");
		}

		m_log_file << "Cat Engine Log file\n\n";
		m_log_file.close();
	}

	std::string logger::getLevelStr(logger::log_level level) const
	{
		const char* levelList[] = {
			"Unknown",
			"Verb",
			"Info",
			"Warning",
			"Error",
			"Fatal"
		};
		
		return levelList[static_cast<std::uint8_t>(level)];
	}

	void logger::print(logger::log_level level, const char* text, ...)
	{
		if (m_level > level)
			return;
		// Open log file 
		m_log_file = std::ofstream(m_log_file_path, std::ios_base::app);

		if (!m_log_file)
		{
			return;
		}

		char buffer[2048];
		*buffer = '\0';
		VA_LIST_OUTPUT(buffer);

		const auto time_str  = getTimeStr(true);
		const auto level_str = getLevelStr(level);
		const auto line = toStr("[%i] [%s] [%s]: %s\n", m_linecount, time_str.c_str(), level_str.c_str(), buffer);
	
		// Add current line to log file
		m_log_file << line;
		m_log_file.close();

		// Add line 
		m_linecount++;
#ifdef _WIN64
		// If we are have debuging output
		OutputDebugString(line.c_str());
#endif
		m_scroll_to_bottom = true;
		console_text_buffer.append(line.c_str());

		memset(buffer, 0, sizeof(buffer));

		if (level == log_level::FATAL)		
			throw std::runtime_error(std::string(buffer));
		
	}

	void logger::draw()
	{
		ImGui::Begin("Console");
		
		ImGui::TextUnformatted(console_text_buffer.begin());
		
		if (m_scroll_to_bottom)
			ImGui::SetScrollY(ImGui::GetScrollMaxY() + 10.0f);

		m_scroll_to_bottom = false;
		ImGui::End();
	}
}