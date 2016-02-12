#ifndef VACPP_DRIVER_H
#define VACPP_DRIVER_H

#include <string>
#include <chrono>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <unordered_map>

#include "common.h"
#include "accelerator_model.h"
#include "model_manager.h"

#include <iostream> /* FIXME: debug include */


class VaDriver {
public:
    // Update thread function
    static void update_driver(VaDriver& driver, Flag& is_running);

    VaDriver();
    ~VaDriver();

    int start();
    int stop();

    int set_value(const std::string& name, const double& value);
    int get_number_of_values_available();
    std::vector<PVValuePair> get_values(int quantity);
private:
    static const std::chrono::milliseconds _min_update_duration;

    Flag _is_running;
    std::mutex _queue_to_server_mutex, _queue_from_server_mutex;
    std::queue<PVValuePair> _queue_to_server, _queue_from_server;
    std::thread* _update_thread = nullptr;
    std::unordered_map<std::string, std::thread*> _model_threads;

    ModelManager _model_manager;

    // Thread start functions
    void _start_model_threads();
    std::thread* _start_model_thread(Model* model);
    void _start_update_thread();
    void _delete_model_threads();

    void _process_communication_and_wait();
    void _wait_from(const TimePoint& start_time);
    void _send_values_to_models();
    void _recv_values_from_models();
    void _recv_values_from_model(Model* model);
};

void print_pairs(const std::vector<PVValuePair>& pairs); // debug
void print_pair(const PVValuePair& pair); // debug


#endif /* VACPP_DRIVER_H */
