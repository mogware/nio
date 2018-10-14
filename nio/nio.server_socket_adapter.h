#ifndef __NIO_SERVER_SOCKET_ADAPTER__
#define __NIO_SERVER_SOCKET_ADAPTER__

#include "net.h"

namespace nio
{
	class server_socket_channel_impl;
	class socket_channel_impl;
	class server_socket_adapter : public net::server_socket
	{
		std::weak_ptr<server_socket_channel_impl> impl_;
	public:
		server_socket_adapter(
			const std::shared_ptr<server_socket_channel_impl>& impl);
	public:
		virtual ~server_socket_adapter(void);
	public:
		std::shared_ptr<net::socket> accept(void);
		std::shared_ptr<net::socket> implement_accept(
			const std::shared_ptr<socket_channel_impl>& channel);
		void bind(const net::socket_address& localaddr);
		void bind(const net::socket_address& localaddr, const int& backlog);
		bool is_bound(void) const;
		void close(void);
	};
}

#endif
