#include "driver.h"


VaDriver::VaDriver()
{
    _min_update_duration = std::chrono::milliseconds(_min_update_duration_ms);
    _unset_is_running_flag();

    // _model_manager = new ModelManager(&_is_running_flag);

    // TODO: generalise
    //_load_models();
    std::string flat_file_name("/home/fac_files/siriusdb/vacpp/si.txt");
    _model = new AcceleratorModel(flat_file_name, &_is_running_flag);
}
VaDriver::~VaDriver()
{
    // TODO: generalise
    //_delete_models();
    //_delete_threads();
    // delete _model_manager;
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

int VaDriver::start()
{
    if (_is_running())
        return ERROR_DRIVER_ALREADY_STARTED;

    _set_is_running_flag();

    _start_models();
    _start_update();
    // TODO: check if everything is up and return status
    return SUCCESS;
}
int VaDriver::stop()
{
    if (!_is_running())
        return ERROR_DRIVER_NOT_STARTED;

    _unset_is_running_flag();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // TODO: check if threads stopped and return status
    return SUCCESS;
}

int VaDriver::set_value(const std::string& name, const double& value)
{
    /*
     * Set a model value
     */
    std::unique_lock<std::mutex> ql(_queue_from_server_mutex);
    this->_queue_from_server.emplace(name, value);

    return SUCCESS;
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

inline bool VaDriver::_is_running()
{
    return _is_running_flag.load();
}
inline void VaDriver::_set_is_running_flag()
{
    _is_running_flag.store(true);
}
inline void VaDriver::_unset_is_running_flag()
{
    _is_running_flag.store(false);
}

void VaDriver::_update()
{
    while (_is_running())
        _process_communication_and_wait();
}
void VaDriver::_process_communication_and_wait()
{
    const auto start_time = Clock::now();

    _send_values_to_models();
    _recv_values_from_models();

    _wait_from(start_time);
}
void VaDriver::_send_values_to_models()
{
    // TODO: add code
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
void VaDriver::_wait_from(const TimePoint &start_time)
{
    const auto delta = Clock::now() - start_time;
    if (delta < _min_update_duration)
    std::this_thread::sleep_for(_min_update_duration-delta);
}

// TODO: remove debug print functions
void print_pairs(const std::vector<PVValuePair>& pairs)
{
    for (const auto& pair : pairs)
        print_pair(pair);
}

void print_pair(const PVValuePair& pair)
{
    std::cout << pair.first << ": " << pair.second << std::endl;
}
