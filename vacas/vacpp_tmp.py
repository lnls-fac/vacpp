import time
import math

_pvs_dict = {
    'SIDI-CURRENT':300,
    'SIPA-LIFETIME':10,
}

t0 = time.time()

def update_current():
    global t0
    global _pvs_dict
    t = time.time()
    I, tau = _pvs_dict['SIDI-CURRENT'], 3600*_pvs_dict['SIPA-LIFETIME']
    newI = I * math.exp(-(t-t0)/tau)
    t0 = t;
    _pvs_dict['SIDI-CURRENT'] = newI

def cpp_to_python():
    update_current()
    return list(_pvs_dict.keys()), list(_pvs_dict.values())

def python_to_cpp(pv, value):
    return 0
