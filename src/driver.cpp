#include <string>
#include <vector>
#include <ctime>
#include <cmath>

#include "driver.h"

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
  static time_t t0 = time(0);
         time_t t1 = time(0);
  sidi_current.value *= exp(-(t1 - t0)/(3600*sipa_lifetime.value));
  //bodi_current.value *= exp(-(t1 - t0)/(3600*bopa_lifetime.value));
  t0 = t1;
}

int python_to_cpp(const std::string& pv, const double& value) {
  return 0;
}

void cpp_to_python(std::vector<std::string>& pvs, std::vector<double>& values) {

  //update_currents();
  sidi_current.value += 1;
  pvs.push_back(sidi_current.name);  values.push_back(sidi_current.value);
  //pvs.push_back(sipa_lifetime.name); values.push_back(sipa_lifetime.value);

  //pvs.push_back(bodi_current.name);  values.push_back(bodi_current.value);
  //pvs.push_back(bopa_lifetime.name); values.push_back(bopa_lifetime.value);

}
