#ifndef VACPP_DRIVER_H
#define VACPP_DRIVER_H

#include <string>
#include <chrono>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>

#include "common.h"
#include "accelerator_model.h"
#include "model_manager.h"

#include <iostream> /* FIXME: debug include */


class VaDriver {
public:
    static void update_driver(VaDriver* driver); // update thread function

    VaDriver();
    ~VaDriver();

    int start();
    int stop();

    int set_value(const std::string& name, const double& value);
    int get_number_of_values_available();
    std::vector<PVValuePair> get_values(int quantity);
private:
    static const int _min_update_duration_ms = 100;

    std::chrono::milliseconds _min_update_duration;
    std::atomic<bool> _is_running_flag;
    std::mutex _queue_to_server_mutex, _queue_from_server_mutex;
    std::queue<PVValuePair> _queue_to_server, _queue_from_server;

    std::thread* _update_thread = nullptr;
    std::thread* _model_thread = nullptr;

    AcceleratorModel* _model = nullptr; // TODO: generalise
    ModelManager* _model_manager = nullptr;

    // Thread start functions
    void _start_models();
    void _start_update();

    inline bool _is_running();
    inline void _set_is_running_flag();
    inline void _unset_is_running_flag();

    void _update();
    void _process_communication_and_wait();
    void _wait_from(const TimePoint& start_time);
    void _send_values_to_models();
    void _recv_values_from_models();
};

void print_pairs(const std::vector<PVValuePair>& pairs); // debug
void print_pair(const PVValuePair& pair); // debug


#endif /* VACPP_DRIVER_H */
