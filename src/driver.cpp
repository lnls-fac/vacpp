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

  models.set_si_model().beam_charge.update_bunches();
  pvs.push_back("SIDI-CURRENT"); values.push_back(models.set_si_model().beam_charge.get_charge());
  pvs.push_back("SIPA-LIFETIME"); values.push_back(models.set_si_model().beam_charge.get_charge());
  // update_currents();
  // pvs.push_back(sidi_current.name);  values.push_back(sidi_current.value);
  // pvs.push_back(sipa_lifetime.name); values.push_back(sipa_lifetime.value);
  // pvs.push_back(bodi_current.name);  values.push_back(bodi_current.value);
  // pvs.push_back(bopa_lifetime.name); values.push_back(bopa_lifetime.value);

}
