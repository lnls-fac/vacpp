#ifndef VACPP_MODEL_H
#define VACPP_MODEL_H

#include <string>
#include <vector>
#include <chrono>
#include <queue>
#include <thread>
#include <mutex>
#include <utility>

#include "common.h"

#include <iostream> /* DEBUG */


class Model {
public:
    static void process_model(Model& model, Flag& is_running);

    void process();
    int get_number_of_values_available();
    std::vector<PVValuePair> get_values(int quantity);
    void set_value(const PVValuePair& pair);

    virtual void print() = 0;
private:
    static const std::chrono::milliseconds _min_process_duration;

    std::mutex _queue_to_driver_mutex, _queue_from_driver_mutex;
    std::queue<PVValuePair> _queue_to_driver, _queue_from_driver;

    void _process_requests();
    virtual void _update_state() = 0;
    void _update_values();
};


#endif /* VACPP_MODEL_H */
