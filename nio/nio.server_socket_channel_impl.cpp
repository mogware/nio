#include "nio.server_socket_channel_impl.h"
#include "nio.server_socket_adapter.h"
#include "nio.socket_channel_impl.h"
#include "nio.socket_adapter.h"
#include "nio.exceptions.h"

nio::server_socket_channel_impl::server_socket_channel_impl(
	const std::shared_ptr<nio::selector_provider>& provider)
	: server_socket_channel(provider)
	, socket_(nullptr)
	, is_bound_(false)
{
}

nio::server_socket_channel_impl::~server_socket_channel_impl()
{
	close();
}

void nio::server_socket_channel_impl::set_socket_adapter(
	const std::shared_ptr<server_socket_adapter>& adapter)
{
	socket_ = adapter;
}

void nio::server_socket_channel_impl::implement_blocking(const bool& block)
{
	int on = block ? 0 : 1;
	sio::socket_t sockfd = socket_->get_impl()->get_native_socket();
	sio::ioctlsocket(sockfd, sio::sio_nbio, &on);
}

void nio::server_socket_channel_impl::implement_close_selectable_channel(void)
{
	if (!socket_->is_closed())
		socket_->close();
}

bool nio::server_socket_channel_impl::is_bound() const
{
	return is_bound_;
}

bool nio::server_socket_channel_impl::query(const char* iid, void** ppv)
{
	void* pv = nullptr;
	if (iid == typeid(server_socket_channel).name())
		pv = static_cast<server_socket_channel*>(this);
	else if (iid == typeid(selectable_channel).name())
		pv = static_cast<selectable_channel*>(this);
	else if (iid == typeid(channel).name())
		pv = static_cast<channel*>(this);
	else if (iid == typeid(unknown).name())
		pv = static_cast<server_socket_channel*>(this);
	else
		return false;
	if (ppv != nullptr)
		*ppv = pv;
	return true;
}

std::shared_ptr<net::server_socket> nio::server_socket_channel_impl::socket() const
{
	return socket_;
}

std::shared_ptr<nio::socket_channel> nio::server_socket_channel_impl::accept()
{

	if (!is_open())
		throw closed_channel_exception();
	if (!is_bound())
		throw not_yet_bound_exception();

	std::shared_ptr<socket_channel_impl> channel =
		std::make_shared<socket_channel_impl>(provider(), false);
	std::shared_ptr<socket_adapter> adapter =
		std::make_shared<socket_adapter>(nullptr, channel);
	channel->set_socket_adapter(adapter);

	begin();
	socket_->implement_accept(channel);
	end(channel->socket()->is_connected());

	return channel->socket()->is_connected() ? channel : nullptr;
}

