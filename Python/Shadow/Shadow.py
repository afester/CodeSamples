#!/usr/bin/env python3

'''
Created on September 1, 2015

@author: andreas
'''


import sys
sys.path.insert(0, '/opt/pyqt53/site-packages')

from PyQt5.QtWidgets import QApplication

import logging.config, pkg_resources, io, data

from ui.MainWindow import MainWindow

def main():
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
