'''
Created on 25.02.2015

@author: afester
'''

from PyQt5.QtCore import QSize, QPoint
import configparser
import logging
import ast


class Settings(object):

    l = logging.getLogger('Settings')


    def __init__(self):
        self.notepads = []
        self.mainWindowSize = QSize(1200, 700)
        self.mainWindowPos = QPoint(240, 200)
        self.browserPath = []

    
    def load(self, fileName = 'notepad.ini'):
        self.l.debug('Loading local settings ...')
        config = configparser.ConfigParser()
        config.read(fileName)

        # load dropbox configuration
        self.dropboxToken = ''
        if config.has_section('dropbox'):
            self.dropboxToken = config['dropbox'].get('dropboxToken')

        # load browser state
        if config.has_section('browser'):
            browserSettings = config['browser']
            path = browserSettings.get('path')
            self.browserPath = ast.literal_eval(path)

        # load main window position and size
        if config.has_section('mainWindow'):
            windowSettings = config['mainWindow']

            # read position
            pos = windowSettings.get('pos', '{},{}'.format(self.mainWindowPos.x(), self.mainWindowPos.y())).split(',')
            self.mainWindowPos = QPoint(int(pos[0]), int(pos[1]))

            # read size
            size = windowSettings.get('size', '{},{}'.format(self.mainWindowSize.width(), self.mainWindowSize.height())).split(',')
            self.mainWindowSize = QSize(int(size[0]), int(size[1]))

        # load notepads
        self.notepads = []
        for s in config.sections():
            if s.startswith('notepad_'):
                npDef = None
                npType = config.get(s, 'type')
                if npType == 'local':
                    npDef = {'name' : config.get(s, 'name'),
                             'type'  : npType,
                             'path'   : config.get(s, 'path') }
                elif npType == 'dropbox':
                    npDef = {'name' : config.get(s, 'name'),
                             'type'  : npType }
                if npDef is not None:
                    self.notepads.append(npDef)


    def dump(self, channel):
        self.l.debug('Saving local settings ...')
        config = configparser.ConfigParser()

        config.add_section('dropbox')
        config.set('dropbox', 'dropboxToken', self.dropboxToken)

        config.add_section('browser')
        config.set('browser', 'path', str(self.browserPath))

        config.add_section('mainWindow')
        config.set('mainWindow', 'pos', '{},{}'.format(self.mainWindowPos.x(), self.mainWindowPos.y()))
        config.set('mainWindow', 'size', '{},{}'.format(self.mainWindowSize.width(), self.mainWindowSize.height()))

        idx = 0
        for s in self.notepads:
            sectName = "notepad_{}".format(idx)
            idx += 1
            
            config.add_section(sectName)
            config.set(sectName, 'name', s['name'])
            config.set(sectName, 'type', s['type'])
            if s['type'] == 'local':
                config.set(sectName, 'path', s['path'])

        config.write(channel)

    
    def save(self, fileName = 'notepad.ini'):
        with open(fileName, 'w') as configfile:
            self.dump(configfile)


    def getNotepads(self):
        return self.notepads

    def addNotepad(self, npDef):
        self.notepads.append(npDef)

    def setMainWindowPos(self, pos):
        self.mainWindowPos = pos    

    def setBrowserPath(self, path):
        self.browserPath = path

    def setMainWindowSize(self, size):
        self.mainWindowSize = size

    def getMainWindowPos(self):
        return self.mainWindowPos

    def getMainWindowSize(self):
        return self.mainWindowSize

    def getBrowserPath(self):
        return self.browserPath

    def getDropboxToken(self):
        return self.dropboxToken 

    def setDropboxToken(self, token):
        self.dropboxToken = token 
