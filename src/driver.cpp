#include "driver.h"


VaDriver::VaDriver()
{
    std::string flat_file_name("/home/fac_files/siriusdb/vacpp/si.txt");
    _model = new AcceleratorModel(flat_file_name);
}
VaDriver::~VaDriver()
{
    delete _model;
    delete _model_thread;
}

int VaDriver::process_forever()
{
    // while (true) {
    //     if (!this->_recv_queue.empty()) {
    //         PVValuePair _pair = this->_recv_queue.front();
    //         print_pair(_pair);
    //         this->_recv_queue.pop();
    //     }
    // }
    std::cin.get();
    stop_flag.store(true);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
int VaDriver::set_value(const std::string& name, const double& value)
{
    PVValuePair _pair(name, value);
    this->_recv_queue.push(_pair);
    return 0;
}
PVValuePair VaDriver::get_value(int quantity)
{
    PVValuePair _pair = this->_send_queue.front();
    this->_send_queue.pop();
    return _pair;
}

int VaDriver::start_models()
{
    _model_thread = new std::thread(&Model::process_model, _model);
    _model_thread->detach();
    return 0;
}

void print_pair(PVValuePair pair)
{
    std::cout << pair.first << ": " << pair.second << std::endl;
}
