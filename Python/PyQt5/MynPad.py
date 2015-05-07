#!/usr/bin/env python3

'''
Created on Feb 13, 2015

@author: andreas
'''

from PyQt5.QtWidgets import QApplication

import sys
import logging.config

from ui.Richtext import MainWindow


def main():
    logging.config.fileConfig('logging.ini')

    # Create the application object
    app = QApplication(sys.argv)

    # Create the main window
    mainWindow = MainWindow(app)


#    from NotepadDB import NotepadDB
#    db = NotepadDB('MynPad')
#    db.openDatabase();
#    db.closeDatabase();


    # Show and run the application
    mainWindow.show()
    app.exec()


if __name__ == '__main__':
    main()
