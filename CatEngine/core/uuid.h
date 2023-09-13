#pragma once
#include "core/common.h"
#include <boost/uuid/uuid.hpp>

namespace cat::core
{
	class CATENGINE_API uuid
	{
	public:
		uuid();
		~uuid();

		// Set uuid by string 
		void set_uuid(const std::string& uuid_str);

		// Set uuid by existing uuid
		void set_uuid(const boost::uuids::uuid& uuid);

		// Get current uuid in string format 
		[[nodiscard]] inline std::string get_uuid_in_str() const;

		// Get current uuid
		[[nodiscard]] inline boost::uuids::uuid get_uuid() const;

		// Generate new UUID
		void generate_uuid();

		// Convert string to uuid 
		static boost::uuids::uuid convert_str_to_uuid(const std::string id);

		// Convert uuid to string 
		static std::string convert_uuid_to_str(boost::uuids::uuid uuid);

	private:
		boost::uuids::uuid m_uuid;

	};
}
