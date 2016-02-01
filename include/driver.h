#ifndef VACPP_DRIVER_H
#define VACPP_DRIVER_H

#include <iostream>
#include <string>
#include <queue>
#include <thread>

#include <trackcpp/accelerator.h>
#include <trackcpp/flat_file.h>


class PVValuePair;

class VaDriver {
public:
    VaDriver();
    int process_forever();
    int exported_function(std::string arg);
    int write_pv(const std::string& name, const double& value);
    PVValuePair read_pv();
private:
    std::queue<PVValuePair> _send_queue, _recv_queue;
    Accelerator _si;
};

class PVValuePair {
public:
    PVValuePair(const std::string& name, const double& value) : _name(name), _value(value) { ; } // define necessary constructors
    void print();
private:
    std::string _name;
    double _value;
};


#endif /* VACPP_DRIVER_H */
