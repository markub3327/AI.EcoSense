#pragma once

#include "messageInterface.hpp"

template<typename T>
class MessageQueue final : public MessageInterface<T> {
    static constexpr uint8_t MESSAGE_CAPACITY = 16;

    T _data[MESSAGE_CAPACITY];
    uint8_t _size, _head;

    // used for parallel safety operations
    bool isFull() const { return this->_size == MESSAGE_CAPACITY; }
    bool isEmpty() const { return this->_size == 0; }

public:
    MessageQueue() : _size(0), _head(0) {}
    ~MessageQueue() override = default;

    bool send(const T& data) override {
        // lock the writing to the message (mutex)
        if (this->isFull()) return false;
        this->_data[this->_size] = data;
        this->_size += 1;
        return true;
    }

    bool read(T& data) override {
        // lock the reading from a message (mutex)
        if (this->isEmpty()) return false;
        data = this->_data[this->_head];
        this->_head = (this->_head + 1) % MESSAGE_CAPACITY;
        this->_size -= 1;
        return true;
    }
};