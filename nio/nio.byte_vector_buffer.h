#ifndef __NIO_BYTE_VECTOR_BUFFER__
#define __NIO_BYTE_VECTOR_BUFFER__

#include <vector>

#include "nio.byte_buffer.h"

namespace nio
{
	class byte_vector_buffer : public byte_buffer
	{
		std::vector<std::uint8_t> backing_array_;
		const bool is_read_only_;
	public:
		byte_vector_buffer(const int& capacity, const bool& readonly = false);
		byte_vector_buffer(std::vector<std::uint8_t>& array,
			const bool& readonly = false);
	public:
		bool is_read_only(void) const;
	public:
		const std::uint8_t* protected_array(void) const;
	public:
		std::uint8_t get(void) const;
		void put(const std::uint8_t& b);
	public:
		std::uint16_t get_short(void) const;
		void put_short(const std::uint16_t& n);
	public:
		std::uint32_t get_int(void) const;
		void put_int(const std::uint32_t& n);
	private:
		static std::uint16_t peek_short(
			const std::vector<std::uint8_t>& a, const int& p);
		static std::uint32_t peek_int(
			const std::vector<std::uint8_t>& a, const int& p);
		static std::uint64_t peek_long(
			const std::vector<std::uint8_t>& a, const int& p);
		static void poke_short(std::vector<std::uint8_t>& a,
			const int& p, const std::uint16_t& v);
		static void poke_int(std::vector<std::uint8_t>& a,
			const int& p, const std::uint32_t& v);
		static void poke_long(std::vector<std::uint8_t>& a,
			const int& p, const std::uint64_t& v);
	};

}

#endif