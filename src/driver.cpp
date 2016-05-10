#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <chrono>

#include "driver.h"
#include "beam_charge.h"
#include "sirius_models.h"

SiriusModels models;

int python_to_cpp(const std::string& pv, const double& value) {
  std::cout << pv << " " << value << std::endl;
  return 0;
}

void cpp_to_python(std::vector<std::string>& pvs, std::vector<double>& values) {

  // BO pvs
  models.set_bo_model().beam_charge.update_bunches();
  const double bo_revolution_period = models.get_bo_model().get_revolution_period();
  pvs.push_back("BODI-CURRENT"); values.push_back(1000*models.set_bo_model().beam_charge.get_charge() / bo_revolution_period);
  pvs.push_back("BOPA-LIFETIME"); values.push_back(models.set_bo_model().beam_charge.get_lifetime()/3600);

  // SI pvs
  models.set_si_model().beam_charge.update_bunches();
  const double si_revolution_period = models.get_si_model().get_revolution_period();
  pvs.push_back("SIDI-CURRENT"); values.push_back(1000*models.set_si_model().beam_charge.get_charge() / si_revolution_period);
  pvs.push_back("SIPA-LIFETIME"); values.push_back(models.set_si_model().beam_charge.get_lifetime()/3600);


}
