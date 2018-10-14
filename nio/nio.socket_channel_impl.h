#ifndef __NIO_SOCKET_CHANNEL_IMPL__
#define __NIO_SOCKET_CHANNEL_IMPL__

#include <memory>

#include "nio.socket_channel.h"
#include "net.h"
#include "sio.h"

namespace nio
{
	struct selector_provider;
	class socket_adapter;
	class socket_channel_impl : public socket_channel
	{
		enum
		{
			socket_status_uninitialized = -1,
			socket_status_unconnected = 0,
			socket_status_pending = 1,
			socket_status_connected = 2,
			socket_status_closed = 3
		} status_;

		sio::socket_t sockfd_;

		std::shared_ptr<net::socket_address> connect_address_;
		std::shared_ptr<net::net_address> local_address_;
		int local_port_;

		volatile bool is_bound_;

		std::shared_ptr<socket_adapter> socket_;
	public:
		socket_channel_impl(const std::shared_ptr<selector_provider>& provider);
		socket_channel_impl(const std::shared_ptr<selector_provider>& provider,
			const bool& connect);
		socket_channel_impl(const std::shared_ptr<selector_provider>& provider,
			const sio::socket_t& sockfd);
		virtual ~socket_channel_impl();
	public:
		void set_socket_adapter(
			const std::shared_ptr<socket_adapter>& adapter);
		int read(std::shared_ptr<byte_buffer>& buffer);
		int write(const std::shared_ptr<byte_buffer>& buffer) const;
	public:
		bool query(const char* iid, void** ppv);
	public:
		std::shared_ptr<net::net_address> get_local_address(void) const;
	public:
		std::shared_ptr<net::socket> socket() const;
		bool is_bound(void) const;
		bool is_connected(void) const;
		bool is_connection_pending(void) const;
		bool connect(const net::socket_address& address);
		bool finish_connect(void);
	public:
		void set_connected(void);
		void set_bound(bool flag);
		void finish_accept(sio::socket_t sockfd);
	protected:
		void implement_blocking(const bool& block);
		void implement_close_selectable_channel(void);
	private:
		void init_local_address_and_port(void);
		void check_open_connected(void) const;
		void check_unconnected(void) const;
		int read_implement(std::shared_ptr<byte_buffer>& buffer);
		int write_implement(const std::shared_ptr<byte_buffer>& buffer) const;
	private:
		static sio::socket_t newsocket(void);
		static void connect(const sio::socket_t& sockfd, const std::shared_ptr<net::net_address>& addr,
			const std::uint16_t& port);
		static bool connected(const sio::socket_t& sockfd, const std::shared_ptr<net::net_address>& addr,
			const std::uint16_t& port);
	friend class socket_adapter;
	};
}

#endif


