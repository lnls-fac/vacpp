#ifndef SI_MODEL_H
#define SI_MODEL_H

#include "trackcpp.h"
#include "beam_charge.h"

#include <vector>
#include <string>

extern const std::string label_hardware;
extern const std::string label_physics;


class ModelElement {

public:
  ModelElement(const std::vector<std::string>& famnames_ = {},
               const unsigned int nr_segmentations_ = 1,
               const Accelerator* accelerator_ptr_ = 0) :
               nr_segmentations(nr_segmentations_),
               accelerator_ptr(accelerator_ptr_) {
                 for(auto i=0; i<famnames_.size();++i) this->famnames.push_back(famnames_[i]);
               }

  const Accelerator* accelerator_ptr;
  std::vector<std::string> devicenames;
  std::vector<std::string> famnames;
  unsigned int nr_segmentations;
  void get_model_indices(const std::string& pv, std::vector<int>& indices) const;

};

class RingModel {

public:

  RingModel(const std::string& label_);

  BeamCharge   beam_charge;

  Accelerator& get_accelerator() { if (state_changed) update_state(); return accelerator; }
  Accelerator& set_accelerator() { state_changed = true; return accelerator; }

  void         beam_loss();
  void         read_flatfile(std::string& fname);
  double       get_revolution_period() { if (state_changed) update_state(); return revolution_period; }
  bool         update_state();
  void         get_pv(const std::string& pv, std::vector<double>& values);
  Status::type set_pv(const std::string& pv, double value, std::vector<std::string>& changed_pvs);

  void         get_pv_model(const std::string& pv, std::vector<double>& values);
  void         get_pv_bpm(const std::string& pv, std::vector<double>& values);
  void         get_pv_ch(const std::string& pv, std::vector<double>& values);
  void         get_pv_cv(const std::string& pv, std::vector<double>& values);
  void         get_pv_current(const std::string& pv, std::vector<double>& values);
  void         get_pv_lifetime(const std::string& pv, std::vector<double>& values);

  void         set_bpm_devicenames(const std::vector<std::string>& devicenames);
  void         set_ch_devicenames(const std::vector<std::string>& devicenames);
  void         set_cv_devicenames(const std::vector<std::string>& devicenames);

  void         set_bpm_elements(const ModelElement& el);
  void         set_ch_elements(const ModelElement& el);
  void         set_cv_elements(const ModelElement& el);

protected:

  std::string               label;
  Accelerator               accelerator;
  double                    revolution_period;
  bool                      state_changed;
  Matrix                    one_turn_matrix;
  std::vector<Twiss>        twiss;
  ModelElement              bpm, ch, cv;

  Status::type update_twiss();
  void         _add_bpm_pvs(std::vector<std::string>& changed_pvs) const;
  Status::type _set_pv_ch(const std::string& pv, double value, std::vector<std::string>& changed_pvs);
  Status::type _set_pv_cv(const std::string& pv, double value, std::vector<std::string>& changed_pvs);


};


#endif
