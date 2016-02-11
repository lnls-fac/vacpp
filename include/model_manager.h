#ifndef VACPP_MODEL_MANAGER_H
#define VACPP_MODEL_MANAGER_H

#include <stdexcept>
#include <string>
#include <unordered_map>

#include "model.h"
#include "accelerator_model.h"


static std::vector<std::string> model_prefixes = {
    "si", "bo"
};


class ModelManager {
public:
    ModelManager(std::atomic<bool>* is_running_flag);
    ~ModelManager();
    Model* get_model_by_prefix(std::string prefix);
private:
    std::unordered_map<std::string, Model*> _models;
};


class ModelFactory {
public:
    static Model* create_model(const std::string& prefix,
        std::atomic<bool>* is_running_flag);
private:
    static Model* _create_accelerator_model(const std::string& prefix,
        std::atomic<bool>* is_running_flag);
};


#endif /* VACPP_MODEL_MANAGER_H */
