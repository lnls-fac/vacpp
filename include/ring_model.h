#ifndef SI_MODEL_H
#define SI_MODEL_H

#include "accelerator.h"
#include "beam_charge.h"

class RingModel : protected Accelerator {

public:
  RingModel();
  BeamCharge beam_charge;
  Accelerator& get_accelerator() { if (state_changed) update_state(); return accelerator; }
  Accelerator& set_accelerator() { state_changed = true; return accelerator; }
  void read_flatfile(std::string& fname);
  double get_revolution_period() { if (state_changed) update_state(); return revolution_period; }

protected:
  double      revolution_period;
  bool        state_changed;
  Accelerator accelerator;
  void update_state();

};


#endif
