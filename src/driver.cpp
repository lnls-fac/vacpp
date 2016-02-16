#include "driver.h"


const std::chrono::milliseconds VaDriver::_min_update_duration(100);
const std::chrono::milliseconds VaDriver::_finalisation_wait(1000);

VaDriver::VaDriver()
{
    _is_running.store(false);

    for (const auto& prefix : model_prefixes) {
        _model_threads[prefix] = nullptr;
    }
    _update_thread = nullptr;
}
VaDriver::~VaDriver()
{
    _delete_model_threads();
    _delete_update_thread();
}

void VaDriver::update_driver(VaDriver& driver, Flag& is_running)
{
    /*
     * Static driver update thread function
     */
    while (is_running.load()) {
        auto start_time = Clock::now();
        driver.process_communication();
        _wait_interval_from(_min_update_duration, start_time);
    }
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
void VaDriver::process_communication()
{
    _send_values_to_models();
    _recv_values_from_models();
}
int VaDriver::stop()
{
    // TODO: evaluate use of exception instead of return code
    if (!_is_running.load())
        return ERROR_DRIVER_NOT_STARTED;

    _is_running.store(false);
    std::this_thread::sleep_for(_finalisation_wait);

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
     * Get current number of values available to be read
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
void VaDriver::_delete_model_threads()
{
    for (const auto& prefix : model_prefixes) {
        if (_model_threads[prefix] != nullptr) {
            delete _model_threads[prefix];
        }
    }
}

void VaDriver::_start_update_thread()
{
    _update_thread = new std::thread(&VaDriver::update_driver,
        std::ref(*this), std::ref(_is_running));
    _update_thread->detach();
}
void VaDriver::_delete_update_thread()
{
    if (_update_thread != nullptr) {
        delete _update_thread;
    }
}

void VaDriver::_send_values_to_models()
{
    std::unique_lock<std::mutex> ql(_queue_from_server_mutex);
    int n = _queue_from_server.size();
    for (int i=0; i<n; ++i) {
        _send_value_to_model();
    }
}
void VaDriver::_send_value_to_model()
{
    const PVValuePair& pair = _queue_from_server.front();
    Model* model = _get_model_from_pv_value_pair(pair);
    model->set_value(pair);
    _queue_from_server.pop();
}
Model* VaDriver::_get_model_from_pv_value_pair(const PVValuePair& pair)
{
    const std::string& name = pair.first;
    const std::string& prefix = _get_prefix_from_name(name);
    return _model_manager.get_model_by_prefix(prefix);
}
const std::string VaDriver::_get_prefix_from_name(const std::string& name)
{
    // TODO: generalise
    return std::string("si");
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

    std::vector<PVValuePair> values = model->get_values(quantity);
    std::unique_lock<std::mutex> ql(_queue_to_server_mutex);
    for (auto& value : values)
        _queue_to_server.push(std::move(value));
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
