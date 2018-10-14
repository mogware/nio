#ifndef __NIO_SELECTOR__
#define __NIO_SELECTOR__

#include <memory>
#include <set>

#include "nio.unknown.h"

namespace nio
{
	class selection_key;
	struct selector_provider;
	struct selector : public unknown
	{
		virtual void close(void) = 0;
		virtual bool is_open(void) const = 0;
		virtual const std::set<std::shared_ptr<selection_key>>& keys(void) const = 0;
		virtual std::shared_ptr<selector_provider> provider(void) const = 0;
		virtual int select(void) = 0;
		virtual int select(const long& timeout) = 0;
		virtual std::set<std::shared_ptr<selection_key>>& selected_keys(void) = 0;
		virtual int select_now(void) = 0;
		virtual void wakeup(void) const = 0;
		static std::shared_ptr<selector> open(void);
		virtual ~selector();
	};
}

#endif



