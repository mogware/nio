#ifndef __NIO_CHANNEL__
#define __NIO_CHANNEL__

#include "nio.unknown.h"

namespace nio
{
	struct channel : public unknown
	{
		virtual bool is_open() const = 0;
		virtual void close() = 0;
		virtual ~channel() {}
	};
}

#endif
