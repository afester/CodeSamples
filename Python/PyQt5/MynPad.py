#!/usr/bin/env python3

'''
Created on Feb 13, 2015

@author: andreas
'''


import sys
sys.path.insert(0, '/opt/pyqt53/site-packages')

from PyQt5.QtWidgets import QApplication

import logging.config
from pip._internal.commands.list import ListCommand


from ui.MainWindow import MainWindow
import data
import pkg_resources
import io

def main():
    print(__name__)
    lx = ListCommand("MynPad", "")
    lx.main(args=None)

    # Note: Need a TextIOWrapper since streams are essentially byte-based since Python 3
    # See also http://bugs.python.org/issue13518
    loggingIni = pkg_resources.resource_stream(data.__name__, 'logging.ini')
    loggingIni = io.TextIOWrapper(loggingIni, encoding='utf-8')
    print("LOGGING ini file: {}".format(loggingIni))

    logging.config.fileConfig(loggingIni)

    # Create the application object
    app = QApplication(sys.argv)

    # Create the main window
    mainWindow = MainWindow(app)

    # Show and run the application
    mainWindow.show()
    app.exec()


if __name__ == '__main__':
    main()
