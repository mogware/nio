#include "nio.byte_vector_buffer.h"
#include "nio.exceptions.h"

nio::byte_vector_buffer::byte_vector_buffer(const int& capacity,
			const bool& readonly)
	: byte_buffer(capacity)
	, backing_array_(capacity, 0)
	, is_read_only_(readonly)
{
}

nio::byte_vector_buffer::byte_vector_buffer(std::vector<std::uint8_t>& array,
		const bool& readonly)
	: byte_buffer(array.size())
	, backing_array_(array)
	, is_read_only_(readonly)
{
}

bool nio::byte_vector_buffer::is_read_only() const
{
	return is_read_only_;
}

const std::uint8_t* nio::byte_vector_buffer::protected_array() const
{
	if (is_read_only())
		throw read_only_buffer_exception("Read-only buffer");
	return backing_array_.data();
}

std::uint8_t nio::byte_vector_buffer::get(void) const
{
	if (position_ == limit_)
		throw buffer_underflow_exception("Buffer underflow");
	uint8_t b = backing_array_[position_];
	const_cast<byte_vector_buffer *>(this)->position_++;
	return b;
}

void nio::byte_vector_buffer::put(const std::uint8_t& b)
{
	if (is_read_only())
		throw read_only_buffer_exception("Read-only buffer");
	if (position_ == limit_)
		throw buffer_overflow_exception("Buffer overflow");
	backing_array_[position_++] = b;
}

std::uint16_t nio::byte_vector_buffer::get_short(void) const
{
	if (position_ + 2 > limit_)
		throw buffer_underflow_exception("Buffer underflow");
	std::uint16_t n = peek_short(backing_array_, position_);
	const_cast<byte_vector_buffer *>(this)->position_ += 2;
	return n;
}

void nio::byte_vector_buffer::put_short(const std::uint16_t& n)
{
	if (is_read_only())
		throw read_only_buffer_exception("Read-only buffer");
	if (position_ + 2 > limit_)
		throw buffer_overflow_exception("Buffer overflow");
	poke_short(backing_array_, position_, n);
	position_ += 2;
}

std::uint32_t nio::byte_vector_buffer::get_int(void) const
{
	if (position_ + 4 > limit_)
		throw buffer_underflow_exception("Buffer underflow");
	std::uint32_t n = peek_int(backing_array_, position_);
	const_cast<byte_vector_buffer *>(this)->position_ += 4;
	return n;
}

void nio::byte_vector_buffer::put_int(const std::uint32_t& n)
{
	if (is_read_only())
		throw read_only_buffer_exception("Read-only buffer");
	if (position_ + 4 > limit_)
		throw buffer_overflow_exception("Buffer overflow");
	poke_int(backing_array_, position_, n);
	position_ += 4;
}

std::uint16_t nio::byte_vector_buffer::peek_short(
	const std::vector<std::uint8_t>& array, const int& pos)
{
	std::uint16_t n = static_cast<std::uint16_t>(array[pos + 1]);
	n |= (static_cast<std::uint16_t>(array[pos + 0]) << 8);
	return n;
}

std::uint32_t nio::byte_vector_buffer::peek_int(
	const std::vector<std::uint8_t>& a, const int& p)
{
	std::uint32_t n = static_cast<std::uint32_t>(a[p + 3]);
	n |= (static_cast<std::uint32_t>(a[p + 2]) << 8);
	n |= (static_cast<std::uint32_t>(a[p + 1]) << 16);
	n |= (static_cast<std::uint32_t>(a[p + 0]) << 24);
	return n;
}

std::uint64_t nio::byte_vector_buffer::peek_long(
	const std::vector<std::uint8_t>& a, const int& p)
{
	std::uint64_t n = static_cast<std::uint64_t>(a[p+7]);
	n |= (static_cast<std::uint64_t>(a[p+6]) << 8);
	n |= (static_cast<std::uint64_t>(a[p+5]) << 16);
	n |= (static_cast<std::uint64_t>(a[p+4]) << 24);
	n |= (static_cast<std::uint64_t>(a[p+3]) << 32);
	n |= (static_cast<std::uint64_t>(a[p+2]) << 40);
	n |= (static_cast<std::uint64_t>(a[p+1]) << 48);
	n |= (static_cast<std::uint64_t>(a[p+0]) << 54);
	return n;
}

void nio::byte_vector_buffer::poke_short(std::vector<std::uint8_t>& a,
	const int& p, const std::uint16_t& v)
{
	a[p+0] = static_cast<std::uint8_t>((v >> 8) & 0xFF);
	a[p+1] = static_cast<std::uint8_t>((v >> 0) & 0xFF);
}

void nio::byte_vector_buffer::poke_int(std::vector<std::uint8_t>& a,
	const int& p, const std::uint32_t& v)
{
	a[p+0] = static_cast<std::uint8_t>((v >> 24) & 0xFF);
	a[p+1] = static_cast<std::uint8_t>((v >> 16) & 0xFF);
	a[p+2] = static_cast<std::uint8_t>((v >> 8) & 0xFF);
	a[p+3] = static_cast<std::uint8_t>((v >> 0) & 0xFF);
}

void nio::byte_vector_buffer::poke_long(std::vector<std::uint8_t>& a,
	const int& p, const std::uint64_t& v)
{
	a[p+0] = static_cast<std::uint8_t>((v >> 56) & 0xFF);
	a[p+1] = static_cast<std::uint8_t>((v >> 48) & 0xFF);
	a[p+2] = static_cast<std::uint8_t>((v >> 40) & 0xFF);
	a[p+3] = static_cast<std::uint8_t>((v >> 32) & 0xFF);
	a[p+4] = static_cast<std::uint8_t>((v >> 24) & 0xFF);
	a[p+5] = static_cast<std::uint8_t>((v >> 16) & 0xFF);
	a[p+6] = static_cast<std::uint8_t>((v >> 8) & 0xFF);
	a[p+7] = static_cast<std::uint8_t>((v >> 0) & 0xFF);
}
