#pragma once
#include "core/common.h"

#include <fstream> 

namespace cat::core::utils
{
	class CATENGINE_API	logger : public core::utils::singleton<logger>
	{
	public:
		enum log_level
		{
			NONE = 0,
			VERB,
			INFO,
			WARN,
			ERR,
			FATAL,
		};

		logger();
		~logger();

		 void				print(logger::log_level level, const char* text, ...);
		 void				create_log_file();

		 void				render_console();		
	private:
		 inline const char* get_level_str(logger::log_level level) const;

		 // get current time in string 
		 inline std::string	get_time(bool printMinAndSec);
		 
		 std::uint32_t	m_linecount;
		 std::ofstream	m_log_file;
		 log_level		m_level;
		 std::string	m_log_file_path;

		 bool				m_scroll_to_bottom;
	};
	

	// quick access to logger
	inline const auto log = cat::core::utils::logger::get_instance();
}

#define INFO(msg, ...) \
do \
{ \
	 cat::core::utils::log->print(cat::core::utils::logger::log_level::INFO, msg, ##__VA_ARGS__); \
} \
while(0)

#define VERB(msg, ...) \
do \
{ \
	 cat::core::utils::log->print(cat::core::utils::logger::log_level::VERB, msg, ##__VA_ARGS__); \
} \
while(0)

#define WARN(msg, ...) \
do \
{ \
	 cat::core::utils::log->print(cat::core::utils::logger::log_level::WARN, msg, ##__VA_ARGS__); \
} \
while(0)

#define ERR(msg, ...) \
do \
{ \
	 cat::core::utils::log->print(cat::core::utils::logger::log_level::ERR, msg, ##__VA_ARGS__); \
} \
while(0)

#define FATAL(msg, ...) \
do \
{ \
	 cat::core::utils::log->print(cat::core::utils::logger::log_level::FATAL, msg, ##__VA_ARGS__); \
} \
while(0)

#define CAT_NO_IMPL() ERR("%s Not implemented", __FUNCTION__)
