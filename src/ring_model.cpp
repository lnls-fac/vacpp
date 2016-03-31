#include "ring_model.h"

RingModel::RingModel(const double energy, const int harmonic_number) {
  this->energy = energy;
  this->cavity_on = true;
  this->radiation_on = true;
  this->vchamber_on = true;
  this->harmonic_number = harmonic_number;
}
