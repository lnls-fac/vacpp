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
    int process_forever();
    int exported_function(std::string arg);
private:
    Accelerator _si;
};


#endif /* VACPP_DRIVER_H */
