#ifndef VACPP_DRIVER_H
#define VACPP_DRIVER_H

#include <iostream>
#include <string>
#include <thread>

#include <trackcpp/accelerator.h>
#include <trackcpp/flat_file.h>


class VaDriver {
public:
    VaDriver();
    int start_models();
    int process_forever();
    int exported_function(std::string arg);
private:
    std::string _si_file, _bo_file;
    Accelerator _si;
    Accelerator _bo;
};


#endif /* VACPP_DRIVER_H */
