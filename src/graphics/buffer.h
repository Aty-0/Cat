#pragma once
#include "core/common.h"

namespace cat::graphics
{
	template<typename T>
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


		[[nodiscard]] inline T* get() const { return reinterpret_cast<T>(m_data.data()); }	
		[[nodiscard]] inline std::vector<T> const getVector() { return m_data; }	
		[[nodiscard]] inline std::uint32_t size() const { return static_cast<std::uint32_t>(m_data.size()); }

	protected:
		std::vector<T> m_data;
	};

	template<typename T>
	inline void buffer<T>::unbindBuffer()
	{

	}

	template<typename T>
	inline void buffer<T>::unbindBufferArray()
	{

	}

	template<typename T>
	inline void buffer<T>::unbindAll()
	{
		unbindBuffer();
		unbindBufferArray();
	}
}