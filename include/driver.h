#ifndef VACPP_DRIVER_H
#define VACPP_DRIVER_H

#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <mutex>

#include "accelerator_model.h"
#include "common.h"


extern std::atomic<bool> stop_flag;


class VaDriver {
public:
    VaDriver();
    ~VaDriver();

    int process_forever();
    int set_value(const std::string& name, const double& value);
    PVValuePair get_value(int quantity);

    int start_models();
private:
    std::queue<PVValuePair> _send_queue, _recv_queue;
    AcceleratorModel* _model;
    std::thread* _model_thread;
};

void print_pair(PVValuePair pair);


#endif /* VACPP_DRIVER_H */
