#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "Client.hpp"
#include "Table.hpp"
#include "Club.hpp"
#include "EventManager.hpp"

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

    size_t number_of_tables;
    std::string open_time, close_time;
    int cost_of_hour;
    file >> number_of_tables >> open_time
         >> close_time >> cost_of_hour;

    if (!is_valid_time(open_time) || !is_valid_time(close_time)) {
        std::cerr << "FAIL: Invalid time format" << std::endl;
        return 1;
    }

    std::vector<Table> tables(number_of_tables, Table(cost_of_hour));
    Club club(1, number_of_tables, open_time, close_time, tables);
    EventManager event_manager;
    event_manager.subscribe(club);
    return event_manager.manage(file);
}
