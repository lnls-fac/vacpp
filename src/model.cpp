#include "model.h"


void Model::process_model(Model* model)
{
    model->process();
}

Model::Model(std::atomic<bool>* is_running_flag)
{
    _is_running_flag = is_running_flag;
}

void Model::process()
{
    while (_is_running()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        _process_requests();
        _update_state();
        _update_values();
    }
    // TODO: catch exceptions
}
int Model::get_number_of_values_available()
{
    std::unique_lock<std::mutex> ql(_queue_to_driver_mutex);
    return _queue_to_driver.size();

}
std::vector<PVValuePair> Model::get_values(int quantity)
{
    std::vector<PVValuePair> values;

    std::unique_lock<std::mutex> ql(_queue_to_driver_mutex);
    for (int i=0; i<quantity; ++i) {
        PVValuePair& p = _queue_to_driver.front();
        values.push_back(std::move(p));
        _queue_to_driver.pop();
    }

    return values;
}

inline bool Model::_is_running()
{
    return _is_running_flag->load();
}

void Model::_process_requests()
{
    // TODO: add code
}
void Model::_update_state()
{
    // TODO: add code
}
void Model::_update_values()
{
    static int i = 0; // FIXME: debug value

    std::unique_lock<std::mutex> ql(_queue_to_driver_mutex);
    _queue_to_driver.push(PVValuePair(std::string("PVNAME"), double(i++)));
}
