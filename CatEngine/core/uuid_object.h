#pragma once
#include "core/common.h"

#define UUID_SYSTEM_GENERATOR
#include "libs/uuid.h"


namespace cat::core
{
	class CATENGINE_API uuid_object
	{
	public:
		uuid_object();
		~uuid_object();

		// Generate new UUID
		uuids::uuid make_new();

		// Set uuid by string 
		void set(const std::string& uuid_str);

		// Set uuid by existing uuid
		void set(const uuids::uuid& uuid);

		// Get current uuid in string format 
		[[nodiscard]] inline std::string get_id_str() const;

		// Get current uuid
		[[nodiscard]] inline uuids::uuid get_id() const;
		

		// Convert string to uuid 
		static uuids::uuid to_id(const std::string id_str);

		// Convert uuid to string 
		static std::string to_str(uuids::uuid id);

		[[nodiscard]] inline bool empty() const;
		
	private:
		uuids::uuid m_id;
	};
}
