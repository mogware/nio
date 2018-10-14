#include "nio.socket_channel.h"
#include "nio.socket_channel_impl.h"
#include "nio.selection_key.h"

nio::socket_channel::socket_channel(
	const std::shared_ptr<nio::selector_provider>& provider)
	: abstract_selectable_channel(provider)
{
}

nio::socket_channel::~socket_channel()
{
}

std::shared_ptr<nio::socket_channel> nio::socket_channel::open(void)
{
	return selector_provider::provider()->open_socket_channel();
}

std::shared_ptr<nio::socket_channel> nio::socket_channel::open(const net::socket_address& address)
{
	std::shared_ptr<socket_channel> sc = open();
	if (sc != nullptr)
		sc->connect(address);
	return sc;
}

int nio::socket_channel::valid_ops(void) const
{
	return selection_key::op_connect | selection_key::op_read |
		selection_key::op_write;
}
