'''
Created on 24.02.2015

@author: afester
'''

# import dropbox

from StylableTextEdit.FormatManager import FormatManager 
from model.NotepadDB import NotepadDB
from model.Page import LocalPage, DropboxPage

class LocalNotepad:

    def __init__(self, npDef):
        self.type = npDef['type']
        self.name = npDef['name']
        self.rootPath = npDef['path']

        self.formatManager = FormatManager()
        self.formatManager.loadFormats()    # TODO: Only required once

        self.db = NotepadDB()
        self.db.openDatabase(self)


    def close(self):
        print('Closing {}'.format(self.name))
        self.db.closeDatabase()


    def getPage(self, pageId):
        result = LocalPage(self, pageId)
        result.load()
        return result


    def getAllPages(self):
        return self.db.getAllPages()

    def getChildCount(self, pageId):
        return self.db.getChildCount(pageId)

    def getChildPages(self, pageId):
        return self.db.getChildPages(pageId)

    def getParentPages(self, pageId):
        return self.db.getParentPages(pageId)

    def getChildPagesWithHandle(self, pageId):
        return self.db.getChildPagesWithHandle(pageId)

    def updateLinks(self, pageId, linksTo):
        self.db.updateLinks(pageId, linksTo)

    def getName(self):
        return self.name


    def getType(self):
        return self.type


    def getFormatManager(self):
        return self.formatManager


    def getRootpath(self):
        return self.rootPath


    def findPathToPage(self, pageId):
        '''Find the first path to a given page id.
'''
        path = self.db.getPathToPage(pageId)
        path.insert(0, self.getName())
        return path



class DropboxNotepad(LocalNotepad):

    def __init__(self, npDef, settings):
        self.type = npDef['type']
        self.name = npDef['name']
        self.rootPath = self.name
        self.client = dropbox.client.DropboxClient(settings.getDropboxToken())

        self.formatManager = FormatManager()
        self.formatManager.loadFormats()    # TODO: Only required once


    def getPage(self, pageId):
        result = DropboxPage(self, pageId)
        result.load()
        return result

