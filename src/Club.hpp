#include <utility>
#include <queue>
#include <unordered_set>
#include <set>
#include "Event.hpp"
#include <optional>

#pragma once

class Club {
public:

    Club(size_t id, size_t nt, std::string ot, std::string ct, std::vector<Table> ts)
            : id_(id), number_of_tables_(nt), number_of_free_tables_(number_of_tables_), open_time_(std::move(ot)),
              close_time_(std::move(ct)), tables_(std::move(ts)) {}

    std::optional<Event> handle_event(const Event &event) {
        if (event.get_club_id() == id_) {
            switch (event.get_id()) {
                case IN: {
                    if (event.get_time() < open_time_) {
                        return Event(event.get_time(), EventId::GEN_ERROR, "NotOpenYet");
                    }
                    if (is_client_in_club(event.get_client())) {
                        return Event(event.get_time(), EventId::GEN_ERROR, "YouShallNotPass");
                    }
                    add_client(event.get_client());
                    break;
                }
                case SIT: {
                    if (!is_client_in_club(event.get_client())) {
                        return Event(event.get_time(), EventId::GEN_ERROR, "ClientUnknown");
                    }
                    if (!is_table_free(event.get_table_number())) {
                        return Event(event.get_time(), EventId::GEN_ERROR, "PlaceIsBusy");
                    }
                    sit_client(event.get_client(), event.get_time(), event.get_table_number());
                    break;
                }
                case WAIT: {
                    if (number_of_free_tables_ != 0) {
                        return Event(event.get_time(), EventId::GEN_ERROR, "ICanWaitNoLonger!");
                    }
                    if (number_of_free_tables_ > clients_queue_.size()) {
                        return Event(event.get_time(), EventId::GEN_OUT, event.get_client().get_name());
                    }
                    add_to_queue(event.get_client());
                    break;
                }
                case OUT: {
                    if (!is_client_in_club(event.get_client())) {
                        return Event(event.get_time(), EventId::GEN_ERROR, "ClientUnknown");
                    }
                    clients_.erase(event.get_client());
                    auto gen_event = client_out(event.get_client(), event.get_time());
                    if (gen_event.get_id() != EventId::GEN_OUT) {
                        return gen_event;
                    }
                    break;
                }
                default:
                    return std::nullopt;
            }
        }
        return std::nullopt;
    }

    std::vector<Event> close() {
        std::vector<Event> result;
        for (const Client &client: clients_) {
            result.push_back(client_out(client, close_time_));
        }
        return result;
    }

    std::string get_open_time() const {
        return open_time_;
    }

    void set_open_time(const std::string &time) {
        if (is_valid_time(time)) {
            open_time_ = time;
        }
    }

    std::string get_close_time() const {
        return close_time_;
    }

    void set_close_time(const std::string &time) {
        if (is_valid_time(time)) {
            close_time_ = time;
        }
    }

    int get_number_of_tables() const {
        return number_of_tables_;
    }

    std::vector<Table> get_tables() const {
        return tables_;
    }

    ClubId get_id() const {
        return id_;
    }

private:
    ClubId id_;
    size_t number_of_tables_;
    size_t number_of_free_tables_;
    std::string open_time_, close_time_;
    std::vector<Table> tables_;
    std::set<Client> clients_;
    std::queue<Client> clients_queue_;

    void sit_client(const Client &client, const std::string &time, int table) {
        clients_.erase(client);
        Client new_client = client;
        new_client.set_status(ClientStatus::IS_SITTING);
        new_client.set_start_sit_time(time);
        clients_.insert(new_client);
        set_table_busy(table, new_client);
    }

    void add_to_queue(const Client &client) {
        clients_.erase(client);
        Client new_client = client;
        new_client.set_status(ClientStatus::IS_WAITING);
        clients_.insert(new_client);
        clients_queue_.push(new_client);
    }

    Event client_out(const Client &client, const std::string &time) {
        int table_number = find_table_of_client(client);
        set_table_free(table_number, time);
        if (!clients_queue_.empty()) {
            Client new_client = clients_queue_.front();
            clients_queue_.pop();
            sit_client(new_client, time, table_number);
            return {time, EventId::GEN_SIT, new_client, table_number};
        }
        return {time, EventId::GEN_OUT, client.get_name()};
    }

    bool is_table_free(size_t number) {
        return tables_[number - 1].is_free();
    }

    void add_client(const Client &client) {
        clients_.insert(client);
    }

    void set_table_free(size_t number, const std::string &time) {
        tables_[number - 1].set_free(time);
        number_of_free_tables_++;
    }

    void set_table_busy(size_t number, const Client &client) {
        tables_[number - 1].set_busy(client);
        number_of_free_tables_--;
    }

    bool is_client_in_club(const Client &client) const {
        return clients_.find(client) != clients_.end();
    }

    int find_table_of_client(const Client &client) const {
        for (size_t i = 0; i < number_of_tables_; ++i) {
            if (tables_[i].get_client() == client) {
                return i + 1;
            }
        }
        throw std::runtime_error("Client isn't sitting");
    }

};
