#include "sirius_models.h"
#include "auxiliary.h"

SiriusModels::SiriusModels() {

  // configures BO model
  std::string bo_flatfile(bo_model_flatfile);
  bo_model.read_flatfile(bo_flatfile);
  const int    bo_harmonic_number = bo_model.get_accelerator().harmonic_number;
  const double bo_circumference = bo_model.get_accelerator().get_length();
  const double bo_revolution_period = bo_model.get_revolution_period();
  const double bo_touschek_coefficient = bo_harmonic_number * 1.0 / (bo_touschek_lifetime * 300e-3 * bo_revolution_period);
  const double bo_charge_init = bo_current_init * bo_revolution_period;
  bo_model.beam_charge.set_bunches(bo_charge_init, bo_harmonic_number);
  bo_model.beam_charge.set_elastic_lifetime(bo_elastic_lifetime);
  bo_model.beam_charge.set_inelastic_lifetime(bo_inelastic_lifetime);
  bo_model.beam_charge.set_quantum_lifetime(bo_quantum_lifetime);
  bo_model.beam_charge.set_touschek_coefficient(bo_touschek_coefficient);

  // configures SI model
  std::string si_flatfile(si_model_flatfile);
  si_model.read_flatfile(si_flatfile);
  const int    si_harmonic_number = si_model.get_accelerator().harmonic_number;
  const double si_circumference = si_model.get_accelerator().get_length();
  const double si_revolution_period = si_model.get_revolution_period();
  const double si_touschek_coefficient = si_harmonic_number * 1.0 / (si_touschek_lifetime * 300e-3 * si_revolution_period);
  const double si_charge_init = si_current_init * si_revolution_period;
  //si_model.set_accelerator().cavity_on = false; // to test tracking
  //si_model.set_accelerator().radiation_on = false;
  si_model.beam_charge.set_bunches(si_charge_init, si_harmonic_number);
  si_model.beam_charge.set_elastic_lifetime(si_elastic_lifetime);
  si_model.beam_charge.set_inelastic_lifetime(si_inelastic_lifetime);
  si_model.beam_charge.set_quantum_lifetime(si_quantum_lifetime);
  si_model.beam_charge.set_touschek_coefficient(si_touschek_coefficient);

}
