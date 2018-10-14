#ifndef __NIO_SERVER_SOCKET_CHANNEL_IMPL__
#define __NIO_SERVER_SOCKET_CHANNEL_IMPL__

#include <memory>

#include "nio.server_socket_channel.h"
#include "net.h"

namespace nio
{
	struct selector_provider;
	class server_socket_adapter;
	class server_socket_channel_impl : public server_socket_channel
	{
		volatile bool is_bound_;
		std::shared_ptr<server_socket_adapter> socket_;
	public:
		server_socket_channel_impl(
			const std::shared_ptr<selector_provider>& provider);
		virtual ~server_socket_channel_impl();
	public:
		void set_socket_adapter(
			const std::shared_ptr<server_socket_adapter>& adapter);
	public:
		bool is_bound() const;
	public:
		bool query(const char* iid, void** ppv);
	public:
		std::shared_ptr<net::server_socket> socket() const;
		std::shared_ptr<socket_channel> accept();
	protected:
		void implement_blocking(const bool& block);
		void implement_close_selectable_channel(void);
	friend class server_socket_adapter;
	};
}

#endif


