#include "ring_model.h"
#include "trackcpp.h"
#include <algorithm>
#include <string>

const std::string label_physics  = "PHYSICS";
const std::string label_hardware = "HARDWARE";

void ModelElement::get_model_indices(const std::string& pv, std::vector<int>& indices) const {

  indices.clear();

  // checks if pointer to accelerator ModelElement belongs to anmodel idx
  if (this->accelerator_ptr == nullptr) {
    std::cerr << "null pointer for ModelElement.accelerator_ptr" << std::endl;
    return;
  }

  // finds index of devicename corresponding to the pv
  int devicename_idx;
  for(devicename_idx=0; devicename_idx<this->devicenames.size();++devicename_idx) {
    if (pv.find(this->devicenames[devicename_idx]) != std::string::npos) break;
  }
  if (devicename_idx == this->devicenames.size()) {
    std::cerr << "ModelElement::get_model_indices: could not find a device name corresponding to pv '" << pv << "'" << std::endl;
    return;
  }

  const std::vector<Element>& latt = this->accelerator_ptr->lattice;
  std::vector<int> model_idx;
  for(auto k=0; k<this->famnames.size();++k) {
    for(int i=0; i<latt.size(); ++i) if (latt[i].fam_name == this->famnames[k]) model_idx.push_back(i);
  }
  std::sort(model_idx.begin(), model_idx.end());

  for(auto i=0; i<this->nr_segmentations; ++i) {
    const int idx = devicename_idx * this->nr_segmentations + i;
    if (idx < model_idx.size()) {
        indices.push_back(model_idx[idx]);
    } else {
      for(auto k=0; k<model_idx.size();++k) std::cout << model_idx[k] << std::endl;
      std::cerr << "ModelElement::get_model_indices:" << pv
                << ", dname_idx:" << devicename_idx
                << ", nr_dnames:" << this->devicenames.size()
                << ", nr_elements:" << model_idx.size() << std::endl;
    }
  }

}

RingModel::RingModel(const std::string& machine_) : machine(machine_) {
  this->accelerator.cavity_on = true;
  this->accelerator.radiation_on = true;
  this->accelerator.vchamber_on = true;
  this->state_changed = true;
  this->revolution_period = 0;
}

void RingModel::read_flatfile(std::string& fname) {
  Status::type status = read_flat_file(fname, this->accelerator);
  if (status != Status::success) {
    std::cerr << "vacpp: could not open flat file '" + fname + "' !" << std::endl;
    std::cerr.flush();
  }
}

Status::type RingModel::update_twiss() {

  std::string now_str = get_timestamp(); now_str = now_str.substr(1,now_str.size()-2);
  auto t_start = std::chrono::system_clock::now();
  Status::type status;

  // finds closed-orbit for using initial fixed point guess
  std::vector<Pos<double>> closed_orbit;
  if (this->twiss.empty()) {
    status = track_findorbit6(this->accelerator,
                              closed_orbit,
                              Pos<double>(0));
  } else {
    status = track_findorbit6(this->accelerator,
                              closed_orbit,
                              this->twiss[0].co);
  }
  if (status != Status::success) {
    this->twiss.clear();
    std::cout << now_str << "    : update twiss for machine " << this->machine << " yielded error '" << string_error_messages[status] << "'" << std::endl;
  }

  if (status != Status::success) {
    return status;
  }

  // calcs twiss
  status = calc_twiss(this->accelerator,
                      closed_orbit[0],
                      this->one_turn_matrix,
                      this->twiss);
  auto t_end = std::chrono::system_clock::now();
  std::chrono::duration<double, std::milli> duration = t_end - t_start;
  if (status != Status::success) {
    this->twiss.clear();
    std::cout << now_str << "    : update twiss for machine " << this->machine << " yielded error '" << string_error_messages[status] << "'" << std::endl;
  } else {
    std::cout << now_str << "    : update twiss for machine " << this->machine << ", " << duration.count() << " ms" <<  std::endl;
  }
  return status;

}

bool RingModel::update_state() {
  if (this->state_changed) {
    Status::type status;
    const double length = accelerator.get_length();
    this->revolution_period = length / light_speed;
    status = this->update_twiss();
    if (status != Status::success) this->beam_loss();
    this->state_changed = false;
    return true;
  } else {
    return false;
  }
}

void RingModel::beam_loss() {
  this->beam_charge.dump();
}

// --- define bpm, ch and cv elements ---

void RingModel::set_bpm_elements(const ModelElement& el) {
  this->bpm = el;
  this->bpm.accelerator_ptr = &(this->accelerator);
}

