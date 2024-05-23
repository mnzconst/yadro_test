#pragma once

#include <string>
#include <algorithm>
#include <unordered_map>

using ClubId = size_t;

enum ClientStatus {
    IN_CLUB = 1,
    IS_SITTING = 2,
    IS_WAITING = 3,
    UNKNOWN = 4,
};

enum EventId {
    IN = 1,
    SIT = 2,
    WAIT = 3,
    OUT = 4,
    GEN_OUT = 11,
    GEN_SIT = 12,
    GEN_ERROR = 13,
};

static const std::unordered_map<EventId, ClientStatus> id_status_mapping = {
        {EventId::IN,      ClientStatus::IN_CLUB},
        {EventId::SIT,     ClientStatus::IS_SITTING},
        {EventId::WAIT,    ClientStatus::IS_WAITING},
        {EventId::OUT,     ClientStatus::UNKNOWN},
        {EventId::GEN_OUT, ClientStatus::UNKNOWN},
        {EventId::GEN_SIT, ClientStatus::IS_SITTING},
};

bool is_valid_time(std::string_view time) {
    if (time.size() != 5) {
        return false;
    }
    return time.substr(0, 2) < "24" && time[2] == ':' && time.substr(3) < "60";
}

bool is_valid_name(std::string name) {
    return std::all_of(name.begin(), name.end(), [](char ch) -> bool {
        return (ch < 'z' && ch > 'a') || (ch > '0' && ch < '9') || ch == '_' || ch == '-';
    });
}

int to_minutes(const std::string &time) {
    return std::stoi(time.substr(0, 2)) * 60 + std::stoi(time.substr(3));
}
