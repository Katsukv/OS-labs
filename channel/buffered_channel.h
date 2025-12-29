#ifndef BUFFERED_CHANNEL_H
#define BUFFERED_CHANNEL_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <utility>

template <typename T>
class BufferedChannel {
public:
    explicit BufferedChannel(int buffer_size)
        : buffer_size_(buffer_size), is_closed_(false) {
        if (buffer_size <= 0) {
            throw std::runtime_error("Buffer size must be positive");
        }
    }

    void Send(T value) {
        std::unique_lock<std::mutex> lock(mutex_);
        send_cv_.wait(lock, [this]() {
            return queue_.size() < buffer_size_ || is_closed_;
        });

        if (is_closed_) {
            throw std::runtime_error("Send on closed channel");
        }

        queue_.push(std::move(value));
        recv_cv_.notify_one();
    }

    std::pair<T, bool> Recv() {
        std::unique_lock<std::mutex> lock(mutex_);
        recv_cv_.wait(lock, [this]() {
            return !queue_.empty() || is_closed_;
        });

        if (queue_.empty() && is_closed_) {
            return {T(), false};
        }

        T value = std::move(queue_.front());
        queue_.pop();
        send_cv_.notify_one();
        return {std::move(value), true};
    }

    void Close() {
        std::lock_guard<std::mutex> lock(mutex_);
        is_closed_ = true;
        send_cv_.notify_all();
        recv_cv_.notify_all();
    }

private:
    const int buffer_size_;
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable send_cv_;
    std::condition_variable recv_cv_;
    bool is_closed_;
};

#endif // BUFFERED_CHANNEL_H