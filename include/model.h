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
    static void process_model(Model* m);

    Model(std::atomic<bool>* is_running_flag);

    void process();
    int get_number_of_values_available();
    std::vector<PVValuePair> get_values(int quantity);
private:
    const int sleep_time = 50; // ms

    std::atomic<bool>* _is_running_flag;
    std::mutex _queue_to_driver_mutex, _queue_from_driver_mutex;
    std::queue<PVValuePair> _queue_to_driver, _queue_from_driver;

    inline bool _is_running();

    void _process_requests();
    virtual void _update_state() = 0;
    void _update_values();
};


#endif /* VACPP_MODEL_H */
