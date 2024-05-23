#pragma once

#include <iomanip>

class EventManager {
public:

    int manage(std::ifstream &is) {
        std::string line;
        while (std::getline(is, line)) {
            if (!line.empty()) {
                Event event = parse_line(line);
                if (!event.is_valid()) {
                    std::cout << event << std::endl;
                    std::cerr << "FAIL: Invalid format for event" << std::endl;
                    return 1;
                }
                all_events_.push_back(event);
                notify(event);
            }
        }
        is.close();
        for (Club &cl: clubs_) {
            for (const Event &event: cl.close()) {
                all_events_.push_back(event);
            }
        }

        print_all_events();
        return 0;
    }

    void print_all_events() {
        for (Club &cl: clubs_) {
            std::cout << cl.get_open_time() << std::endl;
        }

        std::for_each(all_events_.begin(), all_events_.end(), [](const Event &event) {
            std::cout << event << std::endl;
        });

        for (Club &cl: clubs_) {
            std::cout << cl.get_close_time() << std::endl;
        }

        for (const Club &c: clubs_) {
            auto tables = c.get_tables();
            for (size_t i = 0; i < tables.size(); ++i) {
                std::cout << i + 1 << " " << tables[i].get_profit() << " "
                          << std::setfill('0') << std::setw(2) << tables[i].get_work_time().first << ":"
                          << std::setfill('0') << std::setw(2) << tables[i].get_work_time().second << std::endl;
            }
        }
    }

    void notify(const Event &event) {
        for (Club &club: clubs_) {
            std::optional<Event> result = club.handle_event(event);
            if (result.has_value()) {
                all_events_.push_back(result.value());
            }
        }
    }

    void subscribe(const Club &club) {
        clubs_.push_back(club);
    }

private:
    std::vector<Event> all_events_;
    std::vector<Club> clubs_;

    Event parse_line(const std::string &formatted_line) {
        std::stringstream ss(formatted_line);
        int id, table_number = -1, club_id = 1;
        std::string time, client_name;
        bool is_valid = true;
        ss >> time >> id >> client_name;
        if (id == 2) {
            ss >> table_number;
            if (table_number > clubs_[get_club_by_id(club_id)].get_number_of_tables()
                || table_number < 1) {
                is_valid = false;
            }
        }
        if (!check_params(id, time, client_name)) {
            is_valid = false;
        }
        auto id_ = static_cast<EventId>(id);
        return {time, id_, Client(client_name, id_status_mapping.at(id_)), table_number, is_valid};
    }

    size_t get_club_by_id(ClubId id) const {
        for (size_t i = 0; i < clubs_.size(); ++i) {
            if (clubs_[i].get_id() == id) {
                return i;
            }
        }
        return 1;
    }

};
