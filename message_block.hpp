#pragma once
#include <assert.h>
#include <string.h>
#include <string>
#include <memory>
struct message_block_buffer {
	const char * data;
	size_t len;
};
class message_block : public std::enable_shared_from_this<message_block> {
	std::shared_ptr<char> buffer_base_ptr_;
	const std::size_t const_buffer_size_;
	char *wr_ptr_;
	char *rd_ptr_;
public:
	message_block(const std::size_t length)
    : buffer_base_ptr_(std::shared_ptr<char>(new char[length]))
    , const_buffer_size_(length) {
        assert(length);
        assert(&*buffer_base_ptr_);
        wr_ptr_ = rd_ptr_ = &*buffer_base_ptr_;
    }
	message_block(const message_block &b)
    : buffer_base_ptr_(b.buffer_base_ptr_)
    , const_buffer_size_(b.const_buffer_size_)
    , wr_ptr_(b.wr_ptr_)
    , rd_ptr_(b.rd_ptr_) {}
public:
	virtual ~message_block() {}
public:
	template<typename T>
    static std::shared_ptr<message_block> from_type(const T &t) {
        auto mb(std::make_shared<message_block>(sizeof(T)));
        *mb << t;
        return mb;
    }
	static std::shared_ptr<message_block> from_size(const std::size_t s) {
		return std::make_shared<message_block>(s);
	}
public:
	std::shared_ptr<message_block> clone() const {
		return std::make_shared<message_block>(*this);
	}
	std::shared_ptr<message_block> clone_deep() const {
        auto mb(std::make_shared<message_block>(const_buffer_size_));
		mb->copy(rd_ptr(), length());
		return mb;
	}
public:
	const std::size_t capacity_max() const { return const_buffer_size_; }
	const std::size_t capacity() const { return &*buffer_base_ptr_ + const_buffer_size_ - wr_ptr_; }
	const std::size_t length() const { return (wr_ptr_ - rd_ptr_); }
	bool empty() const { return (wr_ptr_ == rd_ptr_); }
	const char* rd_ptr() const { return rd_ptr_; }
	char* wr_ptr() const { return wr_ptr_; }
public:
	template<typename T>
    T& reference() const { assert(length() >= sizeof(T)); return *(T*)(rd_ptr()); }
public:
	void rd_ptr(const std::size_t n) {
		assert(rd_ptr_ + n <= wr_ptr_);
		rd_ptr_ += n;
	}
	void wr_ptr(const std::size_t n) {
		assert(wr_ptr_ + n <= &*buffer_base_ptr_ + const_buffer_size_);
		wr_ptr_ += n;
	}
	void wr_ptr_rewind(const std::size_t n) {
		assert(wr_ptr_ - n >= rd_ptr_);
		wr_ptr_ -= n;
	}
	void reset() { rd_ptr_ = wr_ptr_ = &*buffer_base_ptr_; }
	void crunch() {
		assert(wr_ptr_ >= rd_ptr_);
		if (empty()) {
			reset();
		} else {
			if (rd_ptr_ != &*buffer_base_ptr_) {
				const auto len = length();
				memmove(&*buffer_base_ptr_, rd_ptr_, len);
				rd_ptr_ = &*buffer_base_ptr_;
				wr_ptr_ = &*buffer_base_ptr_ + len;
			}
		}
	}
public:
	message_block& copy(const char* data, const std::size_t len) {
        assert((wr_ptr_ + len) <= (const_buffer_size_ + &*buffer_base_ptr_));
		if (len) {
			memmove(wr_ptr_, data, len);
			wr_ptr_ += len;
		}
		return *this;
	}
public:
    message_block& operator<<(std::string &data) { return copy(data.data(), data.length()); }
	message_block& operator<<(const std::string &data) { return copy(data.data(), data.length()); }
    message_block& operator<<(message_block &data) { return copy(data.rd_ptr(), data.length()); }
    message_block& operator<<(const message_block &data) { return copy(data.rd_ptr(), data.length()); }
    message_block& operator<<(message_block_buffer &mbb) { return copy(mbb.data, mbb.len); }
    message_block& operator<<(const message_block_buffer &mbb) { return copy(mbb.data, mbb.len); }
public:
    message_block& operator>>(std::string &data) {
        data = std::string(rd_ptr_);
        rd_ptr(strlen(rd_ptr_) + 1);
        return *this;
    }
private:
    message_block& operator<<(const char *data);
    message_block& operator<<(char *data);
public:
	template<typename T>
    message_block& operator<<(const T &object) {
        copy(reinterpret_cast<const char*>(&object), sizeof(T));
        return *this;
    }
	template<typename T>
    message_block& operator>>(T &object) {
        assert((rd_ptr_ + sizeof(T)) <= wr_ptr_);
        memcpy(&object, rd_ptr_, sizeof(T));
        rd_ptr_ += sizeof(T);
        return *this;
    }
};
template<typename T>
std::shared_ptr<message_block> operator<<(std::shared_ptr<message_block> m, const T &t) {
	*m << t;
	return m;
}
template<typename T>
std::shared_ptr<message_block> operator>>(std::shared_ptr<message_block> m, const T &t) {
	*m >> t;
	return m;
}
