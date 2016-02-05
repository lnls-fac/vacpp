#ifndef VACPP_ACCELERATOR_MODEL_H
#define VACPP_ACCELERATOR_MODEL_H

#include <trackcpp/accelerator.h>
#include <trackcpp/flat_file.h>

#include "model.h"


class AcceleratorModel : public Model {
public:
    AcceleratorModel(std::string flat_file_name, std::atomic<bool>* stop_flag);
    virtual void _update_state();
private:
    Accelerator _accelerator;
};


#endif /* VACPP_ACCELERATOR_MODEL_H */
