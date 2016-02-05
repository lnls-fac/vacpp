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


extern std::atomic<bool> stop_flag;


class Model {
public:
    Model();
    static void process_model(Model* m);
    void process();
    int get_number_of_values_available();
    std::vector<PVValuePair> get_values(int quantity);
private:
    const int sleep_time = 50; // ms
    std::mutex _send_mutex, _recv_mutex;
    std::queue<PVValuePair> _send_queue, _recv_queue;
    void _process_requests();
    virtual void _update_state() = 0;
    void _update_values();
};


#endif /* VACPP_MODEL_H */
