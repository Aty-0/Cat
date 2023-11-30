#include "logger.h"
#include "core/engine.h"

#include <filesystem>
#include <cstdarg> 

#pragma warning(disable:4996)

// TODO: macro ifdef windows

#include <windows.h>

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
		
		create_log_file();
	}

	logger::~logger()
	{
		m_log_file.close();
	}

	const char* logger::parse_args_to_string(const char* text, ...)
	{
		const auto BUFFER_SIZE = 2048;
		char buffer[BUFFER_SIZE];
		VA_LIST_OUTPUT(buffer);

		return buffer;
	}

	std::string logger::get_time(bool printMinAndSec)
	{
		const auto _time = time(0);
		const auto localTime = localtime(&_time);

		const char* textparsed = printMinAndSec == true ?
			parse_args_to_string("%i.%i.%i %i.%i.%i", (1900 + localTime->tm_year), (1 + localTime->tm_mon), localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec) :
			parse_args_to_string("%i.%i.%i", (1900 + localTime->tm_year), (1 + localTime->tm_mon), localTime->tm_mday);

		return textparsed;
	}

	void logger::create_log_file()
	{
		// Create folder "Logs" if it's not exist
		if (!std::filesystem::is_directory("Logs") || !std::filesystem::exists("Logs"))
		{
			if (!std::filesystem::create_directory("Logs"))
			{
				throw std::runtime_error("Can't create directory");
			}
		}

		auto name = std::string();

		// set name for log file
		name.append("CatLog-");
		name.append(get_time(true));
		name.append(".log");

		m_log_file_path = "Logs/" + name;
		m_log_file = std::ofstream(m_log_file_path, std::ios_base::app);

		if (!m_log_file)
		{
			return;
		}

		m_log_file << "Cat Engine Log file\n\n";
		m_log_file.close();
	}

	const char* logger::get_level_str(log_level level) const
	{
		switch (level)
		{
		case log_level::INFO:
			return "Info";
		case log_level::VERB:
			return "Verbose";
		case log_level::ERR:
			return "Error";
		case log_level::FATAL:
			return "Fatal";
		case log_level::WARN:
			return "Warning";
		default:
			return "Unknown";
		}

		return "";
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
		VA_LIST_OUTPUT(buffer);

		auto line = std::string();
		// Print line count
		line.append("[" + std::to_string(m_linecount) + "] ");

		// Add to line corrent time
		line.append(get_time(false));

		// Add type and text from buffer
		auto message = " [" + std::string(get_level_str(level)) + "]  " + std::string(buffer) + "\n";
		line.append(message);
		
		// Add current line to log file
		m_log_file << line;
		m_log_file.close();

		// Add line 
		m_linecount++;

		// If we are have debuging output
		OutputDebugString(line.c_str());

		m_scroll_to_bottom = true;
		console_text_buffer.append(line.c_str());

		line.clear();
		memset(buffer, 0, sizeof(buffer));

		// trigger exception if it fatal error
		if (level == log_level::FATAL)
		{
			throw std::runtime_error(std::string(buffer));
		}
	}

	void logger::render_console()
	{
		ImGui::Begin("Console");
		
		ImGui::TextUnformatted(console_text_buffer.begin());
		
		if (m_scroll_to_bottom)
			ImGui::SetScrollY(ImGui::GetScrollMaxY() + 10.0f);

		m_scroll_to_bottom = false;
		ImGui::End();
	}
}