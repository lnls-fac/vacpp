#include "model.h"


const std::chrono::milliseconds Model::_min_process_duration(50);

void Model::process_model(Model& model, Flag& is_running)
{
    /*
     * Static model processing thread function
     */
    while (is_running.load()) {
        auto start_time = Clock::now();
        model.process();
        _wait_interval_from(_min_process_duration, start_time);
    }
}

void Model::process()
{
    _process_requests();
    _update_state();
    _update_values();
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
    static int i = 0; // FIXME: debug code

    std::unique_lock<std::mutex> ql(_queue_to_driver_mutex);
    _queue_to_driver.push(PVValuePair(std::string("PVNAME"), double(i++)));
}
