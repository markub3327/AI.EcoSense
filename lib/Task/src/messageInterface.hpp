//
// Created by Martin Kubovčík on 31/05/2025.
//
#pragma once

template <typename T>
class MessageInterface {
    public:
        virtual ~MessageInterface() = default;

        virtual bool send(const T& data) = 0;
        virtual bool read(T& data) = 0;
};
