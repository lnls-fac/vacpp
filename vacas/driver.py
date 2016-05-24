
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

    def __init__(self):
        super(PCASDriver, self).__init__()
        #self.update_thread = threading.Thread(group=None,target=PCASDriver.update,args=(self,))
        self._init_vacpp_structures()
        self._update_all_pvs()
        self.is_running = True

    def _init_vacpp_structures(self):
        devicenames = vacpp.CppStringVector()
        # bpms
        dnames = record_names.get_sidi_bpm_devicenames()
        #print(dnames)
        devicenames.resize(len(dnames))
        for i in range(len(dnames)): devicenames[i] = dnames[i]
        vacpp.set_devicenames("si_bpm", devicenames);
        utils.log(message1='driver', message2='set si_bpm_devicenames', color='yellow', attr=None)
        # ch
        dnames = record_names.get_sips_ch_devicenames()
        devicenames.resize(len(dnames))
        for i in range(len(dnames)): devicenames[i] = dnames[i]
        vacpp.set_devicenames("si_ch", devicenames);
        utils.log(message1='driver', message2='set si_ch_devicenames', color='yellow', attr=None)
        # cv
        dnames = record_names.get_sips_cv_devicenames()
        devicenames.resize(len(dnames))
        for i in range(len(dnames)): devicenames[i] = dnames[i]
        vacpp.set_devicenames("si_cv", devicenames);
        utils.log(message1='driver', message2='set si_cv_devicenames', color='yellow', attr=None)

    def _update_all_pvs(self):
        pvs = record_names.get_all_recordnames()
        for pv in pvs:
            value = vacpp.get_pv(pv)
            self.setParam(pv, value)
        self.updatePVs()

    def update_epics_memory(self):
        # receives updated pvs from c++ library
        pvs = vacpp.CppStringVector()
        vacpp.get_list_of_changed_pvs(pvs)
        # sets EPICS memory with those updated pv values
        for i in range(len(pvs)):
            value = vacpp.get_pv(pvs[i])
            self.setParam(pvs[i], value)
            #self.setParam(pvs[i], values[i])
        self.updatePVs()

    def update_epics_memory_dynamic_pvs(self):
        # receives updated pvs from c++ library
        vacpp.add_dynamic_pvs_to_list()
        self.update_epics_memory()

    def read(self, reason):
        #changed = vacpp.update_models();
        value = super().read(reason)
        utils.log(message1='read', message2=reason+':'+str(value), color=None, attr=None)
        return value

    def write(self, reason, value):
        utils.log(message1='write', message2=reason+':'+str(value), color='red', attr=None)
        prev_value = self.getParam(reason)
        if value != prev_value:
            result = vacpp.set_pv(reason, value)
            self.setParam(reason, float(value))
            self.update_epics_memory()
