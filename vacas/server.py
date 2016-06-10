
import pcaspy
import record_names
import driver
import utils
import time


_DEFAULT_PREFIX = 'XVA-'
_WAIT_TIMEOUT   = 0.02

def run(prefix = _DEFAULT_PREFIX):

    server = pcaspy.SimpleServer()

    set_model_parameters_to_recordnames()
    rnames = record_names.get_all_recordnames()
    server.createPV(prefix, rnames)
    pcas_driver = driver.PCASDriver(rnames)

    utils.log(message1='prefix', message2=prefix, color='yellow', attr=None)
    utils.log(message1='server', message2='ready', color='yellow', attr=None)
    while True:
        pcas_driver.update_epics_memory_dynamic_pvs()
        server.process(_WAIT_TIMEOUT)

def set_model_parameters_to_recordnames():
    record_names.set_parameter('si_harmonic_number', int(driver.vacpp.get_model_parameter('si_harmonic_number')))
    record_names.set_parameter('bo_harmonic_number', int(driver.vacpp.get_model_parameter('bo_harmonic_number')))
