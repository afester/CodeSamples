'''
Created on 24.02.2015

@author: afester
'''

from XMLImporter import XMLImporter
from XMLExporter import XMLExporter
from FormatManager import FormatManager
import os, urllib.parse, uuid, io
import dropbox
from dropbox.rest import ErrorResponse


class LocalNotepad:

    def __init__(self, npDef):
        self.type = npDef['type']
        self.name = npDef['name']
        self.rootPath = npDef['path']

        self.formatManager = FormatManager()
        self.formatManager.loadFormats()    # TODO: Only required once


    def ensureExists(self):
        if not os.path.isdir(self.getRootpath()):
            print(self.getRootpath() + " does not exist, creating directory ...")
            os.mkdir(self.getRootpath())
  
        contentFile = os.path.join(self.getRootpath(), 'content.xml')
        if not os.path.isfile(contentFile):
            print(contentFile + " does not exist, creating file ...")
            with open(contentFile, 'w') as fd:
                fd.write('''<?xml version = '1.0' encoding = 'UTF-8'?>
<page>
  <h1>{}</h1>
</page>
'''.format(self.getName()))


    def getPage(self, pageId):
        result = LocalPage(self, pageId)
        result.load()
        return result


    def getName(self):
        return self.name


    def getType(self):
        return self.type


    def getFormatManager(self):
        return self.formatManager


    def getRootpath(self):
        return self.rootPath


class LocalPage:

    def __init__(self, notepad, pageId):
        assert pageId is None or type(pageId) is str

        self.notepad = notepad
        self.pageId = pageId
        self.links = []
        self.document = None


    def getName(self):
        if self.pageId is None:
            return "Title page"
        else:
            return self.pageId


    def getPagePath(self):
        pagePath = self.notepad.getRootpath()
        if self.pageId is not None:     # not the root page
            pageFilename = urllib.parse.quote(self.pageId)
            pageIdx = [self.pageId[0], pageFilename]
            pagePath = os.path.join(pagePath, *pageIdx)
        return pagePath


    def load(self):
        pagePath = self.getPagePath()
        print("  Loading page at {} ".format(pagePath))

        if not os.path.isdir(pagePath):
            print(pagePath + " does not exist, creating directory ...")
            os.makedirs(pagePath)

        contentFile = os.path.join(pagePath, 'content.xml')
        if not os.path.isfile(contentFile):
            print(contentFile + " does not exist, creating file ...")
            with open(contentFile, 'w') as fd:
                fd.write('''<?xml version = '1.0' encoding = 'UTF-8'?>
<page>
  <h1>{}</h1>
</page>
'''.format(self.pageId))

        importer = XMLImporter(pagePath, "content.xml", self.notepad.getFormatManager())
        importer.importDocument()
        self.document = importer.getDocument()
        self.links = importer.getLinks()


    def save(self):
        pagePath = self.getPagePath()
        print("  Saving page to {} ".format(pagePath))

        exporter = XMLExporter(pagePath, 'content.xml')
        exporter.exportDocument(self.document)

        self.document.setModified(False)


#===============================================================================
#         with open(self.contentFile, 'w') as content_file:
#             # NOTE: toHtml() writes the text document in a "hard coded"
#             # HTML format, converting all style properties to inline style="..." attributes
#             # See QTextHtmlExporter in qtbase/src/gui/text/qtextdocument.cpp.
#             # We are using a custom XML exporter therefore ...
# 
#             #content_file.write(self.editView.toHtml())
# 
#             exporter = XMLExporter(content_file)
#             exporter.exportDocument(self.editView.document())
#===============================================================================

    def saveImage(self, image):
        fileName = str(uuid.uuid4()).replace('-', '') + '.png'
        filePath = os.path.join(self.getPagePath(), fileName)
        print("Saving image to {}".format(filePath))
        image.save(filePath)

        return fileName


    def getLinks(self):
        return self.links


    def getDocument(self):
        return self.document



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


    def ensureExists(self):
        print("DROPBOX: EnsureExists({})".format(self.getRootpath()))
        try:
            self.client.file_create_folder(self.getRootpath())
        except ErrorResponse as rsp:
            print(str(rsp))


class DropboxPage(LocalPage):

    def __init__(self, notepad, pageId):
        LocalPage.__init__(self, notepad, pageId)


    def load(self):
        pagePath = self.getPagePath()
        contentFile = '{}/{}'.format(pagePath, 'content.xml')
        print("  DROPBOX: Loading page from {} ".format(contentFile))

        try:
            with self.notepad.client.get_file(contentFile) as f:
                importer = XMLImporter(pagePath, "content.xml", self.notepad.getFormatManager())
                importer.importFromFile(f)
                self.document = importer.getDocument()
                self.links = importer.getLinks()
        except ErrorResponse as rsp:
            if rsp.status == 404:
                self.createPage(contentFile)
            else:
                print(str(rsp))


    def createPage(self, pagePath):
        print("  DROPBOX: CREATING PAGE {}".format(pagePath))
        contents = io.StringIO('''<?xml version = '1.0' encoding = 'UTF-8'?>
<page>
  <h1>{}</h1>
</page>
'''.format(self.pageId))
        try:
            self.notepad.client.put_file(pagePath, contents)
        except ErrorResponse as rsp:
            print(str(rsp))

        contents.close()


    def getPagePath(self):
        pagePath = self.notepad.getRootpath()
        if self.pageId is not None:     # not the root page
            pageFilename = urllib.parse.quote(self.pageId)
            pageIdx = [pagePath, self.pageId[0], pageFilename]
            pagePath = '/'.join(pageIdx)
            
        return pagePath


    def save(self):
        pagePath = self.getPagePath()
        contentFile = '{}/{}'.format(pagePath, 'content.xml')
        print("  DROPBOX: saving page to {} ".format(contentFile))

        try:
            exporter = XMLExporter(pagePath, 'content.xml')
            contents = io.StringIO(exporter.getXmlString(self.document))
            self.notepad.client.put_file(contentFile, contents, True)
            contents.close()
        except ErrorResponse as rsp:
            print(str(rsp))
