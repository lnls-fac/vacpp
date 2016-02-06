#ifndef VACPP_DRIVER_H
#define VACPP_DRIVER_H

#include <string>
#include <chrono>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>

#include "accelerator_model.h"
#include "common.h"

#include <iostream> /* FIXME: debug include */


class VaDriver {
public:
    static void update_driver(VaDriver* driver);

    VaDriver();
    ~VaDriver();

    int set_value(const std::string& name, const double& value);
    int get_number_of_values_available();
    std::vector<PVValuePair> get_values(int quantity);
private:
    const int sleep_time = 100; // ms

    std::atomic<bool> _stop_flag;
    std::mutex _queue_to_server_mutex, _queue_from_server_mutex;
    std::queue<PVValuePair> _queue_to_server, _queue_from_server;
    AcceleratorModel* _model;
    std::thread* _update_thread;
    std::thread* _model_thread;

    void _start();
    void _start_models();
    void _start_update();
    void _stop();

    void _update();
    void _send_values_to_models();
    void _recv_values_from_models();
};

void print_pairs(const std::vector<PVValuePair>& pairs);
void print_pair(const PVValuePair& pair);


#endif /* VACPP_DRIVER_H */
