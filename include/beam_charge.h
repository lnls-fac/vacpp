#ifndef BEAMCHARGE_H
#define BEAMCHARGE_H

#include <vector>
#include <chrono>

class BeamCharge {

public:

  BeamCharge();

  const std::vector<double>& get_bunches() { update_bunches(); return bunches; }
  double get_touschek_coefficient() const { return touschek_coefficient; };
  double get_elastic_lifetime() const { return 1.0 / elastic_lossrate; };
  double get_inelastic_lifetime() const { return 1.0 / inelastic_lossrate; };
  double get_quantum_lifetime() const { return 1.0 / quantum_lossrate; };
  double get_singleparticle_lossrate() const { return elastic_lossrate + inelastic_lossrate + quantum_lossrate; }
  double get_singleparticle_lifetime() const { return 1.0 / this->get_singleparticle_lossrate(); }

  double get_touschek_lifetime();

  double get_lossrate();
  double get_lifetime();
  double get_charge();

  BeamCharge& update_bunches();
  BeamCharge& dump();
  BeamCharge& inject(const BeamCharge& beam_charge, const double delay = 0.0);
  BeamCharge  eject();
  BeamCharge& set_bunches(const double& charge, int nr_bunches = 1);
  BeamCharge& set_elastic_lifetime(const double& elastic_lifetime);
  BeamCharge& set_inelastic_lifetime(const double& inelastic_lifetime);
  BeamCharge& set_quantum_lifetime(const double& quantum_lifetime);
  BeamCharge& set_touschek_coefficient(const double& touschek_coefficient);

protected:

  std::vector<double> bunches;
  double elastic_lossrate;
  double inelastic_lossrate;
  double quantum_lossrate;
  double touschek_coefficient;

  std::chrono::system_clock::time_point last_time;

};

#endif
