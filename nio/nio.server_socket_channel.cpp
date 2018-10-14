#include "nio.server_socket_channel.h"
#include "nio.selection_key.h"
#include "nio.selector_provider.h"

nio::server_socket_channel::server_socket_channel(
	const std::shared_ptr<nio::selector_provider>& provider)
	: abstract_selectable_channel(provider)
{
}

nio::server_socket_channel::~server_socket_channel()
{
}

std::shared_ptr<nio::server_socket_channel> nio::server_socket_channel::open(void)
{
	return selector_provider::provider()->open_server_socket_channel();
}

int nio::server_socket_channel::valid_ops(void) const
{
	return selection_key::op_accept;
}
