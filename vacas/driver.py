
import sys
import pcaspy
import utils
import time
import threading


sys.path.append('./build/release/package')
import vacpp

#import threading

class PCASDriver(pcaspy.Driver):

    def __init__(self):
        super(PCASDriver, self).__init__()
        #self.update_thread = threading.Thread(group=None,target=PCASDriver.update,args=(self,))
        self.is_running = True

    def update_epics_memory(self):
        # receives updated pvs from c++ library
        pvs = vacpp.CppStringVector()
        values = vacpp.CppDoubleVector()
        vacpp.cpp_to_python(pvs, values)
        # sets EPICS memory with those updated pv values
        for i in range(len(pvs)):
            self.setParam(pvs[i], values[i])
        self.updatePVs()

    # def update(self):
    #     while self.is_running:
    #         self.update_epics_memory()
    #         time.sleep(0.1)

    def read(self, reason):
        value = super().read(reason)
        utils.log(message1='read', message2=reason+':'+str(value), color=None, attr=None)
        return value

    def write(self, reason, value):
        utils.log(message1='write', message2=reason+':'+str(value), color='red', attr=None)
        self.setParam(reason, float(value))
        return vacpp.python_to_cpp(reason, value)
