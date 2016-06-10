#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <chrono>

#include "driver.h"
#include "beam_charge.h"
#include "sirius_models.h"

static std::vector<std::string> changed_pvs;

bool update_models() {
    bool changed = models.update_all();
    return changed;
}

double get_model_parameter(const std::string& name) {
  if (name.find("si_harmonic_number") != std::string::npos) {
    return models.set_si_model().get_accelerator().harmonic_number;
  }
  if (name.find("bo_harmonic_number") != std::string::npos) {
    return models.set_bo_model().get_accelerator().harmonic_number;
  }
}

void add_dynamic_pvs_to_list() {

  // This function is invoked periodically from 'driver.py' to update all dynamic pvs with model values

  changed_pvs.push_back("BODI-CURRENT");
  changed_pvs.push_back("BODI-CURRENT-BUNCHES");
  changed_pvs.push_back("BOPA-LIFETIME");
  changed_pvs.push_back("BOPA-LIFETIME-BUNCHES");
  changed_pvs.push_back("SIDI-CURRENT");
  changed_pvs.push_back("SIDI-CURRENT-BUNCHES");
  changed_pvs.push_back("SIPA-LIFETIME");
  changed_pvs.push_back("SIPA-LIFETIME-BUNCHES");

}

void get_list_of_changed_pvs(std::vector<std::string>& pvs) {

  // This function is invoked by 'driver.py' right after a change in pv setpoint.
  // Its passes up all pv names and values for that need updating in epics memory
  // The global buffer is cleared.

  for(auto i = 0; i<changed_pvs.size(); ++i) pvs.push_back(changed_pvs[i]);
  changed_pvs.clear();

}

void get_pv(const std::string& pv, std::vector<double>& values) {
  models.update_all();
  values.clear();
  if (pv.find("SI") == 0) return models.set_si_model().get_pv(pv, values);
  if (pv.find("BO") == 0) return models.set_bo_model().get_pv(pv, values);
  if (pv.find("TESTSI") == 0) return models.set_si_model().get_pv(pv, values); // test
  // execution should never reach this!
}

void set_pv(const std::string& pv, const double& value) {

  // This function in run when 'driver.py' received a request to change
  // a pv setpoint. It updates the model.

  if (pv.find("SI") == 0) { models.set_si_model().set_pv(pv, value, changed_pvs); return; }
  if (pv.find("BO") == 0) { models.set_bo_model().set_pv(pv, value, changed_pvs); return; }


  //if (pv.find("SIPS-") != std::string::npos) { models.set_si_model().set_power_supply(pv, value, std::vector<std::string>& changed_pvs); return; }
  //if (pv.find("BOPS-") != std::string::npos) { models.set_bo_model().set_power_supply(pv, value); return; }

}

void set_devicenames(const std::string& famname, const std::vector<std::string>& devicenames) {
  if (famname == "si_bpm")  { models.set_si_model().set_bpm_devicenames(devicenames); return; }
  if (famname == "si_ch")   { models.set_si_model().set_ch_devicenames(devicenames); return; }
  if (famname == "si_cv")   { models.set_si_model().set_cv_devicenames(devicenames); return; }
  if (famname == "si_quad") { models.set_si_model().set_quad_devicenames(devicenames); return; }
  if (famname == "bo_bpm")  { models.set_bo_model().set_bpm_devicenames(devicenames); return; }
  if (famname == "bo_ch")   { models.set_bo_model().set_ch_devicenames(devicenames); return; }
  if (famname == "bo_cv")   { models.set_bo_model().set_cv_devicenames(devicenames); return; }
}
