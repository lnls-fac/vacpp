import os as _os
import lnls as _lnls

_sidi_bpm_devicenames_fname = _os.path.join(_lnls.folder_root, 'siriusdb', 'recordnames_flatlists', 'sidi-bpm.txt')
_sips_ch_devicenames_fname  = _os.path.join(_lnls.folder_root, 'siriusdb', 'recordnames_flatlists', 'sips-ch.txt')
_sips_cv_devicenames_fname  = _os.path.join(_lnls.folder_root, 'siriusdb', 'recordnames_flatlists', 'sips-cv.txt')

def _read_devicename_file(filename):
    with open(filename, 'r') as fp:
        content = fp.read()
    content = content.splitlines()
    devicenames = []
    for line in content:
        line = line.strip()
        if not line or line[0] == '#': continue
        words = line.split()
        devicenames.append(words[0])
    return devicenames

def get_sidi_bpm_devicenames():
    return _read_devicename_file(_sidi_bpm_devicenames_fname)

def get_sips_ch_devicenames():
    return _read_devicename_file(_sips_ch_devicenames_fname)

def get_sips_cv_devicenames():
    return _read_devicename_file(_sips_cv_devicenames_fname)

def get_sidi_bpm_recordnames():
    dnames = get_sidi_bpm_devicenames()
    rnames = {}
    for dname in dnames:
        rnames[dname + ':MONIT:X'] = {'type': 'float', 'count': 1, 'value': 0.000, 'prec': 9, 'unit': 'm'}
        rnames[dname + ':MONIT:Y'] = {'type': 'float', 'count': 1, 'value': 0.000, 'prec': 9, 'unit': 'm'}
    rnames['SIDI-BPM-FAM:MONIT:X'] = {'type': 'float', 'count': len(dnames), 'value': 0, 'prec': 9, 'unit': 'm'}
    rnames['SIDI-BPM-FAM:MONIT:Y'] = {'type': 'float', 'count': len(dnames), 'value': 0, 'prec': 9, 'unit': 'm'}
    return rnames

def get_simo_recordnames():
    dnames = get_sidi_bpm_devicenames()
    rnames = {}
    rnames['SIMO-BPM-FAM:SPOS'] = {'type': 'float', 'count': len(dnames), 'value': 0, 'prec': 6, 'unit': 'm'}
    return rnames

def get_sips_ch_recordnames():
    dnames = get_sips_ch_devicenames()
    rnames = {}
    for dname in dnames:
        rnames[dname + ':HARDWARE'] = {'value': 0.000, 'prec': 6, 'unit': 'A'}
        rnames[dname + ':PHYSICS']  = {'value': 0.000, 'prec': 6, 'unit': 'mrad'}
    return rnames

def get_sips_cv_recordnames():
    dnames = get_sips_cv_devicenames()
    rnames = {}
    for dname in dnames:
        rnames[dname + ':HARDWARE'] = {'value': 0.000, 'prec': 6, 'unit': 'A'}
        rnames[dname + ':PHYSICS']  = {'value': 0.000, 'prec': 6, 'unit': 'mrad'}
    return rnames

def get_various_recordnames():
    rnames = {

        #'SITEST' : {'value':0.000, 'prec':3, 'unit': ''},
        #'BOTEST' : {'value':0.000, 'prec':3, 'unit': ''},

        'SIDI-CURRENT'  : {'value': 0.000, 'prec': 3, 'unit': 'mA'},
        'SIPA-LIFETIME' : {'value': 0.000, 'prec': 4, 'unit': 'h' },

        # 'SIPS-BEND-FAM' : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-SFA-FAM'  : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-QFA-FAM'  : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-SDA-FAM'  : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-QDA-FAM'  : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-SD1J-FAM' : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-QF1-FAM'  : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-SF1J-FAM' : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-QF2-FAM'  : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-SD2J-FAM' : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-SD3J-FAM' : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-QF3-FAM'  : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-SF2J-FAM' : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-QF4-FAM'  : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-SF2K-FAM' : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-SD3K-FAM' : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-SD2K-FAM' : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-SF1K-FAM' : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-SD1K-FAM' : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-QDB1-FAM' : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-SDB-FAM'  : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-QFB-FAM'  : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-SFB-FAM'  : {'value': 0.000, 'prec': 3, 'unit': 'A' },
        # 'SIPS-QDB2-FAM' : {'value': 0.000, 'prec': 3, 'unit': 'A' },

        'BOPA-LIFETIME' : {'value': 0.000, 'prec': 4, 'unit': 'h' },
        'BODI-CURRENT'  : {'value': 0.000, 'prec': 3, 'unit': 'mA'},
    }
    return rnames

def get_all_recordnames() :
    rnames = {}
    rnames.update(get_sidi_bpm_recordnames()) # SIDI-BPM
    rnames.update(get_sips_ch_recordnames())  # SIPS-CH
    rnames.update(get_sips_cv_recordnames())  # SIPS-CV
    rnames.update(get_simo_recordnames())     # SIMO
    rnames.update(get_various_recordnames())  # others
    return rnames

def get_pvs_lists():
    rnames = list(get_all_recordnames().keys())
    li,tb,bo,ts,si,ti = [],[],[],[],[],[]
    for rname in rnames:
        if rname.startswith('LI'):li.append(rname)
        if rname.startswith('TB'):tb.append(rname)
        if rname.startswith('BO'):bo.append(rname)
        if rname.startswith('TS'):ts.append(rname)
        if rname.startswith('SI'):si.append(rname)
        if rname.startswith('TI'):ti.append(rname)
    return li,tb,bo,ts,si,ti