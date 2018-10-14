#include "nio.server_socket_adapter.h"
#include "nio.server_socket_channel_impl.h"
#include "nio.socket_channel_impl.h"
#include "nio.exceptions.h"

nio::server_socket_adapter::server_socket_adapter(
	const std::shared_ptr<nio::server_socket_channel_impl>& impl)
	: impl_(impl)
{
}

nio::server_socket_adapter::~server_socket_adapter(void)
{
}

std::shared_ptr<net::socket> nio::server_socket_adapter::accept(void)
{
	if (auto impl = impl_.lock())
	{
		if (!impl->is_bound())
			throw illegal_blocking_mode_exception();
		std::shared_ptr<nio::socket_channel> sc = impl->accept();
		if (sc == nullptr)
			throw illegal_blocking_mode_exception();
		return sc->socket();
	}
	throw illegal_blocking_mode_exception();
}

std::shared_ptr<net::socket> nio::server_socket_adapter::implement_accept(
	const std::shared_ptr<nio::socket_channel_impl>& channel)
{
	std::shared_ptr<net::socket> sock = channel->socket();
	try {
		net::server_socket::implement_accept(sock);
		channel->set_connected();
		channel->set_bound(true);
		channel->finish_accept(sock->get_impl()->get_native_socket());
	}
	catch (const net::socket_timeout_exception&) {
		sock->close();
	}
	catch (const net::socket_exception&) {
		sock->close();
	}
	return sock;
}

void nio::server_socket_adapter::bind(const net::socket_address& localaddr)
{
	if (auto impl = impl_.lock()) {
		net::server_socket::bind(localaddr);
		impl->is_bound_ = true;
	}
}
void nio::server_socket_adapter::bind(const net::socket_address& localaddr, const int& backlog)
{
	if (auto impl = impl_.lock()) {
		net::server_socket::bind(localaddr, backlog);
		impl->is_bound_ = true;
	}
}

bool nio::server_socket_adapter::is_bound(void) const
{
	if (auto impl = impl_.lock())
		return impl->is_bound();
	return false;
}

void nio::server_socket_adapter::close(void)
{
	if (auto impl = impl_.lock())
	{
		if (impl->is_open())
			impl->close();
		else
			net::server_socket::close();
	}
	else
		net::server_socket::close();
}
