#include "accelerator_model.h"


AcceleratorModel::AcceleratorModel(std::string flat_file_name)
{
    read_flat_file(flat_file_name, this->_accelerator);
}

void AcceleratorModel::_update_state()
{
    // TODO: add code
}

void AcceleratorModel::print()
{
    // FIXME: remove debug code
    std::cout << _accelerator.harmonic_number << std::endl;
}
