import time
import datetime
from termcolor import colored

def log(message1='', message2='', color='white', attr=None):
    t0 = time.time()
    st = datetime.datetime.fromtimestamp(t0).strftime('%Y-%m-%d %H:%M:%S')
    st = st + '.{0:03d}'.format(int(1000*(t0-int(t0))))
    if attr is None: attr = []
    strt = colored(st, 'white', attrs=[])
    str1 = colored('{0:<6.6s}'.format(message1), color, attrs=attr)
    str2 = colored('{0}'.format(message2), color, attrs=attr)
    strt = strt + ': ' + str1 + ' ' + str2
    print(strt)
