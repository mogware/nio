#ifndef __NIO_BUFFER__
#define __NIO_BUFFER__

namespace nio
{
	class buffer
	{
	protected:
		static const int unset_mark = -1;
		const int capacity_;
		int limit_;
		int mark_;
		int position_;
	public:
		buffer(int capacity);
		virtual ~buffer() {}
	protected:
		void check_index(const int& index);
		void check_index(const int& index, const int& size_of_type);
		int check_get_bounds(const int& bytes_per_element,
			const int& length, const int& offset, const int& count);
		int check_set_bounds(const int& bytes_per_element,
			const int& length, const int& offset, const int& count);
		void check_start_end_remaining(const int& start, const int& end);
	public:
		void check_writable(void);
	public:
		void clear(void);
		void flip(void);
	public:
		int capacity(void) const;
		bool has_remaining(void) const;
		int remaining(void) const;
		int limit(void) const;
		void limit(const int& new_limit);
		void mark(void);
		int position(void) const;
		void position(const int& new_position);
		void reset(void);
		void rewind(void);
	public:
		virtual bool is_read_only(void) const = 0;
	};
}

#endif
