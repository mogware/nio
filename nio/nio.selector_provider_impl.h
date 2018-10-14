#ifndef __NIO_SELECTOR_PROVIDER_IMPL__
#define __NIO_SELECTOR_PROVIDER_IMPL__

#include "nio.selector_provider.h"

namespace nio
{
	struct selector;
	class socket_channel;
	class server_socket_channel;
	struct selector_provider_impl : public selector_provider,
			std::enable_shared_from_this<selector_provider_impl>
	{
		std::shared_ptr<selector> open_selector(void);
		std::shared_ptr<socket_channel> open_socket_channel(void);
		std::shared_ptr<server_socket_channel> open_server_socket_channel(void);
	};
}

#endif




