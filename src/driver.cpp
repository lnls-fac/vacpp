#include "driver.h"


VaDriver::VaDriver()
{
    std::string flat_file_dir("/home/afonso/test/vacpp/files/");
    this->_si_file = std::string(flat_file_dir+"si.txt");
    this->_bo_file = std::string(flat_file_dir+"bo.txt");
}
// VaDriver::VaDriver(const std::string& flat_file_dir)
// {
//     this->_si_file = std::string(flat_file_dir+"si.txt");
//     this->_bo_file = std::string(flat_file_dir+"bo.txt");
// }
int VaDriver::start_models()
{
    read_flat_file(this->_si_file, this->_si);
    read_flat_file(this->_bo_file, this->_bo);

    return 0;
}
int VaDriver::process_forever()
{
    std::cout << this->_si.harmonic_number << std::endl;
    std::cout << this->_bo.harmonic_number << std::endl;

    std::cout << "Processing forever...\n";

    while (true)
        ;

    return 0;
}
