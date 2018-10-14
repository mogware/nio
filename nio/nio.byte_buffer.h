#ifndef __NIO_BYTE_BUFFER__
#define __NIO_BYTE_BUFFER__

#include <memory>
#include <vector>

#include "nio.buffer.h"

namespace nio
{
	class byte_buffer : public buffer
	{
	public:
		byte_buffer(const int& capacity);
	public:
		static std::shared_ptr<byte_buffer> of(const int& capacity);
		static std::shared_ptr<byte_buffer> of(std::vector<std::uint8_t>& array);
	public:
		const std::uint8_t* array(void) const
			{ return protected_array(); }
	public:
		virtual const std::uint8_t* protected_array(void) const = 0;
	public:
		virtual std::uint8_t get(void) const = 0;
		virtual void put(const std::uint8_t& b) = 0;
	public:
		virtual std::uint16_t get_short(void) const = 0;
		virtual void put_short(const std::uint16_t& n) = 0;
	public:
		virtual std::uint32_t get_int(void) const = 0;
		virtual void put_int(const std::uint32_t& n) = 0;
	};
}

#endif

