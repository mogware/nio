#include <vector>

#include "nio.byte_buffer.h"
#include "nio.byte_vector_buffer.h"

nio::byte_buffer::byte_buffer(const int& capacity)
	: buffer(capacity)
{
}

std::shared_ptr<nio::byte_buffer> nio::byte_buffer::of(const int& capacity)
{
	return std::make_shared<byte_vector_buffer>(capacity);
}

std::shared_ptr<nio::byte_buffer> nio::byte_buffer::of(
		std::vector<std::uint8_t>& array)
{
	return std::make_shared<byte_vector_buffer>(array);
}