void RingModel::set_ch_elements(const ModelElement& el) {
  this->ch = el;
  this->ch.accelerator_ptr = &(this->accelerator);
}

void RingModel::set_cv_elements(const ModelElement& el) {
  this->cv = el;
  this->cv.accelerator_ptr = &(this->accelerator);
}

void RingModel::set_quad_elements(const ModelElement& el) {
  this->quad = el;
  this->quad.accelerator_ptr = &(this->accelerator);
}

void RingModel::set_bpm_devicenames(const std::vector<std::string>& devicenames) {
  this->bpm.devicenames.resize(devicenames.size());
  for(auto i=0; i<devicenames.size(); ++i) this->bpm.devicenames[i] = devicenames[i];
}

void RingModel::set_ch_devicenames(const std::vector<std::string>& devicenames) {
  this->ch.devicenames.resize(devicenames.size());
  for(auto i=0; i<devicenames.size(); ++i) this->ch.devicenames[i] = devicenames[i];
}

void RingModel::set_cv_devicenames(const std::vector<std::string>& devicenames) {
  this->cv.devicenames.resize(devicenames.size());
  for(auto i=0; i<devicenames.size(); ++i) this->cv.devicenames[i] = devicenames[i];
}

void RingModel::set_quad_devicenames(const std::vector<std::string>& devicenames) {
  this->quad.devicenames.resize(devicenames.size());
  for(auto i=0; i<devicenames.size(); ++i) this->quad.devicenames[i] = devicenames[i];
}


// --- get pv set ---

void RingModel::get_pv(const std::string& pv, std::vector<double>& values) {
  this->update_state();
  if (pv.find("SIPS-CH") == 0) return get_pv_ch(pv, values);
  if (pv.find("SIPS-CV") == 0) return get_pv_cv(pv, values);
  if (pv.find("SIPS-Q") == 0) return get_pv_quad(pv, values);
  if (pv.find("SIDI-BPM") == 0) return get_pv_bpm(pv, values);
  if (pv.find("SIDI-CURRENT") == 0) return get_pv_current(pv, values);
  if (pv.find("SIPA-LIFETIME") == 0) return get_pv_lifetime(pv, values);
  if (pv.find("SIMO-") == 0) return get_pv_model(pv, values);
  if (pv.find("BODI-CURRENT") == 0) return get_pv_current(pv, values);
  if (pv.find("BOPA-LIFETIME") == 0) return get_pv_lifetime(pv, values);
  std::cerr << "get_pv:" + pv + " is not defined!" << std::endl; // should never reach this line
  values.push_back(0.0);
}

void RingModel::get_pv_ch(const std::string& pv, std::vector<double>& values) {
  if (pv.find(label_physics) != std::string::npos) {
    std::vector<int> indices;
    this->ch.get_model_indices(pv, indices);
    if (indices.size() == 0) {
      std::cerr << "could not find ch devicename '" << pv << "'" << std::endl;
      values.push_back(0.0);
      return;
    }
    const Element& e = this->accelerator.lattice[indices[0]];
    if (e.polynom_b.size() < 1) {
      values.push_back(0.0);
    } else {
      const double hkick = e.polynom_b[0] * e.length;
      values.push_back(hkick);
    }
  } else {
    std::cerr << "RingModel::get_pv_ch for '" << pv << "' is not implemented yet." << std::endl;
    values.push_back(0.0);
  }
}

void RingModel::get_pv_cv(const std::string& pv, std::vector<double>& values) {
  if (pv.find(label_physics) != std::string::npos) {
    std::vector<int> indices;
    this->cv.get_model_indices(pv, indices);
    if (indices.size() == 0) {
      std::cerr << "could not find cv devicename '" << pv << "'" << std::endl;
      values.push_back(0.0);
      return;
    }
    const Element& e = this->accelerator.lattice[indices[0]];
    if (e.polynom_a.size() < 1) {
      values.push_back(0.0);
    } else {
      const double vkick = e.polynom_a[0] * e.length;
      values.push_back(vkick);
    }
  } else {
    std::cerr << "RingModel::get_pv_cv for '" << pv << "' is not implemented yet." << std::endl;
    values.push_back(0.0);
  }
}

