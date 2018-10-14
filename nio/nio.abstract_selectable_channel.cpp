#include "nio.abstract_selectable_channel.h"
#include "nio.abstract_selector.h"
#include "nio.selection_key.h"
#include "nio.exceptions.h"

nio::abstract_selectable_channel::abstract_selectable_channel(
	const std::shared_ptr<nio::selector_provider>& provider)
	: provider_(provider)
	, is_blocking_(true)
	, key_list_()
{
}

nio::abstract_selectable_channel::~abstract_selectable_channel()
{
}

std::shared_ptr<nio::selector_provider> nio::abstract_selectable_channel::provider(void) const
{
	return provider_;
}

bool nio::abstract_selectable_channel::is_enrolled(void) const
{
	return !key_list_.empty();
}

std::shared_ptr<nio::selection_key> nio::abstract_selectable_channel::key_for(
	const std::shared_ptr<nio::selector>& sel) const
{
	for (auto it = key_list_.begin(); it != key_list_.end(); ++it)
	{
		std::shared_ptr<selection_key> key = *it;
		if (key != nullptr && key->selector() == sel)
			return key;
	}
	return nullptr;
}

std::shared_ptr<nio::selection_key> nio::abstract_selectable_channel::enroll(
	const std::shared_ptr<nio::selector>& sel, const int& ops,
	const std::shared_ptr<unknown>& attachment)
{
	if (!is_open())
		throw closed_channel_exception();
	if (!((ops & ~valid_ops()) == 0))
		throw std::invalid_argument("no valid ops in interest set: " + ops);
	if (is_blocking())
		throw illegal_blocking_mode_exception();
	if (!sel->is_open())
	{
		if (ops == 0)
			throw illegal_selector_exception();
		throw std::runtime_error("selector not open");
	}
	std::shared_ptr<nio::selection_key> key = key_for(sel);
	if (key == nullptr)
	{
		abstract_selector* as = nullptr;
		if (sel->query(IID_PPV_ARG(abstract_selector, &as)))
			key = as->enroll(shared_from_this(), ops, attachment);
		key_list_.push_back(key);
	}
	else if (! key->is_valid())
		throw cancelled_key_exception();
	else
	{
		key->interest_ops(ops);
		key->attach(attachment);
	}
	return key;
}

bool nio::abstract_selectable_channel::is_blocking(void) const
{
	return is_blocking_;
}

void nio::abstract_selectable_channel::configure_blocking(const bool& block)
{
	if (!is_open())
		throw closed_channel_exception();
	if (is_blocking_ == block)
		return;
	if (block && contains_valid_keys())
		throw illegal_blocking_mode_exception();
	implement_blocking(block);
	is_blocking_ = block;
}

void nio::abstract_selectable_channel::disenroll(const std::shared_ptr<selection_key>& key)
{
	key_list_.remove(key);
}

bool nio::abstract_selectable_channel::contains_valid_keys(void) const
{
	for (auto it = key_list_.begin(); it != key_list_.end(); ++it)
	{
		std::shared_ptr<selection_key> key = *it;
		if (key != nullptr && key->is_valid())
			return true;
	}
	return false;
}

void nio::abstract_selectable_channel::implement_close_channel(void)
{
	implement_close_selectable_channel();
	for (auto it = key_list_.begin(); it != key_list_.end(); ++it)
	{
		std::shared_ptr<selection_key> key = *it;
		if (key != nullptr)
			key->cancel();
	}
}