#include "nio.selection_key.h"
#include "nio.selectable_channel.h"
#include "nio.selector.h"

nio::selection_key::selection_key()
{
}

nio::selection_key::~selection_key()
{
}

std::shared_ptr<nio::unknown> nio::selection_key::attach(
	const std::shared_ptr<nio::unknown>& object)
{
	std::shared_ptr<unknown> old_attachment = attachment_;
	attachment_ = object;
	return old_attachment;
}

std::shared_ptr<nio::unknown> nio::selection_key::attachment(void) const
{
	return attachment_;
}

bool nio::selection_key::is_acceptable(void) const
{
	return (ready_ops() & op_accept) == op_accept;
}

bool nio::selection_key::is_connectable(void) const
{
	return (ready_ops() & op_connect) == op_connect;
}

bool nio::selection_key::is_readable(void) const
{
	return (ready_ops() & op_read) == op_read;
}

bool nio::selection_key::is_writable() const
{
	return (ready_ops() & op_write) == op_write;
}