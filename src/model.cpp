#include "model.h"


extern sig_atomic_t stop_flag;


Model::Model()
{
    
}
void Model::process()
{
    std::cout << "Processing..." << std::endl;
    while (!stop_flag) {
        std::cout << "Will sleep " << sleep_time << " ms..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        // _process_requests();
        // _update_state();
        _update_values();
    }
}
int Model::get_number_of_values_available()
{
    std::unique_lock<std::mutex> ql(_send_mutex);
    return _send_queue.size();

}
std::vector<PVValuePair> Model::get_values(int quantity)
{
    std::vector<PVValuePair> values;

    std::unique_lock<std::mutex> ql(_send_mutex);
    for (int i=0; i<quantity; ++i) {
        PVValuePair& p = _send_queue.front();
        values.push_back(std::move(p));
        _send_queue.pop();
    }

    return values;
}
void Model::_update_values()
{
    static int i = 0;

    std::unique_lock<std::mutex> ql(_send_mutex);
    _send_queue.push(PVValuePair(std::string("PVNAME"), double(i++)));
}