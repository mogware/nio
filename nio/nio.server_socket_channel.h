#ifndef __NIO_SERVER_SOCKET_CHANNEL__
#define __NIO_SERVER_SOCKET_CHANNEL__

#include <memory>

#include "nio.abstract_selectable_channel.h"
#include "net.h"

namespace nio
{
	class socket_channel;
	class server_socket;
	class server_socket_channel : public abstract_selectable_channel
	{
	public:
		server_socket_channel(const std::shared_ptr<selector_provider>& provider);
		virtual ~server_socket_channel();
	public:
		static std::shared_ptr<server_socket_channel> open(void);
	public:
		int valid_ops(void) const;
	public:
		virtual std::shared_ptr<net::server_socket> socket() const = 0;
		virtual std::shared_ptr<socket_channel> accept() = 0;
	};
}

#endif


