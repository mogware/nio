#ifndef __NIO_ABSTRACT_INTERRUPTIBLE_CHANNEL__
#define __NIO_ABSTRACT_INTERRUPTIBLE_CHANNEL__

#include "nio.channel.h"

namespace nio
{
	class abstract_interruptible_channel : public channel
	{
		volatile bool is_closed_;
		volatile bool is_interrupted_;
	public:
		abstract_interruptible_channel();
		~abstract_interruptible_channel();
	public:
		bool is_open(void) const;
		void close(void);
	public:
		void begin(void) const;
		void end(bool success) const;
		virtual void implement_close_channel(void) = 0;
	};
}

#endif
