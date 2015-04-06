'''
Created on 10.03.2015

@author: afester
'''

from PyQt5.Qt import QTextTable, QTextFormat
from xml.sax.saxutils import escape
import tools
import urllib


class TextDocumentTraversal:
    '''Generic implementation of the document structure traversal algorithm.
    This class can be subclassed to write out the document in different
    formats.'''

    def __init__(self):
        pass

    def traverse(self, document):
        frm = document.rootFrame()

        self.indent = 0
        self.listStack = []
        self.currentList = 0
        self.beginDocument()

        self.dumpFrame(frm)

        self.endDocument()
        

    def dumpFrame(self, frm):

        if type(frm) is QTextTable:
            self.dumpTable(frm)
        else:
            self.indent += 1
            iterator = frm.begin()
            while not iterator.atEnd():
                block = None
                frame = iterator.currentFrame() # None if not a frame
                block = iterator.currentBlock() # Invalid block if its a frame
    
                if frame:
                    self.dumpFrame(frame)
                else:
                    self.dumpBlock(block)
    
                iterator += 1

            self.closePendingLists()

            self.indent -= 1
            self.currentList = 0

    def dumpTable(self, table):
        self.textView.append('{}Table ({}, {})'.format(self.prefix(), table.rows(), table.columns()))
        self.indent += 1
        for row in range(0, table.rows()):
            self.textView.append('{}Row'.format(self.prefix()))
            self.indent += 1
            for column in range(0, table.columns()):
                tableCell = table.cellAt(row, column)
                self.textView.append('{}Cell'.format(self.prefix()))

                # PyQt's QTextTableCell does not expose the frame iterator (begin(), end()) 
        #=======================================================================
        #         # Same as above when iterating the frame contents!
        #         self.indent += 1
        #         iterator = tableCell.begin()        # AttributeError: 'QTextTableCell' object has no attribute 'begin'
        #         while not iterator.atEnd():
        #             block = None
        #             frame = iterator.currentFrame() # None if not a frame
        #             block = iterator.currentBlock() # Invalid block if its a frame
        # 
        #             if frame:
        #                 self.dumpFrame(frame)
        #             else:
        #                 self.dumpBlock(block)
        # 
        #             iterator += 1
        #         
        #         self.indent -= 1
        #=======================================================================

            self.indent -= 1
        self.indent -= 1


    def dumpBlock(self, block):

        textList = block.textList()
        if textList:                        # block is part of a list (it IS a list item)
            listFormat = textList.format()
            listStyle = listFormat.property(QTextFormat.UserProperty)

            self.beginItem(listStyle)
            self.currentList = listFormat.indent()

            self.indent += 1
            iterator = block.begin()
            if not iterator.atEnd():
                self.traverseBlock(block)

            self.endItem()
        else:
            self.closePendingLists()
            self.currentList = 0

            self.traverseBlock(block)


    def traverseBlock(self, block):
        iterator = block.begin()

        blockFormat = block.blockFormat()
        blockStyle = blockFormat.property(QTextFormat.UserProperty)

        self.beginBlock(blockStyle)
        self.indent += 1

        while not iterator.atEnd():
            fragment = iterator.fragment()
            self.dumpFragment(fragment)
            iterator += 1

        self.indent -= 1
        self.endBlock(blockStyle)


    def dumpFragment(self, fragment):
        text = fragment.text().replace('\u2028', '\n')

        charFormat = fragment.charFormat()
        style = charFormat.property(QTextFormat.UserProperty)

        closeAnchor = False
        if style and style[0] == 'link':
            href = escape(charFormat.anchorHref())
            self.beginLink(href)
            closeAnchor = True
            style = None

        isObject = (text.find('\ufffc') != -1)
        isImage = isObject and charFormat.isImageFormat()

        if isImage:
            imgFmt = charFormat.toImageFormat()
            imgName = tools.os_path_split(imgFmt.name())[-1]

            # If the same image repeats multiple times, it is simply represented by
            # yet another object replacement character ...
            for img in range(0, len(text)):
                self.image(imgName)
        else:
            text = escape(text)
            self.fragment(text, style)

        if closeAnchor:
            self.endLink()


    def beginDocument(self):
        pass

    def endDocument(self):
        pass

    def beginItem(self, style):
        pass

    def endItem(self):
        pass

    def closePendingLists(self):
        pass

    def beginBlock(self, style):
        pass
    
    def endBlock(self, style):
        pass

    def fragment(self, text, style):
        pass

    def beginLink(self, destination):
        pass

    def endLink(self):
        pass

    def image(self, name):
        pass


