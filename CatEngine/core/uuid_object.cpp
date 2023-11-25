#include "uuid_object.h"
#include "core/utils/logger.h"

namespace cat::core
{
	uuid_object::uuid_object() : 
		m_id(make_new())
	{

	}

	uuid_object::~uuid_object()
	{

	}

	uuids::uuid uuid_object::make_new()
	{
		uuids::uuid id = uuids::uuid_system_generator{}();

		CAT_ASSERT(!id.is_nil());
		CAT_ASSERT(id.version() == uuids::uuid_version::random_number_based);
		CAT_ASSERT(id.variant() == uuids::uuid_variant::rfc);

		return id;
	}

	void uuid_object::set_id(const std::string& id_str)
	{
		CAT_ASSERT(!id_str.empty());

		m_id = str_to_id(id_str);
	}

	void uuid_object::set_id(const uuids::uuid& id)
	{
		CAT_ASSERT(!id.is_nil());
		m_id = id;
	}

	std::string uuid_object::get_id_str() const
	{
		return uuids::to_string(m_id);
	}

	
	uuids::uuid uuid_object::get_id() const
	{
		return m_id;
	}
	
	uuids::uuid uuid_object::str_to_id(const std::string id_str)
	{
		auto id = uuids::uuid::from_string(id_str);
		CAT_ASSERT(id.has_value());
		CAT_ASSERT(uuids::to_string(id.value()) == id_str);

		return id.value();
	}

	std::string uuid_object::id_to_str(uuids::uuid id)
	{
		return uuids::to_string(id);
	}

}