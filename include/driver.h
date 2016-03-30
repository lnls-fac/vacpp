#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <vector>

int python_to_cpp(const std::string& pv, const double& value);
void cpp_to_python(std::vector<std::string>& pvs, std::vector<double>& values);

#endif
