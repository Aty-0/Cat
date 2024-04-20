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

		virtual void unbindBuffer();
		virtual void unbindBufferArray();
		virtual void unbindAll();


		template<typename type>
		[[nodiscard]] inline type* get() const { return reinterpret_cast<type>(m_data); }
		
		[[nodiscard]] inline std::int32_t size() const { return static_cast<std::int32_t>(m_size); }

	protected:
		void* m_data;
		std::size_t m_size;
	};
}