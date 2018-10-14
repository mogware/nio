#ifndef __NIO_ABSTRACT_SELECTABLE_CHANNEL__
#define __NIO_ABSTRACT_SELECTABLE_CHANNEL__

#include <memory>
#include <list>

#include "nio.selectable_channel.h"
#include "nio.unknown.h"

namespace nio
{
	struct selector_provider;
	class abstract_selectable_channel : public selectable_channel,
		public std::enable_shared_from_this<abstract_selectable_channel>
	{
		std::shared_ptr<selector_provider> provider_;
		volatile bool is_blocking_;
		std::list<std::shared_ptr<selection_key>> key_list_;
	public:
		abstract_selectable_channel(const std::shared_ptr<selector_provider>& provider);
		virtual ~abstract_selectable_channel();
	public:
		std::shared_ptr<selector_provider> provider(void) const;
		void configure_blocking(const bool& block);
		bool is_blocking(void) const;
		bool is_enrolled(void) const;
		std::shared_ptr<selection_key> key_for(
			const std::shared_ptr<selector>& sel) const;
		std::shared_ptr<selection_key> enroll(
			const std::shared_ptr<selector>& sel, const int& ops,
			const std::shared_ptr<unknown>& attachment);
	public:
		void disenroll(const std::shared_ptr<selection_key>& key);
	protected:
		virtual void implement_blocking(const bool& block) = 0;
		virtual void implement_close_selectable_channel(void) = 0;
	protected:
		void implement_close_channel(void);
	private:
		bool contains_valid_keys(void) const;
	};
}

#endif
