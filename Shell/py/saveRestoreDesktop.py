#!/usr/bin/env python3

# Script taken from https://askubuntu.com/questions/720193/how-can-i-preserve-the-desktops-icon-positions-after-reboot-on-a-dual-monitor

import subprocess
import os
import sys
import time

# --- set your localized desktop name below:
dt_name = "Desktop"
# ---


# used strings & definitions
val = " 'metadata::nautilus-icon-position' "
search = "gio info -a" + val
set_val = "gio set -t string "
home = os.environ["HOME"]
dt_dir = home + "/" + dt_name

get = lambda cmd: subprocess.check_output(["/bin/bash", "-c", cmd]).decode("utf-8")

def snapshot(fileName):
    datafile = os.path.join(home, fileName)

    # remove the previous snapshot
    try:
        os.remove(datafile)
    except FileNotFoundError:
        pass

    # list the items on the desktop, look up their positions
    dt_items = os.listdir(dt_dir)
    dt_data = []
    for f in [f for f in dt_items if all([not f.startswith("."), not f.endswith("~")])]:
        f = '"'+f+'"' if f.count(" ") > 0 else f
        file = dt_dir + "/" + f
        cmd = search + file
        try:
            loc = [int(n) for n in get(cmd).split()[-1].split(",")]
            loc = (",").join([str(n) for n in loc])
            open(datafile, "a+").write(file+"|||"+loc+"\n")
            # print('{}: {}'.format(file, loc))
        except:
            pass

    # notify that a snapshot was taken
    subprocess.call(["/bin/bash", "-c", "notify-send 'Desktop snapshot stored in {}'".format(datafile)])

    # send the command to shutdown
    # subprocess.Popen(["gnome-session-quit", "--power-off"])

def restore(fileName):
    datafile = os.path.join(home, fileName)

    # wait for 20 seconds for the desktop to be fully loaded and the screen to be found
#    time.sleep(20)

    # read the snapshot file
    items = [l.strip().split("|||") for l in open(datafile).readlines()]
    for item in items:
        try:
            # place the icon(s) acoording to the remembered position(s)
            xy = [int(n) for n in item[1].split(",")]
            move_to = (",").join(str(n) for n in [xy[0], xy[1]])
            command = set_val+item[0]+val+move_to
            subprocess.Popen(["/bin/bash", "-c", command])
        except:
            pass

    # restart gnome-shell
    subprocess.call(["killall", "-HUP", 'gnome-shell'])

    # send notification
    subprocess.Popen(["/bin/bash", "-c", "notify-send 'Icon positions were restored'"])

def usage(arg0):
    print('Usage: {} snapshot|restore dataFile'.format(arg0))

def main(argv):
    if len(argv) != 3:
        usage(argv[0])
    else:
        command = argv[1]
        fileName = argv[2]
        if command == "snapshot":
            snapshot(fileName)
        elif command == "restore":
            restore(fileName)
        else:
            usage(argv[0])

main(sys.argv)
