#ifndef __NIO_ABSTRACT_SELECTOR__
#define __NIO_ABSTRACT_SELECTOR__

#include <memory>
#include <set>

#include "nio.selector.h"
#include "nio.selector_provider.h"
#include "nio.unknown.h"

namespace nio
{
	class abstract_selectable_channel;
	class abstract_selection_key;
	class abstract_selector : public selector
	{
		std::set<std::shared_ptr<selection_key>> cancelled_keys_;
		std::shared_ptr<selector_provider> provider_;
		volatile bool is_open_;
	public:
		abstract_selector(const std::shared_ptr<selector_provider>& provider);
		virtual ~abstract_selector(void);
	public:
		void close(void);
		bool is_open(void) const;
		std::shared_ptr<selector_provider> provider(void) const;
		std::set<std::shared_ptr<selection_key>>& cancelled_keys(void);
		void cancel(const std::shared_ptr<selection_key>& key);
	public:
		virtual std::shared_ptr<selection_key> enroll(
			const std::shared_ptr<abstract_selectable_channel>& ch,
			const int& ops, const std::shared_ptr<unknown>& attachment) = 0;
	protected:
		void disenroll(const std::shared_ptr<abstract_selection_key>& key);
		void begin(void) const;
		void end(void) const;
	protected:
		virtual void implement_close_selector(void) = 0;
	};
}

#endif
