#include "driver.h"


VaDriver::VaDriver()
{
    _stop_flag.store(false);

    // TODO: generalise
    std::string flat_file_name("/home/fac_files/siriusdb/vacpp/si.txt");
    _model = new AcceleratorModel(flat_file_name, &_stop_flag);

    _start();
}
VaDriver::~VaDriver()
{
    if(_stop_flag.load())
        _stop();

    std::this_thread::sleep_for(std::chrono::seconds(1));        

    delete _model;
    delete _model_thread;
    delete _update_thread;
}

void VaDriver::update_driver(VaDriver *driver)
{
    /*
     * Static driver update thread function
     */
    driver->_update();
}

int VaDriver::set_value(const std::string& name, const double& value)
{
    /*
     * Set a model value
     */
    PVValuePair _pair(name, value);

    std::unique_lock<std::mutex> ql(_queue_from_server_mutex);
    this->_queue_from_server.push(_pair);

    return 0;
}
int VaDriver::get_number_of_values_available()
{
    /*
     * Get current number of values to be read from queue
     */
    std::unique_lock<std::mutex> ql(_queue_to_server_mutex);
    return _queue_to_server.size();
}
std::vector<PVValuePair> VaDriver::get_values(int quantity)
{
    /*
     * Read a number of values from queue
     */
    std::vector<PVValuePair> values;

    std::unique_lock<std::mutex> ql(_queue_to_server_mutex);
    for (int i=0; i<quantity; ++i) {
        PVValuePair& p = _queue_to_server.front();
        values.push_back(std::move(p));
        this->_queue_to_server.pop();
    }

    return values;
}

void VaDriver::_start()
{
    _start_models();
    _start_update();
}
void VaDriver::_start_models()
{
    _model_thread = new std::thread(&Model::process_model, _model);
    _model_thread->detach();
}
void VaDriver::_start_update()
{
    _update_thread = new std::thread(&VaDriver::update_driver, this);
    _update_thread->detach();
}
void VaDriver::_stop()
{
    _stop_flag.store(true);
    // TODO: check if threads stopped
}

void VaDriver::_update()
{
    while (!_stop_flag.load()) {
        _send_values_to_models();
        _recv_values_from_models();
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
    }
}
void VaDriver::_send_values_to_models()
{

}
void VaDriver::_recv_values_from_models()
{
    int quantity = _model->get_number_of_values_available();
    if (quantity == 0)
        return;

    // TODO: Loop over all models
    std::vector<PVValuePair> values = _model->get_values(quantity);
    std::unique_lock<std::mutex> ql(_queue_to_server_mutex);
    for (auto& value : values)
        _queue_to_server.push(std::move(value));
}

void print_pairs(const std::vector<PVValuePair>& pairs)
{
    for (const auto& pair : pairs)
        print_pair(pair);
}

void print_pair(const PVValuePair& pair)
{
    std::cout << pair.first << ": " << pair.second << std::endl;
}
