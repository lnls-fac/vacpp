#include <string>
#include <vector>
#include <ctime>
#include <cmath>

#include "driver.h"

static const std::string sidi_current_pv = "CURRENT";

static double sipa_lifetime = 10.0;

double update_sidi_current() {
  static double sidi_current = 300;
  static time_t t0 = time(0);
  time_t t1 = time(0);
  double newI = sidi_current * exp(-(t1 - t0)/(3600*sipa_lifetime));
  t0 = t1;
  sidi_current = newI;
  return newI;
}

int python_to_cpp(const std::string& pv, const double& value) {
  return 0;
}

void cpp_to_python(std::vector<std::string>& pvs, std::vector<double>& values) {

  double sidi_current = update_sidi_current();

  pvs.push_back(sidi_current_pv);
  values.push_back(sidi_current);

}
