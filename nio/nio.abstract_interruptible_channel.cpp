#include "nio.abstract_interruptible_channel.h"

nio::abstract_interruptible_channel::abstract_interruptible_channel()
	: is_closed_(false)
	, is_interrupted_(false)
{
}

nio::abstract_interruptible_channel::~abstract_interruptible_channel()
{
}

bool nio::abstract_interruptible_channel::is_open(void) const
{
	return !is_closed_;
}

void nio::abstract_interruptible_channel::close(void)
{
	if (!is_closed_)
	{
		is_closed_ = true;
		implement_close_channel();
	}
}

void nio::abstract_interruptible_channel::begin(void) const
{
}

void nio::abstract_interruptible_channel::end(bool success) const
{
}
