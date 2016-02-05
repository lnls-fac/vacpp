#include "driver.h"


VaDriver::VaDriver()
{
    _stop_flag.store(false);

    std::string flat_file_name("/home/fac_files/siriusdb/vacpp/si.txt");
    _model = new AcceleratorModel(flat_file_name, &_stop_flag);
}
VaDriver::~VaDriver()
{
    delete _model;
    delete _model_thread;
    delete _update_thread;
}

void VaDriver::update_driver(VaDriver *driver)
{
    /*
     * Driver update thread function
     */
    std::cout << "Inside update_driver()...\n";
    driver->_update();
    std::cout << "Exiting update_driver()...\n";
}

void VaDriver::stop()
{
    _stop_flag.store(true);
}
int VaDriver::start_models()
{
    /*
    * Start model threads
    */
    _model_thread = new std::thread(&Model::process_model, _model);
    _model_thread->detach();
    return 0;
}
int VaDriver::start_update()
{
    /*
    * Start driver update thread
    */
    _update_thread = new std::thread(&VaDriver::update_driver, this);
    _update_thread->detach();
    return 0;
}

int VaDriver::set_value(const std::string& name, const double& value)
{
    /*
     * Set a model value
     */
    PVValuePair _pair(name, value);

    std::unique_lock<std::mutex> ql(_recv_mutex);
    this->_recv_queue.push(_pair);

    return 0;
}
int VaDriver::get_number_of_values_available()
{
    /*
     * Get current number of values to be read from queue
     */
    std::unique_lock<std::mutex> ql(_send_mutex);
    return _send_queue.size();
}
std::vector<PVValuePair> VaDriver::get_values(int quantity)
{
    /*
     * Read a number of values from queue
     */
    std::vector<PVValuePair> values;

    std::unique_lock<std::mutex> ql(_send_mutex);
    for (int i=0; i<quantity; ++i) {
        PVValuePair& p = _send_queue.front();
        values.push_back(std::move(p));
        this->_send_queue.pop();
    }

    return values;
}

void VaDriver::_update()
{
    while (!_stop_flag.load()) {
        // Pass values to models
        _send_values_to_models();
        _recv_values_from_models();
        // Read values from models

        // if (!this->_recv_queue.empty()) {
        //     PVValuePair _pair = this->_recv_queue.front();
        //     print_pair(_pair);
        //     this->_recv_queue.pop();
        // }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
    }
}
void VaDriver::_send_values_to_models()
{

}
void VaDriver::_recv_values_from_models()
{

}

void print_pair(PVValuePair pair)
{
    std::cout << pair.first << ": " << pair.second << std::endl;
}
