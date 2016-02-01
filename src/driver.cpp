#include "driver.h"


VaDriver::VaDriver()
{
    std::string file_name("/home/fac_files/siriusdb/vacpp/si.txt");
    read_flat_file(file_name, this->_si);
}
int VaDriver::process_forever()
{
    std::cout << this->_si.harmonic_number << std::endl;
    std::cout << "Processing forever..." << std::endl;

    while (true) {
        if (!this->_recv_queue.empty()) {
            PVValuePair _pair = this->_recv_queue.front();
            _pair.print();
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
int VaDriver::write_pv(const std::string& name, const double& value)
{
    PVValuePair _pair(name, value);
    this->_recv_queue.push(_pair);
    return 0;
}
PVValuePair VaDriver::read_pv()
{
    PVValuePair _pair = this->_send_queue.front();
    this->_send_queue.pop();
    return _pair;
}

void PVValuePair::print()
{
    std::cout << _name << ": " << _value << std::endl;
}
