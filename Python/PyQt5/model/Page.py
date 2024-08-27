"""
Created on 24.02.2015

@author: afester
"""

import logging
import os
import urllib.parse
import uuid

# from dropbox.rest import ErrorResponse
from StylableTextEdit.StylableTextModel import Frame, Paragraph, TextFragment, DocumentFactory
from model.XMLExporter import XMLExporter
from model.XMLImporter import XMLImporter
from git import Repo

class LocalPage:

    LOG = logging.getLogger('LocalPage')

    def __init__(self, notepad, pageId):
        """@param notepad  The Notepad instance for this page
           @param pageId   The page name / page id for this page
        """

        assert type(pageId) is str

        self.notepad = notepad
        self.pageId = pageId
        self.links = []
        self.document = None

    def getName(self) -> str:
        """
        :return: The page id (page name) of this page
        """

        return self.pageId

    def getFilename(self):
        """
        :return: The file name for this page, like "Todo%20List.xml"
        """

        return urllib.parse.quote(self.getName(), '') + '.xml'

    def getPageDir(self):
        """@return The directory for this page, like "c:\\temp\\testpad\\T"
        """
        pagePath = self.notepad.getRootpath()
        pageIdx = self.pageId[0].upper()
        pagePath = pagePath + '/' + pageIdx
        return pagePath

    def get_page_path_relative(self) -> str:
        """
        :return: The relative path name to the page.xml file, like `T\\Todo%20List.xml`
        """

        pageIdx = self.pageId[0].upper()
        return os.path.join(pageIdx, self.getFilename())

    def getPagePath(self) -> str:
        """
        :return: The absolute path name to the page.xml file, like `c:\\temp\\testpad\\T\\Todo%20List.xml`
        """

        return os.path.join(self.getPageDir(), self.getFilename())

    def load(self):
        pageFullPath = self.getPagePath()
        self.LOG.debug('Loading page at {} '.format(pageFullPath))

        pageDir = self.getPageDir()
        if not os.path.isfile(pageFullPath):
            self.LOG.debug('Page does not exist, creating empty document ...')

            rootFrame = Frame()
            p1 = Paragraph(0, ('title', 'level', '1'))
            title = TextFragment(None)
            title.setText(self.pageId)
            p1.add(title)
            p2 = Paragraph(0, ('para', None, None))
            rootFrame.add(p1)
            rootFrame.add(p2)

            docFac = DocumentFactory(pageDir, self.notepad.formatManager)
            self.document = docFac.createDocument(rootFrame)

            self.links = []
        else:
            importer = XMLImporter(pageDir, self.getFilename(), self.notepad.getFormatManager())
            importer.importDocument()
            self.document = importer.getDocument()
            self.links = importer.getLinks()

    def save(self):
        pagePath = self.getPagePath()
        self.LOG.debug('Saving page to {}'.format(pagePath))

        pageDir = self.getPageDir()
        if not os.path.isdir(pageDir):
            self.LOG.debug('{} does not exist, creating directory ...'.format(pageDir))
            os.makedirs(pageDir)

        exporter = XMLExporter(self.getPageDir(), self.getFilename())
        exporter.exportDocument(self.document)

        self.notepad.updateLinks(self.getName(), exporter.getLinks())
        self.document.setModified(False)

    def saveImage(self, image):
        fileName = str(uuid.uuid4()).replace('-', '') + '.png'
        filePath = os.path.join(self.getPageDir(), fileName)
        print("Saving image to {}".format(filePath))
        image.save(filePath)

        return fileName

    def getLinks(self):
        return self.links

    def getDocument(self):
        return self.document

    def get_history(self) -> list[dict]:
        repo_dir = self.notepad.getRootpath()
        repo = Repo(repo_dir)

        file_path = self.get_page_path_relative()
        history = repo.iter_commits(all=True, paths=file_path)

        page_history = [{"date": x.committed_datetime,
                         "message": str(x.message).strip()}
                        for x in history]

        return page_history


class DropboxPage(LocalPage):

    def __init__(self, notepad, pageId):
        LocalPage.__init__(self, notepad, pageId)

    def getPageDir(self):
        """@return The directory for this page, like "testpad/T"
        """
        pagePath = self.notepad.getRootpath()

        if self.pageId is not None:     # not the root page
            pageIdx = self.pageId[0]
            pagePath = pagePath + '/' + pageIdx
        return pagePath

    def getPagePath(self):
        """@return The absolute path name to the page.xml file,
                   like "testpad/T/Todo%20List.xml"
        """
        return self.getPageDir() + '/' + self.getFilename()

    def load(self):
        pagePath = self.getPagePath()
        print("  DROPBOX: Loading page from {} ".format(pagePath))

        try:
            # NOTE: get_file retrieves a BINARY file, with no character decoding applied!
            # The sax parser seems to use the proper encoding, based on the xml header
            with self.notepad.client.get_file(pagePath) as f:
                importer = XMLImporter(self.getPageDir(), self.getFilename(), self.notepad.getFormatManager())
                importer.importFromFile(f)
                self.document = importer.getDocument()
                self.links = importer.getLinks()
        except ErrorResponse as rsp:
            if rsp.status == 404:
                self.createPage(pagePath)

                # TODO: Should not require an additional roundtrip after page creation
                with self.notepad.client.get_file(pagePath) as f:
                    importer = XMLImporter(self.getPageDir(), self.getFilename(),
                                           self.notepad.getFormatManager())
                    importer.importFromFile(f)
                    self.document = importer.getDocument()
                    self.links = importer.getLinks()
            else:
                print(str(rsp))

    def createPage(self, pagePath):
        print("  DROPBOX: CREATING PAGE {}".format(pagePath))
        xmlString = '''<?xml version="1.0" encoding="utf-8"?>
<article version="5.0" xml:lang="en"
         xmlns="http://docbook.org/ns/docbook"
         xmlns:xlink="http://www.w3.org/1999/xlink">
  <title></title>

  <section>
    <title>{}</title>
  </section>
</article>
'''.format(self.pageId)
        try:
            fileData = bytes(xmlString, encoding='utf-8')
            self.notepad.client.put_file(pagePath, fileData)
        except ErrorResponse as rsp:
            print(str(rsp))

    def save(self):
        pagePath = self.getPagePath()
        print("  DROPBOX: saving page to {} ".format(pagePath))

        try:
            exporter = XMLExporter(self.getPageDir(), self.getFilename())
            xmlString = exporter.getXmlString(self.document)

            # NOTE: put_file stores a BINARY file, with no character encoding applied!
            fileData = bytes(xmlString, encoding='utf-8')
            self.notepad.client.put_file(pagePath, fileData, True)
        except ErrorResponse as rsp:
            print(str(rsp))
