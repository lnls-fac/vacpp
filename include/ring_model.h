#ifndef SI_MODEL_H
#define SI_MODEL_H

#include "accelerator.h"
#include "beamcharge.h"

class RingModel : protected Accelerator {

public:
  RingModel(const double energy, const int harmonic_number);

protected:
  BeamCharge charge;
  
};


#endif