void RingModel::get_pv_quad(const std::string& pv, std::vector<double>& values) {
  if (pv.find(label_physics) != std::string::npos) {
    std::vector<int> indices;
    this->quad.get_model_indices(pv, indices);
    if (indices.size() == 0) {
      std::cerr << "could not find quad devicename '" << pv << "'" << std::endl;
      values.push_back(0.0);
      return;
    }
    const Element& e = this->accelerator.lattice[indices[0]];
    if (e.polynom_a.size() < 2) {
      values.push_back(0.0);
    } else {
      const double quad = e.polynom_a[1];
      values.push_back(quad);
    }
  } else {
    std::cerr << "RingModel::get_pv_quad for '" << pv << "' is not implemented yet." << std::endl;
    values.push_back(0.0);
  }
}

void RingModel::get_pv_bpm(const std::string& pv, std::vector<double>& values) {

  const double charge = this->beam_charge.get_charge();
  if (charge == 0) {
    if (pv.find("FAM:") != std::string::npos) {
      for(auto i=0; i<this->bpm.devicenames.size(); ++i) values.push_back(0.0);
      return;
    } else {
      values.push_back(0.0); return;
    }
  }

  if (pv.find("FAM:MONIT:X") != std::string::npos) {
    for(auto i=0; i<this->bpm.devicenames.size(); ++i) {
      std::vector<int> indices;
      this->bpm.get_model_indices(this->bpm.devicenames[i], indices);
      if (indices.size() == 0) {
        std::cerr << "could not find bpm devicename" << std::endl;
        values.push_back(0.0);
      } else {
          values.push_back(this->twiss[indices[0]].co.rx); // [m]
      }
    }
  } else if (pv.find("FAM:MONIT:Y") != std::string::npos) {
    for(auto i=0; i<this->bpm.devicenames.size(); ++i) {
      std::vector<int> indices;
      this->bpm.get_model_indices(this->bpm.devicenames[i], indices);
      if (indices.size() == 0) {
        std::cerr << "could not find bpm devicename" << std::endl;
        values.push_back(0.0);
      } else {
          values.push_back(this->twiss[indices[0]].co.ry); // [m]
      }
    }
  } else {
    if (pv.find("MONIT:X") != std::string::npos) {
      std::vector<int> indices;
      this->bpm.get_model_indices(pv, indices);
      if (indices.size() == 0) {
        std::cerr << "could not find bpm devicename " << pv << std::endl;
        values.push_back(0.0);
      } else {
          values.push_back(this->twiss[indices[0]].co.rx); // [m]
      }
    } else if (pv.find("MONIT:Y") != std::string::npos) {
      std::vector<int> indices;
      this->bpm.get_model_indices(pv, indices);
      if (indices.size() == 0) {
        std::cerr << "could not find bpm devicename " << pv << std::endl;
        values.push_back(0.0);
      } else {
          values.push_back(this->twiss[indices[0]].co.ry); // [m]
      }
    } else {
      std::cerr << "could not find bpm devicename " << pv << std::endl;
      values.push_back(0.0);
    }
  }

}

void RingModel::get_pv_model(const std::string& pv, std::vector<double>& values) {
  if (pv.find("-BPM-FAM:SPOS") != std::string::npos) {
    for(auto i=0; i<this->bpm.devicenames.size(); ++i) {
      std::vector<int> indices;
      this->bpm.get_model_indices(this->bpm.devicenames[i], indices);
      if (indices.size() == 0) {
        std::cerr << "could not find bpm devicename" << std::endl;
        values.push_back(0.0);
      } else {
          values.push_back(this->twiss[indices[0]].spos); // [m]
      }
    }
  }
}

void RingModel::get_pv_current(const std::string& pv, std::vector<double>& values) {
  BeamCharge& bc = this->beam_charge;
  bc.update_bunches();
  const double si_revolution_period = this->get_revolution_period();
  if (pv.find("BUNCHES") != std::string::npos) {
    for(auto i=0; i<bc.bunches.size(); ++i) values.push_back(1000 * bc.bunches[i] / si_revolution_period);
  } else {
    const double value = 1000*bc.get_charge() / si_revolution_period;
    values.push_back(value);
  }
}

void RingModel::get_pv_lifetime(const std::string& pv, std::vector<double>& values) {
  BeamCharge& bc = this->beam_charge;
  bc.update_bunches();
  if (pv.find("BUNCHES") != std::string::npos) {
    for(auto i=0; i<bc.bunches.size(); ++i) {
      double lossrate = bc.get_singleparticle_lossrate() + bc.touschek_coefficient * bc.bunches[i];
      const double lifetime = 1.0 / lossrate / 3600; // [h]
      values.push_back(lifetime);
    }
  } else {
    const double value = bc.get_lifetime()/3600; // [h]
    values.push_back(value);
  }
}


