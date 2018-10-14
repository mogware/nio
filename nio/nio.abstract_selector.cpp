#include "nio.abstract_selector.h"
#include "nio.selectable_channel.h"
#include "nio.abstract_selectable_channel.h"
#include "nio.abstract_selection_key.h"

nio::abstract_selector::abstract_selector(
		const std::shared_ptr<selector_provider>& provider)
	: is_open_(true)
	, provider_(provider)
{
}

nio::abstract_selector::~abstract_selector(void)
{
}

void nio::abstract_selector::close(void)
{
	if (is_open_)
	{
		is_open_ = false;
		implement_close_selector();
	}
}

bool nio::abstract_selector::is_open(void) const
{
	return is_open_;
}

std::shared_ptr<nio::selector_provider> nio::abstract_selector::provider(void) const
{
	return provider_;
}

std::set<std::shared_ptr<nio::selection_key>>& nio::abstract_selector::cancelled_keys(void)
{
	return cancelled_keys_;
}

void nio::abstract_selector::disenroll(const std::shared_ptr<abstract_selection_key>& key)
{
	std::shared_ptr<selectable_channel> ch = key->channel();
	abstract_selectable_channel* asc = nullptr;
	if (ch->query(IID_PPV_ARG(abstract_selectable_channel, &asc)))
		asc->disenroll(key);
	key->is_valid_ = false;
}

void nio::abstract_selector::begin(void) const
{
}

void nio::abstract_selector::end(void) const
{
}

void nio::abstract_selector::cancel(const std::shared_ptr<nio::selection_key>& key)
{
	cancelled_keys_.insert(key);
}
