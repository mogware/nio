#include "nio.selection_key_impl.h"
#include "nio.selectable_channel.h"
#include "nio.selector_impl.h"
#include "nio.socket_channel.h"
#include "nio.exceptions.h"

nio::selection_key_impl::selection_key_impl(const std::shared_ptr<selectable_channel>& channel,
	const int& operations, const std::shared_ptr<unknown>& object,
	const std::shared_ptr<selector_impl> selector)
	: channel_(channel)
	, interest_ops_(operations)
	, ready_ops_()
	, selector_(selector)
{
	attach(object);
}

nio::selection_key_impl::~selection_key_impl(void)
{
}

std::shared_ptr<nio::selection_key> nio::selection_key_impl::of(
	const std::shared_ptr<nio::selectable_channel>& channel,
	const int& operations, const std::shared_ptr<nio::unknown>& object,
	const std::shared_ptr<nio::selector_impl> selector)
{
	return std::make_shared<selection_key_impl>(channel, operations, object, selector);
}

std::shared_ptr<nio::selectable_channel> nio::selection_key_impl::channel(void) const
{
	return channel_;
}

int nio::selection_key_impl::interest_ops(void) const
{
	check_valid();
	return interest_ops_;
}

int nio::selection_key_impl::interest_ops_nocheck(void) const
{
	return interest_ops_;
}

void nio::selection_key_impl::interest_ops(const int& ops)
{
	check_valid();
	if ((ops & ~(channel()->valid_ops())) != 0)
		throw std::invalid_argument("no valid ops in interest set: " + ops);
	interest_ops_ = ops;
}

int nio::selection_key_impl::ready_ops(void) const
{
	check_valid();
	return ready_ops_;
}

void nio::selection_key_impl::ready_ops(const int& ops)
{
	ready_ops_ = ops & interest_ops_;
}

std::shared_ptr<nio::selector> nio::selection_key_impl::selector(void) const
{
	return selector_;
}

bool nio::selection_key_impl::query(const char* iid, void** ppv)
{
	void* pv = nullptr;
	if (iid == typeid(abstract_selection_key).name())
		pv = static_cast<abstract_selection_key*>(this);
	else if (iid == typeid(selection_key).name())
		pv = static_cast<selection_key*>(this);
	else if (iid == typeid(unknown).name())
		pv = static_cast<selection_key*>(this);
	else
		return false;
	if (ppv != nullptr)
		*ppv = pv;
	return true;
}

bool nio::selection_key_impl::is_connected(void) const
{
	socket_channel* sc = nullptr;
	if (channel_->query(IID_PPV_ARG(socket_channel, &sc)))
		return sc->is_connected();
	return true;
}

void nio::selection_key_impl::check_valid() const
{
	if (!is_valid())
		throw cancelled_key_exception();
}
