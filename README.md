# Class library for non-blocking networking I/O

## Usage

Example of simple client/server usage

```c
void server(const bool& ipv6)
{
	sio::socket_init();

	// selector: multiplexor of selectable_channel objects
	std::shared_ptr<nio::selector> selector = nio::selector::open();

	// server_socket_channel: selectable channel for listening sockets
	std::shared_ptr<nio::server_socket_channel> server =
			nio::server_socket_channel::open();
	net::socket_address raddr(net::net_address::get_local_host(ipv6), 7777);

	// binds the channel's socket to a local address
	server->socket()->bind(raddr);

	// adjusts this channel's blocking mode
	server->configure_blocking(false);

	int ops = server->valid_ops();
	std::shared_ptr<nio::selection_key> key = server->enroll(selector, ops, nullptr);

	while (true)
	{
		// selects keys for channels ready for I/O operations
		selector->select();

		// token representing a selectable_channel with a selector
		std::set<std::shared_ptr<nio::selection_key>>& keys = selector->selected_keys();
		std::set<std::shared_ptr<nio::selection_key>>::iterator it = keys.begin();
		while (it != keys.end())
		{
			std::shared_ptr<nio::selection_key> key = *it;
			if (key->is_acceptable())
			{
				std::shared_ptr<nio::socket_channel> client = server->accept();

				// adjusts this channel's blocking mode to false
				client->configure_blocking(false);

				// operation-set bit for read operations
				client->enroll(selector, nio::selection_key::op_read, nullptr);
			}
			else if (key->is_readable())
			{
				nio::socket_channel* client = nullptr;
				if (key->channel()->query(IID_PPV_ARG(nio::socket_channel, &client)))
				{
					std::shared_ptr<nio::byte_buffer> bb = nio::byte_buffer::of(256);
					client->read(bb);
					std::string result(reinterpret_cast<char *>(const_cast<uint8_t *>(bb->array())));
					printf("Message received: %s\n", result.c_str());
					if (result.compare("Crunchify") == 0)
						client->close();

				}
			}
			it = keys.erase(it);
		}
	}

	sio::socket_term();
}
```

```c
void client(const bool& ipv6)
{
	sio::socket_init();
	net::socket_address raddr(net::net_address::get_local_host(), 7777);
	std::shared_ptr<nio::socket_channel> client = nio::socket_channel::open(raddr);
	std::vector<std::string> company_details;
	company_details.push_back("Facebook");
	company_details.push_back("Twitter");
	company_details.push_back("IBM");
	company_details.push_back("Google");
	company_details.push_back("Crunchify");
	for (std::string name : company_details)
	{
		std::vector<std::uint8_t> message(name.begin(), name.end());
		//nio::byte_vector_buffer buffer(message);
		std::shared_ptr<nio::byte_buffer> buffer = nio::byte_buffer::of(message);
		client->write(buffer);
#if defined(SIO_WIN32)
		Sleep(2000);
#else
		sleep(2);
#endif
	}
	sio::socket_term();
}
```
