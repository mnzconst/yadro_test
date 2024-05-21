#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

struct Table {
    std::string client_name;
    std::string start_time;
    bool is_free = true;
};

struct Event {
    std::string start_time;
    int id = -1;
    std::string client_name;
    int table_number = -1;
};

std::ostream &operator<<(std::ostream &os, const Event &event) {
    os << event.start_time << " " << event.id << " " << event.client_name;
    if (event.table_number != -1) {
        os << " " << event.table_number;
    }
    return os;
}

bool is_valid_time(std::string_view time) {
    if (time.size() != 5) {
        return false;
    }
    return time.substr(0, 2) < "24" && time[2] == ':' && time.substr(3) < "60";
}

bool is_valid_name(std::string name) {
    return std::all_of(name.begin(), name.end(), [](char ch) {
        return ch < 'z' && ch > 'a' || ch > '0' && ch < '9' || ch == '_' || ch == '-';
    });
}

bool is_valid_event(const Event &event) {
    return event.id > 0
           && event.id < 5
           && is_valid_time(event.start_time)
           && is_valid_name(event.client_name);
}

Event create_event(const std::string &line, size_t number_of_tables) {
    Event event;
    std::stringstream ss(line);
    ss >> event.start_time >> event.id >> event.client_name;
    if (event.id == 2) {
        ss >> event.table_number;
        if (event.table_number > number_of_tables
            || event.table_number < 1) {
            event.id = -1;
        }
    }
    if (!is_valid_event(event)) {
        event.id = -1;
    }
    return event;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "FAIL: Unexpected amount of files" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "FAIL: Can't open the file" << std::endl;
        return 1;
    }

    size_t number_of_tables, cost_of_hour;
    std::string open_time, close_time, input;
    file >> number_of_tables >> open_time
         >> close_time >> cost_of_hour;

    if (!is_valid_time(open_time) || !is_valid_time(close_time)) {
        std::cerr << "FAIL: Invalid time format" << std::endl;
        return 1;
    }

    std::vector<Event> events;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            Event event = create_event(line, number_of_tables);
            std::cout << event << std::endl;
            if (event.id == -1) {
                std::cerr << "FAIL: Invalid format for event" << std::endl;
                return 1;
            }
            events.push_back(event);
        }
    }
    file.close();

    std::vector<Table> tables(number_of_tables);

}
