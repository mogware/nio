#ifndef __NIO_SOCKET_ADAPTER__
#define __NIO_SOCKET_ADAPTER__

#include "net.h"

namespace nio
{
	class socket_channel_impl;
	class socket_adapter : public net::socket
	{
		std::weak_ptr<socket_channel_impl> channel_;
	public:
		socket_adapter(const std::shared_ptr<socket_channel_impl>& channel);
		socket_adapter(const std::shared_ptr<net::socket_impl>& impl,
			const std::shared_ptr<socket_channel_impl>& channel);
	public:
		virtual ~socket_adapter(void);
	public:
		bool is_connected(void) const;
		bool is_bound(void) const;
		std::shared_ptr<net::net_address> get_local_address(void) const;
	public:
		void bind(const net::socket_address& localaddr);
		void connect(const net::socket_address& remoteaddr, const int& timeout);
		void close(void);
	public:
		std::basic_streambuf<char, std::char_traits<char>>* get_stream(void);
	};
}

#endif

