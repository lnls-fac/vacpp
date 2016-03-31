#include <cmath>
#include "beamcharge.h"

BeamCharge& BeamCharge::set_bunches(const double& charge, int nr_bunches) {
  this->bunches.clear();
  for(unsigned int i = 0; i < nr_bunches; ++i) {
    this->bunches.push_back(charge/nr_bunches);
  }
  this->last_time = std::chrono::system_clock::now();
}

BeamCharge& BeamCharge::update_bunches() {
  std::chrono::system_clock::time_point this_time = std::chrono::system_clock::now();
  double delta_time = (1.0e-6/3600.0) * std::chrono::duration_cast<std::chrono::microseconds>(this_time - this->last_time).count();
  for(unsigned int i=0; i<this->bunches.size(); ++i) {
    this->bunches[i] *= exp(-delta_time/this->inelastic_lifetime);
  }
  this->last_time = this_time;
}

BeamCharge& BeamCharge::set_elastic_lifetime(const double& elastic_lifetime) {
  this->update_bunches();
  this->elastic_lifetime = elastic_lifetime;
}
