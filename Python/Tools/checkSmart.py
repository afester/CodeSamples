#!/usr/bin/env python3

import subprocess
import sys
import re

def formatHtml(data, out):
   print('''<!DOCTYPE html>
<html>
  <head>
    <title>S.M.A.R.T Status</title>
    <style>
* {
   font-family: "sans";
   font-size: 10pt;
}

h1 {
   font-size: 14pt;
   font-weight: normal;
   color: blue;
   border-bottom-style: solid;
   border-bottom-width: 1px;
}

.driveinfo {
   border-spacing: 0px;
}

.driveinfo th {
   text-align: right;
   padding: 2px 5px 2px 5px;  /* TRBL */
}

.driveinfo .row0 {
   background-color: #f0f0f0;
}

td.int {
   text-align: center;
}

.smartattr {
   border-spacing: 0px;
}

.smartattr th {
   padding: 5px 5px 5px 5px;  /* TRBL */
}

.smartattr td {
   padding: 5px 5px 5px 5px;  /* TRBL */
}

.smartattr .row1 {
   background-color: #f0f0f0;
}
    </style>
  </head>

  <body>
''', file=out)

   print('<h1>Drive Information</h1>', file=out)
   row = 0
   driveInfo = data['DRVINFO']
   print('    <table class="driveinfo">', file=out)
   for info in driveInfo:
       tr = '      <tr class="row'+ str(row%2) + '"><th>' + info[0] + ':</th><td>' + info[1] + '</td></tr>'
       row = row + 1
       print(tr, file=out)
   print('    </table>\n', file=out)

   print('<h1>S.M.A.R.T attributes</h1>', file=out)
   row = 0
   smartAttr = data['SMARTATTR']
   print('    <table class="smartattr">', file=out)
   tr = '      <tr><th>Id</th><th>Attribute</th><th>Flags</th><th>Value</th><th>Worst</th><th>Thresh</th><th>Fail</th><th>Raw</th></tr>'
   print(tr, file=out)
   for attr in smartAttr:
       tr = '      <tr class="row'+str(row%2)+'"><td>' + attr[0] + '</td><td>' + attr[1] + '</td><td><tt>' + attr[2] + '</tt></td><td class="int">' + attr[3] + '</td><td class="int">' \
                             + attr[4] + '</td><td class="int">' + attr[5] + '</td><td class="int">' + attr[6] + '</td><td class="int">' + attr[7] + '</td></tr>' 
       row = row + 1
       print(tr, file=out)
   print('    </table>', file=out)

   print('''
  </body>
</html>
''', file=out)

print("Using python {0}.{1}.{2} ({3})".format(sys.version_info[0], 
                                              sys.version_info[1], 
                                              sys.version_info[2], 
                                              sys.version_info[3]))

parseResult = {}

section = 0
state = 0
result = subprocess.check_output(["sudo", "/usr/sbin/smartctl", "-x", "/dev/sda"], universal_newlines=True)
for line in result.splitlines():
   line = line.strip()
   if len(line) > 0:
     if line == "=== START OF INFORMATION SECTION ===":
        section = 1
        state = 0
     elif line == "=== START OF READ SMART DATA SECTION ===":
        section = 2
        state = 0
     elif line == "Vendor Specific SMART Attributes with Thresholds:":
        section = 3
        state = 0
     elif line.startswith("General Purpose Log Directory"):
        section = 4
        state = 0
     elif line.startswith("SMART Extended Comprehensive Error Log"):
        section = 5
        state = 0
     elif line.startswith("SCT Status Version:"):
        section = 6
        state = 0

     if section == 1:   # Collect drive information
        if state == 0:
           state = 1
           data = []
           parseResult['DRVINFO'] = data
        else:
           nameVal = line.split(':', 1)
           name = nameVal[0].strip()
           val = nameVal[1].strip()
           data.append( (name, val) )
           # print("(I) " + name + "/" + val)
     elif section == 2:   # Collect SMART data
        # print("(S) " + line)
        pass
     elif section == 3:   # Collect SMART attributes
        if state < 2:
           state = state + 1 # skip first two lines
           data = []
           parseResult['SMARTATTR'] = data
        else:
           if not line.startswith('|'):
              m = re.match('(\d+) (\S+)\s+(\S+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\S+)\s+(\d+)', line)
                           # 9 Power_On_Hours          -O--CK   057   057   000    -    31960
              # print("(A) " + str(m.groups()))
              data.append(m.groups())
     elif section == 4:   # 
        # print("(G) " + line)
        pass
     elif section == 5:   # 
        # print("(E) " + line)
        pass
     elif section == 6:   # Collect SCA data
        # print("(T) " + line)
        pass

with open('res.html', 'w') as dest:
   formatHtml(parseResult, dest)
subprocess.call(["/usr/bin/mail -a \"Content-Type: text/html;\" -s \"S.M.A.R.T status\" andreas < res.html"], shell=True)
