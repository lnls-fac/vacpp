#include "sirius_models.h"
#include "auxiliary.h"

SiriusModels models;

bool SiriusModels::update_all() {
  bool changed = false;
  if (si_model.update_state()) {
    changed = true;
  }
  if (bo_model.update_state()) {
    changed = true;
  }
  return changed;
}

SiriusModels::SiriusModels() :
  si_model("si_model"),
  bo_model("bo_model") {

  // configures BO model
  std::string bo_flatfile(bo_model_flatfile);
  bo_model.read_flatfile(bo_flatfile);
  const int    bo_harmonic_number = bo_model.get_accelerator().harmonic_number;
  const double bo_revolution_period = bo_model.get_revolution_period();
  const double bo_touschek_coefficient = bo_harmonic_number * 1.0 / (bo_touschek_lifetime * 300e-3 * bo_revolution_period);
  const double bo_charge_init = bo_current_init * bo_revolution_period;
  bo_model.beam_charge.set_bunches(bo_charge_init, bo_harmonic_number);
  bo_model.beam_charge.set_elastic_lifetime(bo_elastic_lifetime);
  bo_model.beam_charge.set_inelastic_lifetime(bo_inelastic_lifetime);
  bo_model.beam_charge.set_quantum_lifetime(bo_quantum_lifetime);
  bo_model.beam_charge.set_touschek_coefficient(bo_touschek_coefficient);

  ModelElement bo_bpm({"bpm"}, 1); bo_model.set_bpm_elements(bo_bpm);
  ModelElement bo_ch({"ch"}, 1); bo_model.set_ch_elements(bo_ch);
  ModelElement bo_cv({"cv"}, 1); bo_model.set_cv_elements(bo_cv);

  // configures SI model
  std::string si_flatfile(si_model_flatfile);
  si_model.read_flatfile(si_flatfile);
  const int    si_harmonic_number = si_model.get_accelerator().harmonic_number;
  const double si_revolution_period = si_model.get_revolution_period();
  const double si_touschek_coefficient = si_harmonic_number * 1.0 / (si_touschek_lifetime * 300e-3 * si_revolution_period);
  const double si_charge_init = si_current_init * si_revolution_period;
  si_model.beam_charge.set_bunches(si_charge_init, si_harmonic_number);
  si_model.beam_charge.set_elastic_lifetime(si_elastic_lifetime);
  si_model.beam_charge.set_inelastic_lifetime(si_inelastic_lifetime);
  si_model.beam_charge.set_quantum_lifetime(si_quantum_lifetime);
  si_model.beam_charge.set_touschek_coefficient(si_touschek_coefficient);

  const std::vector<std::string> ch_famnames = {"sfa0","sfb0","sfp0","sda1","sdb1","sdp1","sfa2","sfb2","sfp2"};
  const std::vector<std::string> cv_famnames = {"sfa0","sfb0","sfp0","sda1","sdb1","sdp1","sda3","sdb3","sdp3","cv"};

  ModelElement si_bpm({"bpm"}, 1);    si_model.set_bpm_elements(si_bpm);
  ModelElement si_ch(ch_famnames, 1); si_model.set_ch_elements(si_ch);
  ModelElement si_cv(cv_famnames, 1); si_model.set_cv_elements(si_cv);

}
