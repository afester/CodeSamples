
import os
import xml.sax

from StylableTextEdit.StylableTextModel import Frame, List, Paragraph, TextFragment, ImageFragment, MathFragment, DocumentFactory
from ui.EditorWidget import MathFormulaObject   # TODO: model should not reference UI

class Handler(xml.sax.handler.ContentHandler):

    def __init__(self, contentPath):
        self.contentPath = contentPath

        self.sectionLevel = 0
        self.listLevel = 0
        self.nodeStack = []
        self.keywordLinks = set()
        self.currentStyle = None
        self.paraStyle = None
        self.content = None
        self.href = None
        self.result = None

    def startElement(self, name, attrs):
        # structural tags
        if name == 'article':
            self.nodeStack = [Frame()]
        elif name == 'section':
            self.sectionLevel += 1
        elif name == 'itemizedlist':    # create a List node and set it as current parent
            self.listLevel += 1
            curList = List(('itemizedlist', 'level', str(self.listLevel)))
            parent = self.nodeStack[-1]
            parent.add(curList)
            self.nodeStack.append(curList)              # push
        elif name == 'mediaobject':
            pass
        elif name == 'imageobject':
            pass
        elif name == 'inlineequation':
            pass

        elif name == 'listitem':
            pass

        # These contain <para> elements, but already define the paragraph style:
        elif name == 'blockquote':
            self.paraStyle = ('blockquote', None, None)
        elif name == 'tip':
            self.paraStyle = ('tip', None, None)
        elif name == 'warning':
            self.paraStyle = ('warning', None, None)

        elif name == 'title':           # create a title paragraph and set it as current parent
            if self.sectionLevel > 0:   # no title for <article>
                para = Paragraph(0, ('title', 'level', str(self.sectionLevel)))
                parent = self.nodeStack[-1]                    # top()
                self.nodeStack.append(para)   # push()
                parent.add(para)
                self.content = ''
        elif name == 'para':            # a paragraph contains only fragments
            if self.paraStyle is None:
                self.paraStyle = ('para', None, None)
            para = Paragraph(0, self.paraStyle)

            parent = self.nodeStack[-1]                    # top()
            self.nodeStack.append(para)   # push()
            parent.add(para)
            self.content = ''           # start collecting content
            self.currentStyle = None    # no specific style currently
        elif name == 'programlisting':  # a program listing contains verbatim text only
            language = attrs.getValue('language')
            para = Paragraph(0, ('programlisting', 'language', language))
            parent = self.nodeStack[-1]                    # top()
            self.nodeStack.append(para)   # push()
            parent.add(para)
            self.content = ''           # start collecting content
            self.currentStyle = None    # no specific style currently

        elif name == 'screen':          # a screen contains verbatim text only
            para = Paragraph(0, ('screen', None, None))
            parent = self.nodeStack[-1]                    # top()
            self.nodeStack.append(para)   # push()
            parent.add(para)
            self.content = ''           # start collecting content
            self.currentStyle = None    # no specific style currently

        # These are the fragments which are added to the current paragraph
        elif name == 'emphasis':
            # Add content so far to the current paragraph
            if len(self.content) > 0:
                parent = self.nodeStack[-1]
    
                frag = TextFragment(self.currentStyle)
                frag.setText(self.content)
                self.content = ''
                parent.add(frag)

            emphasizeRole = attrs.get('role', '')
            if emphasizeRole == '':
                self.currentStyle = ('emphasis', None, None)    # todo: nested styles support (needs yet another stack ...)
            else:
                self.currentStyle = ('emphasis', 'role', emphasizeRole)    # todo: nested styles support (needs yet another stack ...)
        elif name == 'code':
            # Add content so far to the current paragraph
            if len(self.content) > 0:
                parent = self.nodeStack[-1]
    
                frag = TextFragment(self.currentStyle)
                frag.setText(self.content)
                self.content = ''
                parent.add(frag)

            self.currentStyle = ('code', None, None)    # todo: nested styles support (needs yet another stack ...)
        elif name == 'link':
            # Add content so far to the current paragraph
            if len(self.content) > 0:
                parent = self.nodeStack[-1]
                frag = TextFragment(self.currentStyle)
                frag.setText(self.content)
                self.content = ''
                parent.add(frag)

            self.href = attrs.get('xlink:href', '')
            self.currentStyle = None            # todo: nested styles support (needs yet another stack ...)
        elif name == 'olink':
            # Add content so far to the current paragraph
            if len(self.content) > 0:
                parent = self.nodeStack[-1]
                frag = TextFragment(self.currentStyle)
                frag.setText(self.content)
                self.content = ''
                parent.add(frag)

            self.currentStyle = ('olink', None, None)    # todo: nested styles support (needs yet another stack ...)
        elif name == 'imagedata':
            parent = self.nodeStack[-1]

            # Add content so far to the current paragraph
            if len(self.content) > 0:
                frag = TextFragment(self.currentStyle)
                frag.setText(self.content)
                self.content = ''
                parent.add(frag)

            imageFile = attrs.getValue('fileref')
            frag = ImageFragment()
            frag.setImage(imageFile)
            parent.add(frag)
        elif name == 'mathphrase':
            parent = self.nodeStack[-1]

            # Add content so far to the current paragraph
            if len(self.content) > 0:
                frag = TextFragment(self.currentStyle)
                frag.setText(self.content)
                parent.add(frag)

            self.content = ''

    def characters(self, data):
        if self.content is not None:
            self.content = self.content + data

    def endElement(self, name):
        # structural tags
        if name == 'article':
            self.result = self.nodeStack[0]
        elif name == 'section':
            self.sectionLevel -= 1
        elif name == 'itemizedlist':
            self.nodeStack = self.nodeStack[0:-1]     # pop()
            self.content = None
            self.currentStyle = None
            self.listLevel -= 1
        elif name == 'mediaobject':
            pass
        elif name == 'imageobject':
            pass
        elif name == 'inlineequation':
            pass

        # These contain <para> elements:
        elif name == 'listitem':
            pass
        elif name == 'blockquote':
            pass
        elif name == 'tip':
            pass
        elif name == 'warning':
            pass

        elif name == 'title':
            if self.sectionLevel > 0:
                parent = self.nodeStack[-1]
                self.nodeStack = self.nodeStack[0:-1]     # pop()
                frag = TextFragment(None)
                frag.setText(self.content)
                parent.add(frag)
                self.content = None
        elif name == 'para':
            if self.content != '':
                parent = self.nodeStack[-1]

                frag = TextFragment(self.currentStyle)
                frag.setText(self.content)
                self.content = None
                parent.add(frag)
                self.currentStyle = None            # todo: nested styles support (needs yet another stack ...)
                self.paraStyle = None

            self.nodeStack = self.nodeStack[0:-1]     # pop()
        elif name == 'programlisting':
            if self.content != '':
                parent = self.nodeStack[-1]

                frag = TextFragment(self.currentStyle)
                frag.setText(self.content)
                self.content = None
                parent.add(frag)
                self.currentStyle = None            # todo: nested styles support (needs yet another stack ...)

            self.nodeStack = self.nodeStack[0:-1]     # pop()
        elif name == 'screen':
            if self.content != '':
                parent = self.nodeStack[-1]

                frag = TextFragment(self.currentStyle)
                frag.setText(self.content)
                self.content = None
                parent.add(frag)
                self.currentStyle = None            # todo: nested styles support (needs yet another stack ...)

            self.nodeStack = self.nodeStack[0:-1]     # pop()

        elif name == 'emphasis':
            parent = self.nodeStack[-1]

            frag = TextFragment(self.currentStyle)
            frag.setText(self.content)
            self.content = ''
            parent.add(frag)
            self.currentStyle = None                # todo: nested styles support (needs yet another stack ...)
        elif name == 'code':
            parent = self.nodeStack[-1]

            frag = TextFragment(self.currentStyle)
            frag.setText(self.content)
            self.content = ''
            parent.add(frag)
            self.currentStyle = None                # todo: nested styles support (needs yet another stack ...)
        elif name == 'link':
            parent = self.nodeStack[-1]

            frag = TextFragment(self.currentStyle)
            frag.setText(self.content)
            frag.setHref(self.href)
            self.href = None
            self.content = ''
            parent.add(frag)
            self.currentStyle = None                # todo: nested styles support (needs yet another stack ...)
        elif name == 'olink':
            parent = self.nodeStack[-1]

            frag = TextFragment(self.currentStyle)
            frag.setText(self.content)
            parent.add(frag)

            self.keywordLinks.add(self.content) 

            self.content = ''
            self.currentStyle = None                # todo: nested styles support (needs yet another stack ...)
        elif name == 'imagedata':
            pass
        elif name == 'mathphrase':
            mathFormula = MathFormulaObject()
            mathFormula.setFormula(self.content)
            mathFormula.renderFormula()             # generate image - TODO: is there a better approach?
                                                    # Do we need the image as part of the fragment?
            parent = self.nodeStack[-1]
            frag = MathFragment()
            frag.setText(mathFormula.formula)
            frag.setImage(mathFormula.image)
            parent.add(frag)

            self.currentStyle = None                # todo: nested styles support (needs yet another stack ...)
            self.content = ''

class XMLImporter:

    def __init__(self, contentPath, contentFile, formatManager):
        self.contentPath = contentPath
        self.contentFile = contentFile
        self.formatManager = formatManager


    def importDocument(self):
        contentFilePath = os.path.join(self.contentPath, self.contentFile)
        with open(contentFilePath, 'r', encoding='utf-8') as content_file:
            self.importFromFile(content_file)


    def importFromFile(self, fileDesc):
        # Step 1: read the XML file and create the document structure with the 
        # corresponding styles
        parser = xml.sax.make_parser()
        handler = Handler(self.contentPath)
        parser.setContentHandler(handler)
        parser.parse(fileDesc)

        self.links = sorted(handler.keywordLinks)
        documentModel = handler.result

        # Step 2: Create a QTextDocument from the document tree 
        df = DocumentFactory(self.contentPath, self.formatManager)
        self.document = df.createDocument(documentModel)


    def getDocument(self):
        return self.document
    

    def getLinks(self):
        return self.links
