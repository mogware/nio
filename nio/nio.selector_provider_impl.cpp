#include "nio.selector_provider_impl.h"
#include "nio.selector_impl.h"
#include "nio.socket_channel_impl.h"
#include "nio.socket_adapter.h"
#include "nio.server_socket_channel_impl.h"
#include "nio.server_socket_adapter.h"

std::shared_ptr<nio::selector>
nio::selector_provider_impl::open_selector(void)
{
	return std::make_shared<selector_impl>(shared_from_this());
}

std::shared_ptr<nio::socket_channel>
nio::selector_provider_impl::open_socket_channel(void)
{
	std::shared_ptr<socket_channel_impl> impl =
		std::make_shared<socket_channel_impl>(shared_from_this());
	std::shared_ptr<socket_adapter> adapter =
		std::make_shared<socket_adapter>(impl);
	impl->set_socket_adapter(adapter);
	return impl;
}

std::shared_ptr<nio::server_socket_channel>
nio::selector_provider_impl::open_server_socket_channel(void)
{
	std::shared_ptr<server_socket_channel_impl> impl =
		std::make_shared<server_socket_channel_impl>(shared_from_this());
	std::shared_ptr<server_socket_adapter> adapter =
		std::make_shared<server_socket_adapter>(impl);
	impl->set_socket_adapter(adapter);
	return impl;
}