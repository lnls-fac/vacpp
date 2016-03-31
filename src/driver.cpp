#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <chrono>

#include "driver.h"
#include "beamcharge.h"

#include "sirius_models.h"

class PV {
public:
  PV(const std::string& name_, double init_value_ = 0) {
    this->name = name_;
    this->value = init_value_;
  }
  std::string name;
  double value;
};


static PV sidi_current  ("SIDI-CURRENT",  300.0);
static PV sipa_lifetime ("SIPA-LIFETIME", 10.0);
static PV bodi_current  ("BODI-CURRENT",  2.0);
static PV bopa_lifetime ("BOPA-LIFETIME", 1.0);

static void update_currents() {
  static std::chrono::system_clock::time_point t0 = std::chrono::system_clock::now();
         std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
  int dt = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
  sidi_current.value *= exp(-(dt/1.0e6)/(3600*sipa_lifetime.value));
  bodi_current.value *= exp(-(dt/1.0e6)/(3600*bopa_lifetime.value));
  t0 = t1;
}

int python_to_cpp(const std::string& pv, const double& value) {
  return 0;
}

void cpp_to_python(std::vector<std::string>& pvs, std::vector<double>& values) {

  update_currents();

  pvs.push_back(sidi_current.name);  values.push_back(sidi_current.value);
  pvs.push_back(sipa_lifetime.name); values.push_back(sipa_lifetime.value);
  pvs.push_back(bodi_current.name);  values.push_back(bodi_current.value);
  pvs.push_back(bopa_lifetime.name); values.push_back(bopa_lifetime.value);

}
