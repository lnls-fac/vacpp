
import pcaspy
import pvs
import driver
import utils
import time

PREFIX       = 'VA-'
WAIT_TIMEOUT = 0.02

def run():
    
    server = pcaspy.SimpleServer()
    server.createPV(PREFIX, pvs.pvdb)
    pcas_driver = driver.PCASDriver()

    utils.log(message1='start', message2='server', color='yellow', attr=None)
    while True:
        pcas_driver.update_epics_memory()
        server.process(WAIT_TIMEOUT)
        #time.sleep(WAIT_TIMEOUT)
