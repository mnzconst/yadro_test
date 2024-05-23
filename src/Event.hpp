#pragma once

#include <sstream>
#include "Utils.hpp"

class Event {
public:

    Event() = default;

    Event(std::string t, EventId i, std::string ms)
            : time_(std::move(t)), id_(i), message_(std::move(ms)) {}

    Event(std::string t, EventId i, const Client &cl, int table)
            : time_(std::move(t)), id_(i), client_(cl), table_number_(table) {}

    Event(std::string t, EventId i, const Client &cl, int table, bool is_valid)
            : time_(std::move(t)), id_(i), client_(cl), table_number_(table), is_valid_(is_valid) {}

    bool is_valid() const {
        return is_valid_;
    }

    std::string get_time() const {
        return time_;
    }

    EventId get_id() const {
        return id_;
    }

    Client get_client() const {
        return client_;
    }

    Client &get_client() {
        return client_;
    }

    int get_table_number() const {
        return table_number_;
    }

    ClubId get_club_id() const {
        return club_id_;
    }

    friend std::ostream &operator<<(std::ostream &os, const Event &event);

private:
    ClubId club_id_ = 1;
    std::string time_;
    EventId id_;
    Client client_;
    int table_number_ = -1;
    bool is_valid_ = true;
    std::string message_;
};

static bool check_params(int id, const std::string &time, const std::string &client_name) {
    return id > 0
           && id < 5
           && is_valid_time(time)
           && is_valid_name(client_name);
}

std::ostream &operator<<(std::ostream &os, const Event &event) {
    os << event.time_ << " " << event.id_ << " ";
    if (event.message_.empty()) {
        os << event.client_.get_name();
        if (event.table_number_ != -1) {
            os << " " << event.table_number_;
        }
    } else {
        os << event.message_;
    }
    return os;
}


