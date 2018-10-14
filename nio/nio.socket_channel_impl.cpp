#include "nio.socket_channel_impl.h"
#include "nio.socket_adapter.h"
#include "net.default_socket_impl.h"
#include "nio.exceptions.h"

nio::socket_channel_impl::socket_channel_impl(
		const std::shared_ptr<nio::selector_provider>& provider)
	: socket_channel_impl(provider, true)
{
}

nio::socket_channel_impl::socket_channel_impl(
	const std::shared_ptr<selector_provider>& provider,
	const bool& connect)
	: socket_channel(provider)
	, status_(socket_status_uninitialized)
	, sockfd_(connect ? newsocket() : sio::invalid_socket)
	, connect_address_(nullptr)
	, local_address_(nullptr)
	, local_port_(0)
	, is_bound_(false)
	, socket_(nullptr)
{
	status_ = socket_status_unconnected;
}

nio::socket_channel_impl::socket_channel_impl(
		const std::shared_ptr<selector_provider>& provider,
		const sio::socket_t& sockfd)
	: socket_channel(provider)
	, status_(socket_status_uninitialized)
	, sockfd_(sockfd)
	, connect_address_(nullptr)
	, local_address_(nullptr)
	, local_port_(0)
	, is_bound_(false)
	, socket_(nullptr)
{
	status_ = socket_status_connected;
}

nio::socket_channel_impl::~socket_channel_impl()
{
	close();
}

void nio::socket_channel_impl::set_socket_adapter(
	const std::shared_ptr<socket_adapter>& adapter)
{
	socket_ = adapter;
}

void nio::socket_channel_impl::implement_blocking(const bool& block)
{
	int on = block ? 0 : 1;
	sio::ioctlsocket(sockfd_, sio::sio_nbio, &on);
}

void nio::socket_channel_impl::implement_close_selectable_channel(void)
{
	if (status_ != socket_status_closed)
	{
		status_ = socket_status_closed;
		if (socket_ != nullptr && !socket_->is_closed())
			socket_->close();
		else
			sio::closesocket(sockfd_);
	}
}

int nio::socket_channel_impl::read(std::shared_ptr<byte_buffer>& buffer)
{
	buffer->check_writable();
	check_open_connected();
	if (!buffer->has_remaining())
		return 0;
	return read_implement(buffer);
}

int nio::socket_channel_impl::write(const std::shared_ptr<byte_buffer>& buffer) const
{
	check_open_connected();
	if (!buffer->has_remaining())
		return 0;
	return write_implement(buffer);
}

bool nio::socket_channel_impl::query(const char* iid, void** ppv)
{
	void* pv = nullptr;
	if (iid == typeid(socket_channel).name())
		pv = static_cast<socket_channel*>(this);
	else if (iid == typeid(abstract_selectable_channel).name())
		pv = static_cast<abstract_selectable_channel*>(this);
	else if (iid == typeid(selectable_channel).name())
		pv = static_cast<selectable_channel*>(this);
	else if (iid == typeid(abstract_interruptible_channel).name())
		pv = static_cast<abstract_interruptible_channel*>(this);
	else if (iid == typeid(channel).name())
		pv = static_cast<socket_channel*>(this);
	else if (iid == typeid(unknown).name())
		pv = static_cast<socket_channel*>(this);
	else
		return false;
	if (ppv != nullptr)
		*ppv = pv;
	return true;
}

std::shared_ptr<net::net_address> nio::socket_channel_impl::get_local_address(void) const
{
	return is_bound_ ? local_address_ : net::net4_address::ANY;
}

std::shared_ptr<net::socket> nio::socket_channel_impl::socket() const
{
	if (!socket_->has_impl())
	{
		std::shared_ptr<net::net_address> addr = nullptr;
		int port = 0;
		if (connect_address_ != nullptr)
		{
			addr = connect_address_->get_address();
			port = connect_address_->get_port();
		}
		socket_->set_impl(
			net::default_socket_impl::of(sockfd_, local_port_, addr, port));
	}
	return socket_;
}

bool nio::socket_channel_impl::is_bound(void) const
{
	return is_bound_;
}

bool nio::socket_channel_impl::is_connected(void) const
{
	return status_ == socket_status_connected;
}

bool nio::socket_channel_impl::is_connection_pending(void) const
{
	return status_ == socket_status_pending;
}

