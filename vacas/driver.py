
import sys
import pcaspy
import utils
import time
import threading
import record_names
#import threading


utils.print_banner(*record_names.get_pvs_lists())
utils.log(message1='driver', message2='init', color='yellow', attr=None)


sys.path.append('./build/release/package')
import vacpp


class PCASDriver(pcaspy.Driver):

    def __init__(self, rnames):
        super(PCASDriver, self).__init__()
        #self.update_thread = threading.Thread(group=None,target=PCASDriver.update,args=(self,))
        self.rnames = rnames
        self._init_vacpp_structures()
        self._update_all_pvs()
        self.is_running = True


    def _init_vacpp_structures(self):
        devicenames = vacpp.CppStringVector()
        # bpms
        dnames = record_names.get_bpm_devicenames()
        #print(dnames)
        devicenames.resize(len(dnames))
        for i in range(len(dnames)): devicenames[i] = dnames[i]
        vacpp.set_devicenames("si_bpm", devicenames);
        utils.log(message1='driver', message2='set si_bpm_devicenames', color='yellow', attr=None)
        # ch
        dnames = record_names.get_ch_devicenames()
        devicenames.resize(len(dnames))
        for i in range(len(dnames)): devicenames[i] = dnames[i]
        vacpp.set_devicenames("si_ch", devicenames);
        utils.log(message1='driver', message2='set si_ch_devicenames', color='yellow', attr=None)
        # cv
        dnames = record_names.get_cv_devicenames()
        devicenames.resize(len(dnames))
        for i in range(len(dnames)): devicenames[i] = dnames[i]
        vacpp.set_devicenames("si_cv", devicenames);
        utils.log(message1='driver', message2='set si_cv_devicenames', color='yellow', attr=None)
        # quad
        dnames = record_names.get_quad_devicenames()
        devicenames.resize(len(dnames))
        for i in range(len(dnames)): devicenames[i] = dnames[i]
        vacpp.set_devicenames("si_quad", devicenames);
        utils.log(message1='driver', message2='set si_quad_devicenames', color='yellow', attr=None)

    def _update_all_pvs(self):
        pvs = record_names.get_all_recordnames()
        for pv in pvs:
            values = vacpp.CppDoubleVector()
            vacpp.get_pv(pv, values)
            if len(values) == 1:
                self.setParam(pv, values[0])
            else:
                self.setParam(pv, list(values))
        self.updatePVs()

    def update_epics_memory(self):
        # receives updated pvs from c++ library
        pvs = vacpp.CppStringVector()
        vacpp.get_list_of_changed_pvs(pvs)
        # sets EPICS memory with those updated pv values
        for i in range(len(pvs)):
            values = vacpp.CppDoubleVector()
            vacpp.get_pv(pvs[i], values)
            if len(values) == 1:
                self.setParam(pvs[i], values[0])
            else:
                self.setParam(pvs[i], list(values))
        self.updatePVs()

    def update_epics_memory_dynamic_pvs(self):
        # receives updated pvs from c++ library
        vacpp.add_dynamic_pvs_to_list()
        self.update_epics_memory()

    def read(self, reason):
        values = super().read(reason)
        try:
            vstr = '[' + str(values[0]) + ' ... ' + str(values[-1]) + '] (' + str(len(values)) + ')'
        except:
            vstr = str(values)
        utils.log(message1='read', message2=reason+' | '+vstr + ' | ['+self.rnames[reason]['unit'] + ']', color=None, attr=None)
        return values

    def write(self, reason, value):
        utils.log(message1='write', message2=reason+':'+str(value), color='red', attr=None)
        prev_value = self.getParam(reason)
        if value != prev_value:
            result = vacpp.set_pv(reason, value)
            self.setParam(reason, float(value))
            self.update_epics_memory()
