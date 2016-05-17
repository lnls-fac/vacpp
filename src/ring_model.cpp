#include "ring_model.h"
//#include "flat_file.h"
//#include "auxiliary.h"
//#include "tracking.h"
#include "trackcpp.h"

RingModel::RingModel(const std::string& label_) : label(label_) {
  this->accelerator.cavity_on = true;
  this->accelerator.radiation_on = true;
  this->accelerator.vchamber_on = true;
  this->state_changed = true;
  this->revolution_period = 0;
  this->fixed_point_guess = Pos<double>(0);
}

void RingModel::read_flatfile(std::string& fname) {
  Status::type status = read_flat_file(fname, this->accelerator);
  if (status != Status::success) {
    std::cerr << "vacpp: could not open flat file '" + fname + "' !" << std::endl;
    std::cerr.flush();
  }
}

void RingModel::update_state() {
  const double length = accelerator.get_length();
  this->revolution_period = length / light_speed;
  this->update_closed_orbit();
  this->state_changed = false;
}

Status::type RingModel::update_closed_orbit() {

  std::string now_str = get_timestamp();
  auto t_start = std::chrono::system_clock::now();
  Status::type status = track_findorbit6(this->accelerator,
                                         this->closed_orbit,
                                         this->fixed_point_guess);
  auto t_end = std::chrono::system_clock::now();
  std::chrono::duration<double, std::milli> duration = t_end - t_start;
  std::cout << now_str.substr(1,now_str.size()-2) << "    : update closed orbit for " << this->label << ", " << duration.count() << " ms" <<  std::endl;

}
