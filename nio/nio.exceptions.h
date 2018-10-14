#ifndef __NIO_EXCEPTIONS__
#define __NIO_EXCEPTIONS__

#include <stdexcept>
#include <string>

namespace nio
{
	class invalid_mark_exception : public std::logic_error
	{
	public:
		explicit invalid_mark_exception(const std::string& what)
			: std::logic_error(what) { }
	};
	class buffer_underflow_exception : public std::logic_error
	{
	public:
		explicit buffer_underflow_exception(const std::string& what)
			: std::logic_error(what) { }
	};
	class buffer_overflow_exception : public std::logic_error
	{
	public:
		explicit buffer_overflow_exception(const std::string& what)
			: std::logic_error(what) { }
	};
	class read_only_buffer_exception : public std::logic_error
	{
	public:
		explicit read_only_buffer_exception(const std::string& what)
			: std::logic_error(what) { }
	};

	class closed_channel_exception : public std::logic_error
	{
	public:
		explicit closed_channel_exception(const std::string& what = "")
			: std::logic_error(what) { }
	};
	class illegal_blocking_mode_exception : public std::logic_error
	{
	public:
		explicit illegal_blocking_mode_exception(const std::string& what = "")
			: std::logic_error(what) { }
	};
	class not_yet_bound_exception : public std::logic_error
	{
	public:
		explicit not_yet_bound_exception(const std::string& what = "")
			: std::logic_error(what) { }
	};
	class illegal_selector_exception : public std::logic_error
	{
	public:
		explicit illegal_selector_exception(const std::string& what = "")
			: std::logic_error(what) { }
	};
	class cancelled_key_exception : public std::logic_error
	{
	public:
		explicit cancelled_key_exception(const std::string& what = "")
			: std::logic_error(what) { }
	};
	class already_connected_exception : public std::logic_error
	{
	public:
		explicit already_connected_exception(const std::string& what = "")
			: std::logic_error(what) { }
	};
	class connection_pending_exception : public std::logic_error
	{
	public:
		explicit connection_pending_exception(const std::string& what = "")
			: std::logic_error(what) { }
	};
	class no_connection_pending_exception : public std::logic_error
	{
	public:
		explicit no_connection_pending_exception(const std::string& what = "")
			: std::logic_error(what) { }
	};
}

#endif
