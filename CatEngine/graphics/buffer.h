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

		static void unbind_buffer();
		static void unbind_vertex_array();

		static void unbind_all();


		template<typename type>
		[[nodiscard]] inline type* get() const { return reinterpret_cast<type>(m_data); }
		
		[[nodiscard]] inline std::int32_t size() const { return sizeof(m_data); }

	protected:
		void* m_data;
	};
}