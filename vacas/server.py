
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
    pcas_driver.thread.start()

    utils.log(message1='start', message2='server', color='yellow', attr=None)
    while True:
        #pcas_driver.epics_memory_update()
        utils.log(message1='TIME', message2='TIME', color='white', attr=None)
        server.process(INTERVAL)
