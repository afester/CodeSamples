'''
Created on 15.09.2014

@author: afester
'''

import subprocess

class Shell:

    def __init__(self, output):
        self.output = output
        self.hasMore = False

    def readLine(self, stream):
        line = ""
        self.hasMore = False
        try:
            # We need to make sure that the child process does not fill up the pipe
            # and that no deadlock occurs.
            # stream.readLine() worked, but caused deadlocks or IOErrors several times.
            # Popen.communicate() is not an option, since it returns all output of the child
            # process at once, after the child process has terminated. But we need to show 
            # the log output in the log window while the process is running.
            # The best solution so far which works well is to read the stream character 
            # by character and manually build the resulting line.
            while True:
                buffer = stream.read(1)
                if (len(buffer) > 0):
                    self.hasMore = True

                    nextChar = chr(buffer[0])
                    if nextChar == "\n" or nextChar == "":
                       break
                    if nextChar != "\r":
                        line = line + nextChar
                else:
                    break;

        except IOError:
            line = "\n!!! WARNING: IOError occurred!!!\n"
        return line


    # Asynchronously executes a command and prints the output of the command
    # to the current output sink.
    # Waits until the sub process is terminated and returns the result code
    # from the sub process.
    def executeCommand(self, command, params = []):
        args = [command]
        args.extend(params)

        # Uncomment these two lines to enable "dummy mode" where no actions are actually executed
        #if str(args).find("showlabels") == -1 and str(args).find("oklist") == -1:
        #    return 0

        # Note Read the Security Considerations section before using shell=True.!!
        p = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)
        while True:
            line = self.readLine(p.stdout)
            if self.hasMore:
                self.output.nextLine(line)
            else:
                break
        retcode = p.wait()
        return retcode


class Lister:
    def nextLine(self, value):
        print(value)

if __name__ == '__main__':
    shell = Shell(Lister())
    shell.executeCommand("DIR");

