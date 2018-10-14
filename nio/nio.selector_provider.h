#ifndef __NIO_SELECTOR_PROVIDER__
#define __NIO_SELECTOR_PROVIDER__

#include <memory>

namespace nio
{
	struct selector;
	class socket_channel;
	class server_socket_channel;
	struct selector_provider
	{
		static std::shared_ptr<selector_provider> provider(void);
		virtual std::shared_ptr<selector> open_selector(void) = 0;
		virtual std::shared_ptr<socket_channel> open_socket_channel(void) = 0;
		virtual std::shared_ptr<server_socket_channel>
			open_server_socket_channel(void) = 0;
		virtual ~selector_provider(void);
	};
}

#endif



