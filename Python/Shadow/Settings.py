'''
Created on 25.02.2015

@author: afester
'''

from PyQt5.QtCore import QSize, QPoint
import configparser
import logging


class Settings(object):

    l = logging.getLogger('Settings')


    def __init__(self, fileName = 'shadow.ini'):
        self.fileName = fileName
        self.mainWindowSize = QSize(1200, 700)
        self.mainWindowPos = QPoint(240, 200)

    
    def load(self):
        self.l.debug('Loading local settings from {}'.format(self.fileName))
        config = configparser.ConfigParser()
        config.read(self.fileName)

        # load main window position and size
        if config.has_section('mainWindow'):
            windowSettings = config['mainWindow']

            # read position
            pos = windowSettings.get('pos', '{},{}'.format(self.mainWindowPos.x(), self.mainWindowPos.y())).split(',')
            self.mainWindowPos = QPoint(int(pos[0]), int(pos[1]))

            # read size
            size = windowSettings.get('size', '{},{}'.format(self.mainWindowSize.width(), self.mainWindowSize.height())).split(',')
            self.mainWindowSize = QSize(int(size[0]), int(size[1]))


    def dump(self, channel):
        self.l.debug('Saving local settings ...')
        config = configparser.ConfigParser()

        config.add_section('mainWindow')
        config.set('mainWindow', 'pos', '{},{}'.format(self.mainWindowPos.x(), self.mainWindowPos.y()))
        config.set('mainWindow', 'size', '{},{}'.format(self.mainWindowSize.width(), self.mainWindowSize.height()))

        config.write(channel)

    
    def save(self):
        with open(self.fileName, 'w') as configfile:
            self.dump(configfile)

    def setMainWindowPos(self, pos):
        self.mainWindowPos = pos    

    def setMainWindowSize(self, size):
        self.mainWindowSize = size

    def getMainWindowPos(self):
        return self.mainWindowPos

    def getMainWindowSize(self):
        return self.mainWindowSize
