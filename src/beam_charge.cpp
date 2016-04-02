#include <cmath>
#include "beam_charge.h"

#include <iostream>

BeamCharge::BeamCharge() :
  elastic_lossrate(0.0),
  inelastic_lossrate(0.0),
  quantum_lossrate(0.0),
  touschek_coefficient(0.0) {
}

BeamCharge& BeamCharge::set_bunches(const double& charge, int nr_bunches) {
  this->bunches.clear();
  for(unsigned int i = 0; i < nr_bunches; ++i) {
    this->bunches.push_back(charge/nr_bunches);
  }
  this->last_time = std::chrono::system_clock::now();
  return *this;
}

BeamCharge& BeamCharge::update_bunches() {

  // calcs time interval since last charge update
  std::chrono::system_clock::time_point this_time = std::chrono::system_clock::now();
  double delta_time = 1.0e-6 * std::chrono::duration_cast<std::chrono::microseconds>(this_time - this->last_time).count();

  // updates each bunch charge
  const double singleparticle_lossrate = elastic_lossrate + inelastic_lossrate + quantum_lossrate;
  if (singleparticle_lossrate == 0) {
    for(unsigned int i = 0; i < this->bunches.size(); ++i) {
      this->bunches[i] /= 1.0 + this->bunches[i] * touschek_coefficient * delta_time;
    }
  } else {
    const double spexp = exp(-delta_time * singleparticle_lossrate);
    for(unsigned int i = 0; i < this->bunches.size(); ++i) {
      this->bunches[i] *= spexp / (1.0 + touschek_coefficient * this->bunches[i] * (1.0 - spexp) / singleparticle_lossrate);
    }
  }

  // updates timestamp
  this->last_time = this_time;

  return *this;

}

BeamCharge& BeamCharge::set_elastic_lifetime(const double& elastic_lifetime) {
  this->update_bunches();
  this->elastic_lossrate = 1.0 / elastic_lifetime;
}

BeamCharge& BeamCharge::set_inelastic_lifetime(const double& inelastic_lifetime) {
  this->update_bunches();
  this->inelastic_lossrate = 1.0 / inelastic_lifetime;
}

BeamCharge& BeamCharge::set_quantum_lifetime(const double& quantum_lifetime) {
  this->update_bunches();
  this->quantum_lossrate = 1.0 / quantum_lifetime;
}

BeamCharge& BeamCharge::set_touschek_coefficient(const double& touschek_coefficient) {
  this->update_bunches();
  this->touschek_coefficient = touschek_coefficient;
}

double BeamCharge::get_charge() {
  this->update_bunches();
  double charge = 0.0;
  for(unsigned int i = 0; i < this->bunches.size(); ++i) {
    charge += this->bunches[i];
  }
  return charge;
}

double BeamCharge::get_lossrate() {
  this->update_bunches();
  double q = 0.0; double q2 = 0.0;
  for(unsigned int i = 0; i < this->bunches.size(); ++i) {
    q  += this->bunches[i];
    q2 += this->bunches[i] * this->bunches[i];
  }
  double lossrate = this->get_singleparticle_lossrate();
  if (q != 0.0) lossrate += touschek_coefficient * q2 / q;
  return lossrate;
}

double BeamCharge::get_lifetime() {
  return 1.0 / this->get_lossrate();
}
