#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <vector>

bool   update_models();
void   add_dynamic_pvs_to_list();
double get_model_parameter(const std::string& name);
void   get_list_of_changed_pvs(std::vector<std::string>& pvs);
void   get_pv(const std::string& pv, std::vector<double>& values);
void   set_pv(const std::string& pv, const double& value);
void   set_devicenames(const std::string& famname, const std::vector<std::string>& devicenames);

#endif