// --- set pv set ---

Status::type RingModel::set_pv(const std::string& pv, double value, std::vector<std::string>& changed_pvs) {

  if (pv.find("PS-CH") != std::string::npos) this->_set_pv_ch(pv, value, changed_pvs);
  if (pv.find("PS-CV") != std::string::npos) this->_set_pv_cv(pv, value, changed_pvs);

}

void RingModel::_add_bpm_pvs_changed_list(std::vector<std::string>& changed_pvs) const {
    for(auto i=0; i<this->bpm.devicenames.size(); ++i) {
      changed_pvs.push_back(this->machine + "DI-" + this->bpm.devicenames[i]+":MONIT:X");
      changed_pvs.push_back(this->machine + "DI-" + this->bpm.devicenames[i]+":MONIT:Y");
    }
    changed_pvs.push_back(this->machine + "DI-BPM-FAM:MONIT:X");
    changed_pvs.push_back(this->machine + "DI-BPM-FAM:MONIT:Y");
}

void RingModel::_add_all_pvs_changed_list(std::vector<std::string>& changed_pvs) const {
  this->_add_bpm_pvs_changed_list(changed_pvs);
}

Status::type RingModel::_set_pv_ch(const std::string& pv, double value, std::vector<std::string>& changed_pvs) {
  std::vector<int> indices;
  this->ch.get_model_indices(pv, indices);
  if (indices.size() == 0) { std::cerr << "could not find ch devicename" << std::endl; return Status::inconsistent_dimensions; }
  Element& e = this->accelerator.lattice[indices[0]];
  const unsigned int main_order = 1;
  unsigned int max_order = e.polynom_a.size() > e.polynom_b.size() ? e.polynom_a.size() : e.polynom_b.size();
  max_order = main_order > max_order ? main_order : max_order;
  e.polynom_a.resize(max_order,0);
  e.polynom_b.resize(max_order,0);
  e.polynom_b[main_order-1] = - value / e.length;
  this->_add_all_pvs_changed_list(changed_pvs);
  this->state_changed = true;
  return Status::success;
}

Status::type RingModel::_set_pv_cv(const std::string& pv, double value, std::vector<std::string>& changed_pvs) {
  // std::vector<int> indices;
  // this->cv.get_model_indices(pv, indices);
  // if (indices.size() == 0) { std::cerr << "could not find cv devicename" << std::endl; return Status::inconsistent_dimensions; }
  // Element& e = this->accelerator.lattice[indices[0]];
  // if (e.polynom_a.size() < 1) {
  //   e.polynom_a.push_back(value / e.length);
  // } else {
  //   e.polynom_a[0] = value / e.length;
  // }
  // this->_add_bpm_pvs(changed_pvs);
  // this->state_changed = true;
  // return Status::success;
  std::vector<int> indices;
  this->cv.get_model_indices(pv, indices);
  if (indices.size() == 0) { std::cerr << "could not find cv devicename" << std::endl; return Status::inconsistent_dimensions; }
  Element& e = this->accelerator.lattice[indices[0]];
  const unsigned int main_order = 1;
  unsigned int max_order = e.polynom_a.size() > e.polynom_b.size() ? e.polynom_a.size() : e.polynom_b.size();
  max_order = main_order > max_order ? main_order : max_order;
  e.polynom_a.resize(max_order,0);
  e.polynom_b.resize(max_order,0);
  e.polynom_a[main_order-1] = value / e.length;
  this->_add_all_pvs_changed_list(changed_pvs);
  this->state_changed = true;
  return Status::success;
}

Status::type RingModel::_set_pv_quad(const std::string& pv, double value, std::vector<std::string>& changed_pvs) {
  std::vector<int> indices;
  this->quad.get_model_indices(pv, indices);
  if (indices.size() == 0) { std::cerr << "could not find quad devicename" << std::endl; return Status::inconsistent_dimensions; }
  Element& e = this->accelerator.lattice[indices[0]];
  const unsigned int main_order = 2;
  unsigned int max_order = e.polynom_a.size() > e.polynom_b.size() ? e.polynom_a.size() : e.polynom_b.size();
  max_order = main_order > max_order ? main_order : max_order;
  e.polynom_a.resize(max_order,0);
  e.polynom_b.resize(max_order,0);
  e.polynom_a[main_order-1] = value;
  this->_add_all_pvs_changed_list(changed_pvs);
  this->state_changed = true;
  return Status::success;
}
