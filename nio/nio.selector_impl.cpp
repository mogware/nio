#include "nio.selector_impl.h"
#include "nio.abstract_selectable_channel.h"
#include "nio.selection_key_impl.h"
#include "nio.server_socket_channel.h"
#include "nio.socket_channel.h"
#include "nio.exceptions.h"

#include <map>

nio::selector_impl::selector_impl(
		const std::shared_ptr<selector_provider>& provider)
	: abstract_selector(provider)
	, keys_()
	, selected_keys_()
	, wakeup_in(sio::invalid_socket)
	, wakeup_out(sio::invalid_socket)
{
	sio::socket_t sfds[2];
	sio::socketpair(AF_LOCAL, SOCK_DGRAM, 0, sfds);
	wakeup_out = sfds[0];
	wakeup_in = sfds[1];
	int on = 1;
	sio::ioctlsocket(wakeup_in, sio::sio_nbio, &on);
}

nio::selector_impl::~selector_impl()
{
	close();
}

const std::set<std::shared_ptr<nio::selection_key>>&
nio::selector_impl::keys(void) const
{
	return keys_;
}

int nio::selector_impl::select(void)
{
	return select_internal(-1);
}

int nio::selector_impl::select(const long& timeout)
{
	if (timeout < 0)
		throw std::invalid_argument("timeout < 0: " + timeout);
	return select_internal((timeout == 0) ? -1 : timeout);
}

std::set<std::shared_ptr<nio::selection_key>>&
nio::selector_impl::selected_keys(void)
{
	return selected_keys_;
}

int nio::selector_impl::select_now(void)
{
	return select_internal(0);
}

void nio::selector_impl::wakeup(void) const
{
	try
	{
		std::uint8_t wakeup = 1;
		sio::send(wakeup_out, &wakeup, sizeof(wakeup), 0);
	}
	catch (const sio::errno_exception&)
	{
	}
}

std::shared_ptr<nio::selection_key> nio::selector_impl::enroll(
	const std::shared_ptr<abstract_selectable_channel>& ch,
	const int& ops, const std::shared_ptr<unknown>& attachment)
{
	if (provider() != ch->provider())
		return nullptr;
	std::shared_ptr<selection_key> key =
		selection_key_impl::of(ch, ops, attachment, shared_from_this());
	keys_.insert(key);
	return key;
}

void nio::selector_impl::implement_close_selector(void)
{
	wakeup();
	sio::closesocket(wakeup_in);
	sio::closesocket(wakeup_out);
	do_cancel();
	for (auto it = keys_.begin(); it != keys_.end(); ++it)
	{
		std::shared_ptr<selection_key> key = *it;
		abstract_selection_key* ask = nullptr;
		if (key->query(IID_PPV_ARG(abstract_selection_key, &ask)))
			disenroll(std::shared_ptr<abstract_selection_key>(key, ask));
	}
}

bool nio::selector_impl::query(const char* iid, void** ppv)
{
	void* pv = nullptr;
	if (iid == typeid(abstract_selector).name())
		pv = static_cast<abstract_selector*>(this);
	else if (iid == typeid(selector).name())
		pv = static_cast<selector*>(this);
	else if (iid == typeid(unknown).name())
		pv = static_cast<selector*>(this);
	else
		return false;
	if (ppv != nullptr)
		*ppv = pv;
	return true;
}

void nio::selector_impl::check_closed(void) const
{
	if (!is_open())
		throw closed_channel_exception();
}

int nio::selector_impl::select_internal(const long& timeout)
{
	check_closed();
	do_cancel();
	bool is_blocking = timeout != 0;

	if (is_blocking)
		begin();
	int ready_count = 0;
	try {
		ready_count = poll(timeout);
	}
	catch (const sio::errno_exception& e)
	{
		if (e.get_errno() != EINTR)
			throw e;
	}
	if (is_blocking)
		end();
	ready_count -= do_cancel();
	return ready_count;
}

