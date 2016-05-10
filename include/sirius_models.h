#ifndef SIRIUS_MODELS_H
#define SIRIUS_MODELS_H

#include "ring_model.h"

#include <limits>

const double bo_energy               = 3e9;          // [eV]
const double bo_current_init         = 1 * 3e-3;     // [mA]
const double bo_elastic_lifetime     = 37.45 * 3600; // [s]
const double bo_inelastic_lifetime   = 92.27 * 3600; // [s]
const double bo_touschek_lifetime    = 6 * 3600;     // [s] (at 300mA with uniform filling)
const double bo_quantum_lifetime     = std::numeric_limits<double>::infinity(); // [s]
const char   bo_model_flatfile[]     = "/home/fac_files/siriusdb/models_flatfiles/bo-v02-01-m0.txt";

const double si_energy               = 3e9;          // [eV]
const double si_current_init         = 1 * 300e-3;   // [mA]
const double si_elastic_lifetime     = 37.45 * 3600; // [s]
const double si_inelastic_lifetime   = 92.27 * 3600; // [s]
const double si_touschek_lifetime    = 6 * 3600;     // [s] (at 300mA with uniform filling)
const double si_quantum_lifetime     = std::numeric_limits<double>::infinity(); // [s]
const char   si_model_flatfile[]     = "/home/fac_files/siriusdb/models_flatfiles/si-v16-01-s05-01.txt";

class SiriusModels {

public:
  SiriusModels();
  RingModel& get_si_model() { return si_model; }
  RingModel& set_si_model() { return si_model; }
  RingModel& get_bo_model() { return bo_model; }
  RingModel& set_bo_model() { return bo_model; }

protected:
  RingModel si_model;
  RingModel bo_model;

};

#endif
