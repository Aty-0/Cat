#include "uuid.h"

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <sstream>

namespace cat::core
{
	uuid::uuid()
		: m_uuid(boost::uuids::random_generator()())
	{

	}

	uuid::~uuid()
	{
		m_uuid = boost::uuids::nil_uuid();
	}

	void uuid::generate_uuid()
	{
		m_uuid = boost::uuids::random_generator()();
	}

	void uuid::set_uuid(const boost::uuids::uuid& uuid)
	{
		m_uuid = uuid;
	}

	void uuid::set_uuid(const std::string& uuid_str)
	{
		m_uuid = boost::uuids::nil_uuid();
		std::istringstream strm(uuid_str);
		strm >> m_uuid;
	}

	[[nodiscard]] inline std::string uuid::get_uuid_in_str() const
	{
		return boost::uuids::to_string(m_uuid);
	}

	[[nodiscard]] inline boost::uuids::uuid uuid::get_uuid() const
	{
		return m_uuid;
	}

	boost::uuids::uuid uuid::convert_str_to_uuid(const std::string id)
	{
		boost::uuids::uuid uuid = boost::uuids::nil_uuid();
		std::istringstream strm(id);
		strm >> uuid;
		return uuid;
	}

	std::string uuid::convert_uuid_to_str(boost::uuids::uuid uuid)
	{
		return boost::uuids::to_string(uuid);
	}
}