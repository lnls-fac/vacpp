
import pcaspy
import pvs
import driver
import utils

PREFIX = 'VA-'
INTERVAL = 0.1


def run():
    server = pcaspy.SimpleServer()
    server.createPV(PREFIX, pvs.pvdb)
    pcas_driver = driver.PCASDriver()
    utils.log(message1='start', message2='server', color='yellow', attr=None)
    while True:
        pcas_driver.epics_memory_update()
        server.process(INTERVAL)
