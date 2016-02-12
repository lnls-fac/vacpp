#include "driver.h"


const std::chrono::milliseconds VaDriver::_min_update_duration(100);

VaDriver::VaDriver()
{
    _is_running.store(false);

    for (const auto& prefix : model_prefixes) {
        _model_threads[prefix] = nullptr;
    }
}
VaDriver::~VaDriver()
{
    _delete_model_threads();
    delete _update_thread; // TODO: generalise
}

void VaDriver::update_driver(VaDriver& driver, Flag& is_running)
{
    /*
     * Static driver update thread function
     */
    while (is_running.load())
        driver._process_communication_and_wait();
}

int VaDriver::start()
{
    // TODO: evaluate use of exception instead of return code
    if (_is_running.load())
        return ERROR_DRIVER_ALREADY_STARTED;

    _is_running.store(true);

    _start_model_threads();
    _start_update_thread();
    // TODO: check if everything is up and return status
    return SUCCESS;
}
int VaDriver::stop()
{
    // TODO: evaluate use of exception instead of return code
    if (!_is_running.load())
        return ERROR_DRIVER_NOT_STARTED;

    _is_running.store(false);
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
    _queue_from_server.emplace(name, value);

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
        _queue_to_server.pop();
    }

    return values;
}

void VaDriver::_start_model_threads()
{
    for (const auto& prefix : model_prefixes) {
        Model* model = _model_manager.get_model_by_prefix(prefix);
        _model_threads[prefix] = _start_model_thread(model);
    }
}
std::thread* VaDriver::_start_model_thread(Model* model)
{
    std::thread* model_thread = new std::thread(&Model::process_model,
        std::ref(*model), std::ref(_is_running));
    model_thread->detach();
    return model_thread;
}
void VaDriver::_start_update_thread()
{
    _update_thread = new std::thread(&VaDriver::update_driver,
        std::ref(*this), std::ref(_is_running));
    _update_thread->detach();
}
void VaDriver::_delete_model_threads()
{
    // TODO: check if threads were created before deletion
    for (const auto& prefix : model_prefixes) {
        delete _model_threads[prefix];
    }
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
    for (const auto& prefix : model_prefixes) {
        Model* model = _model_manager.get_model_by_prefix(prefix);
        _recv_values_from_model(model);
    }
}
void VaDriver::_recv_values_from_model(Model* model)
{
    int quantity = model->get_number_of_values_available();
    if (quantity == 0)
        return;

    // TODO: Loop over all models
    std::vector<PVValuePair> values = model->get_values(quantity);
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
