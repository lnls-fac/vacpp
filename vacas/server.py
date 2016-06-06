
import pcaspy
import record_names
import driver
import utils
import time

_DEFAULT_PREFIX = 'XXVA-'
_WAIT_TIMEOUT   = 0.02

def run(prefix = _DEFAULT_PREFIX):

    server = pcaspy.SimpleServer()

    rnames = record_names.get_all_recordnames()
    server.createPV(prefix, rnames)
    pcas_driver = driver.PCASDriver()

    utils.log(message1='prefix', message2=prefix, color='yellow', attr=None)
    utils.log(message1='server', message2='ready', color='yellow', attr=None)
    while True:
        pcas_driver.update_epics_memory_dynamic_pvs()
        server.process(_WAIT_TIMEOUT)
