#ifndef __NIO_SELECTABLE_CHANNEL__
#define __NIO_SELECTABLE_CHANNEL__

#include <memory>

#include "nio.abstract_interruptible_channel.h"

namespace nio
{
	class selection_key;
	struct selector;
	struct selector_provider;
	struct selectable_channel : public abstract_interruptible_channel
	{
		virtual void configure_blocking(const bool& block) = 0;
		virtual bool is_blocking(void) const = 0;
		virtual bool is_enrolled(void) const = 0;
		virtual std::shared_ptr<selection_key> key_for(
			const std::shared_ptr<selector>& sel) const = 0;
		virtual std::shared_ptr<selector_provider> provider(void) const = 0;
		virtual std::shared_ptr<selection_key> enroll(
			const std::shared_ptr<selector>& sel, const int& ops,
			const std::shared_ptr<unknown>& attachment) = 0;
		virtual int valid_ops(void) const = 0;
		virtual ~selectable_channel() {}
	};
}

#endif

