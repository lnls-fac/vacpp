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

    while (true)
        ;

    return 0;
}
int VaDriver::exported_function(std::string arg)
{
    std::cout << "Entering exported function..." << std::endl;
    std::cout << arg << std::endl;
    std::cout << "Returning from exported function..." << std::endl;
    return 0;
}
