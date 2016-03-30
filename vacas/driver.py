
import sys
import pcaspy
import utils
#import vacpp

sys.path.append('../build/release/package')
import vacpp

#import threading

class PCASDriver(pcaspy.Driver):

    def __init__(self):
        super(PCASDriver, self).__init__()
        #self.driver = vacpp.VaDriver()
        #self.thread = threading.Thread(group=None,target=PCASDriver.epics_memory_update, args=(self, self.driver))

    def epics_memory_update(self):
        pvs = vacpp.CppStringVector()
        values = vacpp.CppDoubleVector()
        vacpp.cpp_to_python(pvs, values)
        for i in range(len(pvs)):
            self.setParam(pvs[i], values[i])

    def read(self, reason):
        value = super().read(reason)
        utils.log(message1='read', message2=reason+':'+str(value), color=None, attr=None)
        return value

    def write(self, reason, value):
        utils.log(message1='write', message2=reason+':'+str(value), color='red', attr=None)
        self.setParam(reason, float(value))
        return vacpp.python_to_cpp(reason, value)