class TextDocumentPrinter(TextDocumentTraversal):

    def __init__(self, emitter):
        self.emitter = emitter
        self.firstFragment = True
        
    def prefix(self):
        return '\u00a0' * self.indent * 2

    def beginDocument(self):
        self.emitter('{}<QTextDocument>'.format(self.prefix()))

    def endDocument(self):
        self.emitter('\n{}</QTextDocument>'.format(self.prefix()))



    def beginItem(self, style):
        level = int(style[2])

        self.firstFragment = True

        # if the level goes back, then close any previous lists
        for x in range(level, self.currentList):
            self.indent -= 1
            self.emitter('\n{}</List>'.format(self.prefix()))

        # if the level goes up, then open any number of required lists
        for x in range(self.currentList, level):
            self.emitter('\n\n{}<List {}>'.format(self.prefix(), style))
            self.indent += 1
        
        self.indent -= 1    # TODO

    def endItem(self):
        pass

    def closePendingLists(self):
        for x in range(0, self.currentList):
            self.indent -= 1
            self.emitter('\n{}</List>'.format(self.prefix()))

    def beginBlock(self, style):
        self.firstFragment = True
        self.emitter('\n\n{}<QTextBlock {}>'.format(self.prefix(), style))

    def endBlock(self, style):
        self.emitter('\n{}</QTextBlock>'.format(self.prefix()))

    def fragment(self, text, style):
        if self.firstFragment:
            self.firstFragment = False 
            self.emitter('\n{}<QTextFragment style="{}">{}</QTextFragment>'.format(self.prefix(), style, text))
        else:
            self.emitter('<QTextFragment style="{}">{}</QTextFragment>'.format(style, text))

    def beginLink(self, destination):
        pass

    def endLink(self):
        pass

    def image(self, imgName):
        if self.firstFragment:
            self.firstFragment = False 
            self.emitter('\n{}<img src="{}"/>'.format(self.prefix(), imgName))
        else:
            self.emitter('<img src="{}"/>'.format(imgName))


class TextXMLPrinter(TextDocumentTraversal):

    def __init__(self, emitter):
        self.emitter = emitter
        self.currentSectionLevel = 0

    def prefix(self):
        return ' ' * self.indent * 2

    def beginDocument(self):
        self.emitter('''<?xml version="1.0" encoding="utf-8"?>
<article version="5.0" xml:lang="en"
         xmlns="http://docbook.org/ns/docbook"
         xmlns:xlink="http://www.w3.org/1999/xlink">
  <title></title>''')

    def endDocument(self):
        for x in range(0, self.currentSectionLevel):
            self.emitter('\n{}</section>'.format(self.prefix()))
            self.currentSectionLevel -= 1
            self.indent -= 1
        self.emitter('\n{}</article>'.format(self.prefix()))


    def beginItem(self, style):
        level = int(style[2])

        # if we are currently in a list, then close previous list item
        if level <= self.currentList:
            self.indent -= 1
            self.emitter('\n{}</listitem>'.format(self.prefix()))

        # if the level goes back, then close any previous lists
        for x in range(level, self.currentList):
            self.indent -= 1
            self.emitter('\n{}</itemizedlist>'.format(self.prefix()))
            self.indent -= 1
            self.emitter('\n{}</listitem>'.format(self.prefix()))  # TODO: This is NOT always correct.
                                                                    # If a new list starts with a level > 1, 
                                                                    # there will be NO corresponding <listitem>!

        # if the level goes up, then open any number of required lists
        for x in range(self.currentList, level):
            self.emitter('\n{}<itemizedlist>'.format(self.prefix()))
            self.indent += 1

        # we always DO have a list item here, so open one
        self.emitter('\n{}<listitem>'.format(self.prefix()))


    def closePendingLists(self):
        for x in range(0, self.currentList):
            self.indent -= 1
            self.emitter('\n{}</listitem>'.format(self.prefix()))
            self.indent -= 1
            self.emitter('\n{}</itemizedlist>'.format(self.prefix()))


    def beginBlock(self, style):

        if style[0] == 'programlisting':
            self.emitter('\n{}<programlisting language="{}">'.format(self.prefix(), style[2]))
        
        elif style[0] == 'title':
            # close previous sections
            newLevel = int(style[2])
            if newLevel == self.currentSectionLevel:
                self.indent -= 1
                self.emitter('\n{}</section>'.format(self.prefix()))
                self.emitter('\n\n{}<section>'.format(self.prefix()))
                self.indent += 1
            else:
                for x in range(newLevel, self.currentSectionLevel):
                    self.emitter('\n{}</section>'.format(self.prefix()))
                    self.currentSectionLevel -= 1
                    self.indent -= 1

                # Open sections until the required level is reached
                for x in range(self.currentSectionLevel, newLevel):
                    self.emitter('\n\n{}<section>'.format(self.prefix()))
                    self.indent += 1
                    self.currentSectionLevel += 1

            self.emitter('\n{}<title>'.format(self.prefix()))

        elif style[0] == 'tip':
            self.emitter('\n{}<tip><para>'.format(self.prefix()))
        else:
            self.emitter('\n{}<{}>'.format(self.prefix(), style[0]))

    def endBlock(self, style):
        if style[0] == 'tip':
            self.emitter('</para></tip>'.format())
            pass
        else:
            self.emitter('</{}>'.format(style[0]))

    def fragment(self, text, style):
        if style is None:
            self.emitter(text)
        elif style[0] == 'emphasis':
            if style[2] is not None and style[2] == 'highlight':
                self.emitter('<emphasis role="highlight">{}</emphasis>'.format(text))
            else:
                self.emitter('<emphasis>{}</emphasis>'.format(text))
        elif style[0] == 'olink':
            linkend = urllib.parse.quote(text, '')
            self.emitter('<olink targetdoc="{}">{}</olink>'.format(linkend, text))
        else:
            self.emitter('<{}>{}</{}>'.format(style[0], text, style[0]))

    def beginLink(self, destination):
        self.emitter('<link xlink:href="{}">'.format(destination))

    def endLink(self):
        self.emitter('</link>')

    def image(self, imgName):
        self.emitter('<mediaobject><imageobject><imagedata fileref="{}"/></imageobject></mediaobject>'.format(imgName))


