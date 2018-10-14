#ifndef __NIO_SOCKET_CHANNEL__
#define __NIO_SOCKET_CHANNEL__

#include <memory>

#include "nio.selector_provider.h"
#include "nio.abstract_selectable_channel.h"
#include "nio.byte_buffer.h"

#include "net.h"

namespace nio
{
	class socket_channel : public abstract_selectable_channel
	{
	public:
		socket_channel(const std::shared_ptr<selector_provider>& provider);
		virtual ~socket_channel();
	public:
		static std::shared_ptr<socket_channel> open(void);
		static std::shared_ptr<socket_channel> open(
			const net::socket_address& address);
	public:
		int valid_ops(void) const;
	public:
		virtual int read(std::shared_ptr<byte_buffer>& buffer) = 0;
		virtual int write(const std::shared_ptr<byte_buffer>& buffer) const = 0;
	public:
		virtual std::shared_ptr<net::socket> socket() const = 0;
		virtual bool is_connected(void) const = 0;
		virtual bool is_connection_pending(void) const = 0;
		virtual bool connect(const net::socket_address& address) = 0;
		virtual bool finish_connect(void) = 0;
	};
}

#endif

