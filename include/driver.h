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
    // VaDriver(const std::string& flat_file_dir);
    int start_models();
    int process_forever();
private:
    std::string _si_file, _bo_file;
    Accelerator _si;
    Accelerator _bo;
};


#endif /* VACPP_DRIVER_H */
