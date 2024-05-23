#pragma once

#include <utility>
#include "Utils.hpp"


class Client {
public:

    Client() : status_(ClientStatus::UNKNOWN), name_(), start_sit_time_() {}

    Client(std::string name, ClientStatus status) : status_(status), name_(std::move(name)), start_sit_time_() {}

    Client &operator=(const Client &other) {
        if (this != &other) {
            status_ = other.status_;
            name_ = other.name_;
            start_sit_time_ = other.start_sit_time_;
        }
        return *this;
    }

    ClientStatus get_status() const {
        return status_;
    }

    void set_status(ClientStatus status) {
        status_ = status;
    }

    std::string get_name() const {
        return name_;
    }

    std::string get_start_sit_time() const {
        return start_sit_time_;
    }

    void set_start_sit_time(const std::string &time) {
        start_sit_time_ = time;
    }

    friend bool operator==(const Client &lhs, const Client &rhs);

    friend bool operator<(const Client &lhs, const Client &rhs);

private:
    ClientStatus status_;
    std::string name_;
    std::string start_sit_time_;
};

bool operator<(const Client &lhs, const Client &rhs) {
    return lhs.get_name() < rhs.get_name();
}

bool operator==(const Client &lhs, const Client &rhs) {
    return lhs.name_ == rhs.name_;
}
