#include "accelerator_model.h"


AcceleratorModel::AcceleratorModel(std::string flat_file_name,
    std::atomic<bool>* stop_flag) : Model(stop_flag)
{
    read_flat_file(flat_file_name, this->_accelerator);
}
void AcceleratorModel::_update_state()
{
    // TODO: add code
}
