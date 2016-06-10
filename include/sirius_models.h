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

const unsigned int si_bpm_nr_model_segmentation  = 1;
const unsigned int si_ch_nr_model_segmentation   = 1;
const unsigned int si_cv_nr_model_segmentation   = 1;
const unsigned int si_quad_nr_model_segmentation = 1;

const std::vector<std::string> si_bpm_famnames  = {"bpm"};
const std::vector<std::string> si_ch_famnames   = {"sfa0","sfb0","sfp0","sda1","sdb1","sdp1","sfa2","sfb2","sfp2"};
const std::vector<std::string> si_cv_famnames   = {"sfa0","sfb0","sfp0","sda1","sdb1","sdp1","sda3","sdb3","sdp3","cv"};
const std::vector<std::string> si_quad_famnames = {"qfa","qda","q1","q2","q3","q4","qdb1","qfb","qdb2"};

class SiriusModels {

public:
  SiriusModels();
  RingModel& get_si_model() { return si_model; }
  RingModel& set_si_model() { return si_model; }
  RingModel& get_bo_model() { return bo_model; }
  RingModel& set_bo_model() { return bo_model; }
  bool       update_all();

protected:
  RingModel   si_model;
  RingModel   bo_model;

};

extern SiriusModels models; // global variable defined in sirius_models.cpp


#endif
