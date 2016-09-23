#!/usr/bin/python3

import sys
import fileinput
import re

def main(argv):
    print('Using Python version %s.%s.%s (%s)' % (sys.version_info[0], sys.version_info[1], sys.version_info[2], sys.version_info[3]))

    if len(argv) < 2:
        sys.exit(1)

    fileName = argv[1]
    print('Loading %s' % (fileName))
    for line in fileinput.input([fileName]):
        line = line.strip()

        # Possible line patterns:
        #   pid syscall(paramList) = result
        #   pid syscall(paramList <unfinished ...>
	#   pid <... syscall resumed> paramList? ) = result
        #   pid --- SIGSEGV {si_signo=SIGSEGV, si_code=SEGV_MAPERR, si_addr=0x14} ---

        # first separate out the pid from the rest of the line - this is always the same pattern!
        pidAndRest = re.match(r'(\d+)\s*(.*)', line)
        pid = pidAndRest.group(1).strip()
        callWithParamAndResult = pidAndRest.group(2).strip()

        # then separate left and right part of rightmost equal sign, to keep the regexp simpler
        # Note that the parameter list might also include an equal sign
        result = None
        if callWithParamAndResult.startswith('---') or callWithParamAndResult.endswith('...>'):
            callWithParam = callWithParamAndResult.strip()
        else:
            parts = callWithParamAndResult.rsplit('=', 1)
            if len(parts) == 2:
                result = parts[1].strip()
            callWithParam = parts[0].strip()

        # Possible callWithParam patterns:
        #   syscall(paramList)
        #   syscall(paramList <unfinished ...>
	#   <... syscall resumed> )

        if callWithParam.endswith('<unfinished ...>'):
            callWithParam = callWithParam[:-16]
            parsedCall = re.match(r'([^(]+)\((.*)', callWithParam)
            #                        syscall(paramList <unfinished ...>
            #                        poll([{fd=18, events=POLLIN|POLLOUT}], 1, 4294967295 <unfinished ...>
            #                        writev(18, [{"I\2\5\0<\0 \4X\0003\0\f\0\n\0\377\377\377\377", 20}, {NULL, 0}, {"", 0}], 3 <unfinished ...>"

        elif callWithParam.startswith('---'):
            parsedCall = re.match(r'--- ([^{]+)([^)]*)', callWithParam)
        elif callWithParam.startswith('+++'):
            parsedCall = re.match(r'\+\+\+ ([^+]+) \+\+\+', callWithParam)
        elif callWithParam.startswith('<...'):
            parsedCall = re.match(r'<... ([^(]+) resumed> ([^)]*)\)', callWithParam)
	    #                       <... syscall resumed> paramList?  )
        else:
            parsedCall = re.match(r'([^(]+)\(([^)]*)\)', callWithParam)
            #                        syscall (paramList)

        if parsedCall is None:
            raise RuntimeError('Unparseable: "' + line + '"')

        syscall = parsedCall.group(1)
        parameters = None
        if len(parsedCall.groups()) > 1:
            parameters = parsedCall.group(2)
        parsedData = (pid, syscall, parameters, result)
        processCall(parsedData)

pidMap = {}
uniquePid = 1

def processCall(data):
    global uniquePid

    pid = data[0]
    mappedPid = pidMap.get(pid)
    if mappedPid is None:
        mappedPid = uniquePid
        uniquePid = uniquePid + 1
        pidMap[pid] = mappedPid

    print(mappedPid, data[1], data[2], data[3])


if __name__ == '__main__':
    main(sys.argv)
