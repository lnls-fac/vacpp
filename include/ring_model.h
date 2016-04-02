#ifndef SI_MODEL_H
#define SI_MODEL_H

#include "accelerator.h"
#include "beam_charge.h"

class RingModel : protected Accelerator {

public:
  RingModel();
  BeamCharge beam_charge;
  const Accelerator& get_accelerator() const { return accelerator; }
  Accelerator&       set_accelerator()       { return accelerator; }

protected:
  Accelerator accelerator;

};


#endif
