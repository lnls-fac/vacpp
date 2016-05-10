#include "ring_model.h"
#include "flat_file.h"
#include "auxiliary.h"

RingModel::RingModel() {
  this->cavity_on = true;
  this->radiation_on = true;
  this->vchamber_on = true;
  this->state_changed = true;
  this->revolution_period = 0;
}

void RingModel::read_flatfile(std::string& fname) {
  Status::type status = read_flat_file(fname, this->accelerator);
  if (status != Status::success) {
    std::cerr << "! could not open flat file '" + fname + "'" << std::endl;
    std::cerr.flush();
  }
}

void RingModel::update_state() {
  const double length = accelerator.get_length();
  this->revolution_period = length / light_speed;
  this->state_changed = false;
}
