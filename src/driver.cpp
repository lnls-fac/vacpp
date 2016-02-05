#include "driver.h"


VaDriver::VaDriver()
{
    std::string flat_file_name("/home/fac_files/siriusdb/vacpp/si.txt");
    _model = new AcceleratorModel(flat_file_name);
}
VaDriver::~VaDriver()
{
    delete _model;
}
int VaDriver::process_forever()
{
    while (true) {
        if (!this->_recv_queue.empty()) {
            PVValuePair _pair = this->_recv_queue.front();
            print_pair(_pair);
            this->_recv_queue.pop();
        }
    }

    return 0;
}
int VaDriver::exported_function(std::string arg)
{
    std::cout << "Entering exported function..." << std::endl;
    std::cout << arg << std::endl;
    std::cout << "Returning from exported function..." << std::endl;
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

void print_pair(PVValuePair pair)
{
    std::cout << pair.first << ": " << pair.second << std::endl;
}
