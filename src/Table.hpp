#pragma once

#include "Utils.hpp"
#include <cmath>

class Table {
public:

    Table() : client_(Client()), is_free_(true), cost_of_hour_(0), profit_(0), work_time(0) {}

    explicit Table(int cost) : client_(Client()), is_free_(true), cost_of_hour_(cost), profit_(0), work_time(0) {}

    bool is_free() const {
        return is_free_;
    }

    Client get_client() const {
        return client_;
    }

    void set_busy(const Client &client) {
        client_ = client;
        is_free_ = false;
    }

    void set_free(const std::string &end_time) {
        profit_ += calc_profit(end_time);
        client_ = Client();
        is_free_ = true;
    }

    void set_cost_of_hour(int cost) {
        cost_of_hour_ = cost;
    }

    int get_profit() const {
        return profit_;
    }

    std::pair<std::string, std::string> get_work_time() const {
        return {std::to_string(work_time / 60), std::to_string(work_time % 60)};
    }

private:
    Client client_;
    bool is_free_;
    int cost_of_hour_;
    int profit_;
    int work_time;

    int calc_profit(const std::string &end_time) {
        int start = to_minutes(client_.get_start_sit_time());
        int end = to_minutes(end_time);
        work_time += (end - start);
        return (int) std::ceil((end - start) / 60.) * cost_of_hour_;
    }

};


