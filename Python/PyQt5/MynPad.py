#!/usr/bin/env python3

'''
Created on Feb 13, 2015

@author: andreas
'''


import sys

import importlib_resources

sys.path.insert(0, '/opt/pyqt53/site-packages')

from PyQt6.QtWidgets import QApplication

import logging.config
# from pip.commands.list import ListCommand


from ui.MainWindow import MainWindow
import data
import io

def main():
    print(__name__)

#    lx = ListCommand("MynPad", "")
#    lx.main(args=None)

    loggingIni = importlib_resources.files(data.__name__).joinpath('logging.ini')
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
