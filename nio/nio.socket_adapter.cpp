#include "nio.socket_adapter.h"
#include "nio.socket_channel_impl.h"
#include "nio.exceptions.h"

nio::socket_adapter::socket_adapter(
	const std::shared_ptr<nio::socket_channel_impl>& channel)
	: net::socket(false)
	, channel_(channel)
{
}

nio::socket_adapter::socket_adapter(
		const std::shared_ptr<net::socket_impl>& impl,
		const std::shared_ptr<nio::socket_channel_impl>& channel)
	: net::socket(impl, false)
	, channel_(channel)
{
}

nio::socket_adapter::~socket_adapter(void)
{
}

bool nio::socket_adapter::is_connected(void) const
{
	if (auto channel = channel_.lock())
		return channel->is_connected();
	return false;
}

bool nio::socket_adapter::is_bound(void) const
{
	if (auto channel = channel_.lock())
		return channel->is_bound();
	return false;
}

std::shared_ptr<net::net_address> nio::socket_adapter::get_local_address(void) const
{
	if (auto channel = channel_.lock())
		return channel->get_local_address();
	return nullptr;
}

void nio::socket_adapter::bind(const net::socket_address& localaddr)
{
	if (auto channel = channel_.lock())
	{
		if (channel->is_connected())
			throw already_connected_exception();
		if (socket_channel_impl::socket_status_pending == channel->status_)
			throw connection_pending_exception();
		net::socket::bind(localaddr);
		channel->init_local_address_and_port();
		channel->is_bound_ = true;
	}
}

void nio::socket_adapter::connect(const net::socket_address& remoteaddr, const int& timeout)
{
	if (auto channel = channel_.lock())
	{
		if (!channel->is_blocking())
			throw illegal_blocking_mode_exception();
		if (channel->is_connected())
			throw already_connected_exception();
		net::socket::connect(remoteaddr, timeout);
		channel->init_local_address_and_port();
		if (net::socket::is_connected())
		{
			channel->set_connected();
			channel->is_bound_ = net::socket::is_bound();
		}
	}
}

void nio::socket_adapter::close(void)
{
	if (auto channel = channel_.lock())
	{
		if (channel->is_open())
			channel->close();
		else
			net::socket::close();
		channel->status_ = socket_channel_impl::socket_status_closed;
	}
}

std::basic_streambuf<char, std::char_traits<char>>*
nio::socket_adapter::get_stream(void)
{
// TODO
	return nullptr;
}
