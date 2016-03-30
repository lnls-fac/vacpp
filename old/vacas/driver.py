
import sys
import pcaspy

sys.path.append('../build/release/package')
import vacpp


class PCASDriver(pcaspy.Driver):

    def __init__(self):
        super().__init__()
        self.driver = vacpp.VaDriver()

    def read(self, reason):
        return super().read(reason)

    def write(self, reason, value):
        self.setParam(reason, value)
        self.driver.exported_function(reason+':'+str(value))
        return 0
