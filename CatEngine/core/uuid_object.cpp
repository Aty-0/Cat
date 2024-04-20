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

	[[nodiscard]] inline bool uuid_object::empty() const
	{
		return m_id.is_nil();
	}

	void uuid_object::set(const std::string& id_str)
	{
		CAT_ASSERT(!id_str.empty());

		m_id = toID(id_str);
	}

	void uuid_object::set(const uuids::uuid& id)
	{
		CAT_ASSERT(!id.is_nil());
		m_id = id;
	}

	std::string uuid_object::getIDStr() const
	{
		return uuids::to_string(m_id);
	}

	
	uuids::uuid uuid_object::getID() const
	{
		return m_id;
	}
	
	uuids::uuid uuid_object::toID(const std::string id_str)
	{
		CAT_ASSERT(!id_str.empty());
		auto id = uuids::uuid::from_string(id_str);
		CAT_ASSERT(id.has_value());
		CAT_ASSERT(uuids::to_string(id.value()) == id_str);

		return id.value();
	}

	std::string uuid_object::toStr(uuids::uuid id)
	{
		return uuids::to_string(id);
	}

}