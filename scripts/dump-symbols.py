import csv, sys, json

def read_stdin():
    readline = sys.stdin.readline()
    while readline:
        yield readline
        readline = sys.stdin.readline()
symbols = {}
for line in read_stdin():
    if not "lttng_ust_cyg_profil" in line:
        continue
    split =  line.split('addr =')[-1].replace('0x','').split(',')
    addr = split[0].strip().lower()
    name = split[1].replace("name =",'').strip().strip('"').strip('\n').replace(" ", '')

    if addr not in symbols:
        print("%s T %s" % (addr, name))

    symbols[addr] = name



