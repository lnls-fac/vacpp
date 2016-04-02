#include "sirius_models.h"

#include <limits>

static const double si_energy               = 3e9;        // [eV]
static const int    si_harmonic_number      = 864;        // [n.u.]
static const double si_elastic_lifetime     = 37.45*3600; // [s]
static const double si_inelastic_lifetime   = 92.27*3600; // [s]
static const double si_quantum_lifetime     = std::numeric_limits<double>::infinity(); // [s]
static const double si_touschek_coefficient = 2.16e-1;    // [1/(C.s)]
static const double si_charge_init          = 300.0;     // [C]

SiriusModels::SiriusModels() {
  si_model.set_accelerator().energy = si_energy;
  si_model.set_accelerator().harmonic_number = si_harmonic_number;
  si_model.beam_charge.set_bunches(si_charge_init, si_harmonic_number);
  si_model.beam_charge.set_elastic_lifetime(si_elastic_lifetime);
  si_model.beam_charge.set_elastic_lifetime(si_inelastic_lifetime);
  si_model.beam_charge.set_quantum_lifetime(si_quantum_lifetime);
  si_model.beam_charge.set_touschek_coefficient(si_touschek_coefficient);
}
