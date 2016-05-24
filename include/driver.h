#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <vector>

bool   update_models();
void   add_dynamic_pvs_to_list();
void   get_list_of_changed_pvs(std::vector<std::string>& pvs);
double get_pv(const std::string& pv);
void   set_pv(const std::string& pv, const double& value);
void   set_devicenames(const std::string& famname, const std::vector<std::string>& devicenames);

#endif