bool nio::socket_channel_impl::connect(const net::socket_address& address)
{
	check_unconnected();
	std::shared_ptr<net::net_address> addr = address.get_address();
	int port = address.get_port();
	if (addr->is_any_local_address())
		addr = net::net_address::get_local_host();
	bool finished = false;
	if (is_blocking())
		begin();
	try {
		connect(sockfd_, addr, port);
		finished = true;
	}
	catch (const sio::errno_exception& e)
	{
		if (sio::inprogress(e))
			status_ = socket_status_pending;
		else
		{
			if (is_open())
			{
				close();
				finished = true;
			}
		}
	}
	if (is_blocking())
		end(finished);
	init_local_address_and_port();
	connect_address_ = std::make_shared<net::socket_address>(
		address.get_address(), address.get_port());
	if (socket_ != nullptr)
	{
		socket_->get_impl()->set_address(connect_address_->get_address());
		socket_->get_impl()->set_port(connect_address_->get_port());
	}
	if (is_blocking())
		status_ = finished ? socket_status_connected : socket_status_unconnected;
	else
		status_ = socket_status_pending;
	is_bound_ = finished;
	return finished;
}

bool nio::socket_channel_impl::finish_connect(void)
{
	if (!is_open())
		throw closed_channel_exception();
	if (status_ == socket_status_connected)
		return true;
	if (status_ != socket_status_pending)
		throw no_connection_pending_exception();
	bool finished = false;
	begin();
	try
	{
		std::shared_ptr<net::net_address> addr = connect_address_->get_address();
		int port = connect_address_->get_port();
		finished = connected(sockfd_, addr, port);
	}
	catch (const sio::errno_exception&)
	{
		if (is_open())
		{
			close();
			finished = true;
		}
	}
	end(finished);
	status_ = finished ? socket_status_connected : status_;
	is_bound_ = finished;
	return finished;
}

void nio::socket_channel_impl::set_connected(void)
{
	status_ = socket_status_connected;
}

void nio::socket_channel_impl::set_bound(bool flag)
{
	is_bound_ = flag;
}

void nio::socket_channel_impl::finish_accept(sio::socket_t sockfd)
{
	sockfd_ = sockfd;
	init_local_address_and_port();
}

void nio::socket_channel_impl::init_local_address_and_port(void)
{
	sio::sock_addr4 sa;
	int salen = sizeof(sa);
	sio::getsockname(sockfd_, sa, &salen);
	local_address_ = std::make_shared<net::net4_address>(sa.get_addr(), "");
	local_port_ = sa.get_port();
	if (socket_ != nullptr)
		socket_->get_impl()->set_local_port(local_port_);
}

void nio::socket_channel_impl::check_open_connected(void) const
{
	if (!is_open())
		throw closed_channel_exception();
	if (!is_connected())
		throw not_yet_bound_exception();
}

void nio::socket_channel_impl::check_unconnected(void) const
{
	if (!is_open())
		throw closed_channel_exception();
	if (status_ == socket_status_connected)
		throw not_yet_bound_exception();
	if (status_ == socket_status_pending)
		throw connection_pending_exception();
}

int nio::socket_channel_impl::read_implement(std::shared_ptr<byte_buffer>& buffer)
{
	if (is_blocking())
		begin();
	int read_count = sio::recv(sockfd_, 
		const_cast<uint8_t *>(buffer->array() + buffer->position()),
		buffer->remaining(), 0);
	if (read_count > 0)
		buffer->position(buffer->position() + read_count);
	if (is_blocking())
		end(read_count > 0);
	return read_count;
}

int nio::socket_channel_impl::write_implement(const std::shared_ptr<byte_buffer>& buffer) const
{
	if (is_blocking())
		begin();
	int write_count = sio::send(sockfd_,
		const_cast<uint8_t *>(buffer->array() + buffer->position()),
		buffer->remaining(), 0);
	if (write_count > 0)
		buffer->position(buffer->position() + write_count);
	if (is_blocking())
		end(write_count > 0);
	return write_count;
}

sio::socket_t nio::socket_channel_impl::newsocket(void)
{
	return sio::socket(AF_INET, SOCK_STREAM, 0);
}

void nio::socket_channel_impl::connect(const sio::socket_t& sockfd,
	const std::shared_ptr<net::net_address>& addr, const std::uint16_t& port)
{
	if (addr->get_family() == AF_INET) {
		sio::sock_addr4 sa(addr->get_address(), port);
		sio::connect(sockfd, sa, sizeof(sa));
	}
	else if (addr->get_family() == AF_INET6) {
		sio::sock_addr6 sa(addr->get_address(), port);
		sio::connect(sockfd, sa, sizeof(sa));
	}
}

bool nio::socket_channel_impl::connected(const sio::socket_t& sockfd, 
	const std::shared_ptr<net::net_address>& addr, const std::uint16_t& port)
{
	try {
		sio::reactor sr;
		sr.select(sockfd, sio::reactor::out);
		sio::reactor::event ev[1], *pev = ev;
		int rc = sr.poll(pev, 1, 0);
		if (rc == 0)
			return false;
		int error;
		int size = sizeof(error);
		sio::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &size);
		if (error == 0)
			return true;
		throw sio::errno_exception("connected", sio::socket_errno(error));
	}
	catch (const sio::errno_exception& e) {
		if (e.get_errno() == EINTR)
			return false;
		throw e;
	}
	return true;
}