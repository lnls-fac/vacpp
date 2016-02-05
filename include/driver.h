#ifndef VACPP_DRIVER_H
#define VACPP_DRIVER_H

#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>

#include "accelerator_model.h"
#include "common.h"


class VaDriver {
public:
    static void update_driver(VaDriver* driver);

    VaDriver();
    ~VaDriver();

    void stop();
    int start_models();
    int start_update();

    int set_value(const std::string& name, const double& value);
    int get_number_of_values_available();
    std::vector<PVValuePair> get_values(int quantity);
private:
    const int sleep_time = 100; // ms

    std::atomic<bool> _stop_flag;
    std::mutex _send_mutex, _recv_mutex;
    std::queue<PVValuePair> _send_queue, _recv_queue;
    AcceleratorModel* _model;
    std::thread* _update_thread;
    std::thread* _model_thread;

    void _update();
    void _send_values_to_models();
    void _recv_values_from_models();
};

void print_pair(PVValuePair pair);


#endif /* VACPP_DRIVER_H */
