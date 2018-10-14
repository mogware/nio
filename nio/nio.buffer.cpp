#include "nio.buffer.h"
#include "nio.exceptions.h"

#include <string>
#include <sstream>

static std::string to_string(const int& n)
{
	std::ostringstream stream;
	stream << n;
	return stream.str();
}

nio::buffer::buffer(int capacity)
	: capacity_(capacity)
	, limit_(capacity)
	, mark_(unset_mark)
	, position_(0)
{
}

void nio::buffer::check_index(const int& index)
{
	if (index < 0 || index >= limit_)
		throw std::out_of_range("index=" + to_string(index) +
			", limit=" + to_string(limit_));
}

void nio::buffer::check_index(const int& index, const int& size_of_type)
{
	if (index < 0 || index > limit_ - size_of_type)
		throw std::out_of_range("index=" + to_string(index) +
			", limit=" + to_string(limit_) +
			", size of type=" + to_string(size_of_type));
}

int nio::buffer::check_get_bounds(const int& bytes_per_element,
		const int& length, const int& offset, const int& count)
{
	int byte_count = bytes_per_element * count;
	if ((offset | count) < 0 || offset > length || length - offset < count)
		throw std::out_of_range("offset=" + to_string(offset) +
			", count=" + to_string(count) +
			", length=" + to_string(length));
	if (byte_count > remaining())
		throw buffer_underflow_exception("Buffer underflow");
	return byte_count;
}

int nio::buffer::check_set_bounds(const int& bytes_per_element,
	const int& length, const int& offset, const int& count)
{
	int byte_count = bytes_per_element * count;
	if ((offset | count) < 0 || offset > length || length - offset < count)
		throw std::out_of_range("offset=" + to_string(offset) +
			", count=" + to_string(count) +
			", length=" + to_string(length));
	if (byte_count > remaining())
		throw buffer_overflow_exception("Buffer overflow");
	if (is_read_only())
		throw read_only_buffer_exception("Read-only buffer");
	return byte_count;
}

void nio::buffer::check_start_end_remaining(const int& start, const int& end)
{
	if (end < start || start < 0 || end > remaining())
		throw std::out_of_range("start=" + to_string(start) +
			", end=" + 	to_string(end) +
			", remaining()=" + to_string(remaining()));
}

void nio::buffer::check_writable(void)
{
	if (is_read_only())
		throw std::invalid_argument("Read-only buffer");
}

void nio::buffer::clear(void)
{
	position_ = 0;
	mark_ = unset_mark;
	limit_ = capacity_;
}

void nio::buffer::flip(void)
{
	limit_ = position_;
	mark_ = unset_mark;
	position_ = 0;
}

int nio::buffer::capacity(void) const
{
	return capacity_;
}

bool nio::buffer::has_remaining(void) const
{
	return position_ < limit_;
}

int nio::buffer::remaining(void) const
{
	return limit_ - position_;
}

int nio::buffer::limit(void) const
{
	return limit_;
}

void nio::buffer::limit(const int& new_limit)
{
	if (new_limit < 0 || new_limit > capacity_)
		throw std::invalid_argument("Bad limit (capacity " +
			to_string(capacity_) + "): " + to_string(new_limit));
	limit_ = new_limit;
	if (position_ > new_limit)
		position_ = new_limit;
	if ((mark_ != unset_mark) && (mark_ > new_limit))
		mark_ = unset_mark;
}

void nio::buffer::mark(void)
{
	mark_ = position_;
}

int nio::buffer::position(void) const
{
	return position_;
}

void nio::buffer::position(const int& new_position)
{
	if (new_position < 0 || new_position > limit_)
		std::invalid_argument("Bad position (limit " +
			to_string(limit_) + "): " + to_string(new_position));
	position_ = new_position;
	if ((mark_ != unset_mark) && (mark_ > position_))
		mark_ = unset_mark;
}

void nio::buffer::reset(void)
{
	if (mark_ == unset_mark)
		throw invalid_mark_exception("Mark not set");
	position_ = mark_;
}

void nio::buffer::rewind(void)
{
	position_ = 0;
	mark_ = unset_mark;
}