#pragma once
#include "core/common.h"

namespace cat::graphics
{
	class CATENGINE_API buffer
	{
	public:
		buffer() = default;
		virtual ~buffer() { }

		virtual void gen() = 0;
		virtual void bind() = 0;
		virtual void clear() = 0;

		virtual void unbind_buffer();
		virtual void unbind_buffer_array();
		virtual void unbind_all();


		template<typename type>
		[[nodiscard]] inline type* get() const { return reinterpret_cast<type>(m_data); }
		
		[[nodiscard]] inline std::int32_t size() const { return static_cast<std::int32_t>(m_size); }

	protected:
		void* m_data;
		std::size_t m_size;
	};
}