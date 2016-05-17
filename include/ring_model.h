#ifndef SI_MODEL_H
#define SI_MODEL_H

#include "accelerator.h"
#include "auxiliary.h"
#include "pos.h"
#include "beam_charge.h"

#include <vector>
#include <string>

class RingModel {

public:

  RingModel(const std::string& label_ = "");
  BeamCharge beam_charge;
  Accelerator& get_accelerator() { if (state_changed) update_state(); return accelerator; }
  Accelerator& set_accelerator() { state_changed = true; return accelerator; }
  void read_flatfile(std::string& fname);
  double get_revolution_period() { if (state_changed) update_state(); return revolution_period; }

protected:

  std::string               label;
  Accelerator               accelerator;
  double                    revolution_period;
  bool                      state_changed;
  std::vector<Pos<double> > closed_orbit;
  Pos<double>               fixed_point_guess;

  Status::type update_closed_orbit();
  void         update_state();

};


#endif