int nio::selector_impl::do_cancel(void)
{
	int deselected = 0;

	std::set<std::shared_ptr<selection_key>>& keys = cancelled_keys();
	if (keys.size() > 0)
	{
		for (auto it = keys.begin(); it != keys.end(); ++it)
		{
			std::shared_ptr<selection_key> key = *it;
			keys_.erase(key);
			abstract_selection_key* ask = nullptr;
			if (key->query(IID_PPV_ARG(abstract_selection_key, &ask)))
				disenroll(std::shared_ptr<abstract_selection_key>(key, ask));
			if (selected_keys_.erase(key) != 0)
				deselected++;
		}
		keys.clear();
	}
	return deselected;
}

int nio::selector_impl::poll(const long& timeout)
{
	std::map<sio::socket_t, std::shared_ptr<selection_key>> pollmap;
	sio::reactor sr;
	sr.select(wakeup_in, sio::reactor::in);

	for (auto it = keys_.begin(); it != keys_.end(); ++it)
	{
		sio::socket_t sockfd = get_native_socket((*it)->channel());
		if (sockfd == sio::invalid_socket)
			continue;

		int ops = (*it)->interest_ops_nocheck();
		int mask = 0;
		if (((selection_key::op_accept | selection_key::op_read) & ops) != 0)
			mask |= sio::reactor::in;
		if (((selection_key::op_connect | selection_key::op_write) & ops) != 0)
			mask |= sio::reactor::out;
		if (mask == 0)
			continue;

		sr.select(sockfd, mask);
		pollmap.insert(std::pair<sio::socket_t,
			std::shared_ptr<selection_key>>(sockfd, *it));
	}

	sio::reactor::event ev[32], *pev = ev;
	int nevents = sr.poll(pev, 32, timeout);

	int ready_count = 0;
	for (int i = 0; i < nevents; ++i)
	{
		if (ev[i].events == 0)
			continue;

		if (ev[i].sockfd == wakeup_in)
		{
			while (true)
			{
				try
				{
					std::uint8_t wakeup;
					if (sio::recv(wakeup_in, &wakeup, sizeof(wakeup), 0) < 1)
						break;
				}
				catch (const sio::errno_exception&)
				{
					break;
				}
			}
			continue;
		}
		
		std::shared_ptr<selection_key> key = pollmap.at(ev[i].sockfd);
		if (key == nullptr)
			continue;

		int ops = key->interest_ops_nocheck();
		int selected_ops = 0;
		if ((ev[i].events & sio::reactor::err) != 0)
			selected_ops |= ops;
		if ((ev[i].events & sio::reactor::in) != 0)
			selected_ops |= ops & (selection_key::op_accept | selection_key::op_read);
		if ((ev[i].events & sio::reactor::out) != 0)
		{
			if (key->is_connected())
				selected_ops |= ops & selection_key::op_write;
			else
				selected_ops |= ops & selection_key::op_connect;
		}
		if (selected_ops != 0)
		{
			bool selected = selected_keys_.find(key) != selected_keys_.end();
			if (selected && key->ready_ops() != selected_ops)
			{
				key->ready_ops(key->ready_ops() | selected_ops);
				ready_count++;
			}
			else if (!selected)
			{
				key->ready_ops(selected_ops);
				selected_keys_.insert(key);
				ready_count++;
			}
		}
	}
	return ready_count;
}

sio::socket_t nio::selector_impl::get_native_socket(const std::shared_ptr<selectable_channel>& ch)
{
	socket_channel* sc = nullptr;
	if (ch->query(IID_PPV_ARG(socket_channel, &sc)))
		return sc->socket()->get_impl()->get_native_socket();
	server_socket_channel* ssc = nullptr;
	if (ch->query(IID_PPV_ARG(server_socket_channel, &ssc)))
		return ssc->socket()->get_impl()->get_native_socket();
	return sio::invalid_socket;
}

