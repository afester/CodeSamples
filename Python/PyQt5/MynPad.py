#!/usr/bin/env python3

'''
Created on Feb 13, 2015

@author: andreas
'''


import sys
sys.path.insert(0, '/opt/pyqt53/site-packages')

from PyQt5.QtWidgets import QApplication

import logging.config

from ui.MainWindow import MainWindow


def main():
    logging.config.fileConfig('logging.ini')

    # Create the application object
    app = QApplication(sys.argv)

    # Create the main window
    mainWindow = MainWindow(app)

    # Show and run the application
    mainWindow.show()
    app.exec()


if __name__ == '__main__':
    main()
