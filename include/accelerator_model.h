#ifndef VACPP_ACCELERATOR_MODEL_H
#define VACPP_ACCELERATOR_MODEL_H

#include <trackcpp/accelerator.h>
#include <trackcpp/flat_file.h>

#include "model.h"


class AcceleratorModel : public Model {
public:
    AcceleratorModel(std::string flat_file_name);
    void print();
private:
    Accelerator _accelerator;
    
    virtual void _update_state();
};


#endif /* VACPP_ACCELERATOR_MODEL_H */