class TextHTMLPrinter(TextDocumentTraversal):

    def __init__(self, emitter, page):
        self.emitter = emitter
        self.currentSectionLevel = 0
        self.page = page

    def prefix(self):
        return ' ' * self.indent * 2

    def beginDocument(self):
        self.emitter('''<!DOCTYPE html>
<html lang="en">
  <head></head>
  <body>''')
        self.indent += 1

    def endDocument(self):
        self.emitter('\n\n{}</body>\n</html>'.format(self.prefix()))


    def beginItem(self, style):
        level = int(style[2])

        # if the level goes back, then close any previous lists
        for x in range(level, self.currentList):
            self.indent -= 1
            self.emitter('\n{}</ul>'.format(self.prefix()))

        # if the level goes up, then open any number of required lists
        for x in range(self.currentList, level):
            self.emitter('\n{}<ul>'.format(self.prefix()))
            self.indent += 1

        # we always DO have a list item here, so open one
        self.emitter('\n{}<li>'.format(self.prefix()))

    def endItem(self):
        self.indent -= 1
        self.emitter('</li>')

    def closePendingLists(self):
        for x in range(0, self.currentList):
            self.indent -= 1
            self.emitter('\n{}</ul>'.format(self.prefix()))


    def beginBlock(self, style):

        if style[0] == 'programlisting':
            self.emitter('\n{}<pre class="code"><code class="{}">'.format(self.prefix(), style[2]))
        elif style[0] == 'screen':
            self.emitter('\n{}<pre>'.format(self.prefix()))
        elif style[0] == 'title':
            self.emitter('\n\n{}<h{}>'.format(self.prefix(), int(style[2])))
        elif style[0] == 'tip':
            self.emitter('\n{}<table class="tip"><tr><td><img src="icons/tip.png" /></td><td>'.format(self.prefix()))
        elif style[0] == 'para':
            if self.currentList == 0:
                self.emitter('\n{}<p>'.format(self.prefix()))
        else:
            print("INVALID FORMAT: {}".format(style))

    def endBlock(self, style):
        if style[0] == 'programlisting':
            self.emitter('</code></pre>')
        elif style[0] == 'screen':
            self.emitter('</pre>')
        elif style[0] == 'title':
            self.emitter('</h{}>'.format(int(style[2])))
        elif style[0] == 'tip':
            self.emitter('</td></tr></table>')
        elif style[0] == 'para':
            if self.currentList == 0:
                self.emitter('</p>')
        else:
            print("INVALID FORMAT: {}".format(style))

    def fragment(self, text, style):
        if style is None or style[0] is None:
            self.emitter(text)
        elif style[0] == 'olink':
            self.emitter('<i>{}</i>'.format(text))
        elif style[0] == 'code':
            self.emitter('<tt>{}</tt>'.format(text))
        elif style[0] == 'emphasis':
            if style[1] == 'role' and style[2] == 'highlight':
                self.emitter('<em>{}</em>'.format(text))
            else:
                self.emitter('<strong>{}</strong>'.format(text))
        else:
            self.emitter('<{}>{}</{}>'.format(style, text, style))

    def beginLink(self, destination):
        self.emitter('<a href="{}">'.format(destination))

    def endLink(self):
        self.emitter('</a>')

    def image(self, imgName):
        prefix = "file:///{}/".format(self.page.getPageDir())
        self.emitter('<img src="{}"/>'.format(prefix + imgName))
