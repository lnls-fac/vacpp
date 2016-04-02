#ifndef SIRIUS_MODELS_H
#define SIRIUS_MODELS_H

#include "ring_model.h"

class SiriusModels {

public:
  SiriusModels();
  const RingModel& get_si_model() const { return si_model; }
        RingModel& set_si_model()       { return si_model; }

protected:
  RingModel si_model;
  RingModel bo_model;

};

#endif
