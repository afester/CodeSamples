'''
Created on 10.03.2015

@author: afester
'''

from PyQt5.Qt import QTextTable, QTextFormat
from xml.sax.saxutils import escape
import tools



class TextDocumentTraversal:
    '''Generic implementation of the document structure traversal algorithm.
    This class can be subclassed to write out the document in different
    formats.'''

    def __init__(self):
        pass

    def traverse(self, document):
        frm = document.rootFrame()

        self.indent = 0
        self.currentLevel = 0

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
            
            self.indent -= 1


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
        if textList:
            self.dumpListItem(block)
        else:

            for x in range(0, self.currentLevel):
                self.indent -= 1
                self.endList('ul')
            self.currentLevel = 0

            blockFormat = block.blockFormat()
            blockStyle = blockFormat.property(QTextFormat.UserProperty)

            self.beginBlock(blockStyle)
    
            self.indent += 1
            iterator = block.begin()
            while not iterator.atEnd():
                fragment = iterator.fragment()
                self.dumpFragment(fragment)
                iterator += 1
    
            self.indent -= 1

            self.endBlock(blockStyle)


    def dumpListItem(self, block):
        textList = block.textList()
        listFormat = textList.format()
        level = listFormat.indent()
        itemNumber = textList.itemNumber(block)

        for x in range(self.currentLevel, level):
            self.beginList('ul')
            self.indent += 1
        for x in range(level, self.currentLevel):
            self.indent -= 1
            self.endList('ul')
        self.currentLevel = level

        self.beginItem()

        # add the fragments
        self.indent += 1
        iterator = block.begin()
        while not iterator.atEnd():
            fragment = iterator.fragment()
            self.dumpFragment(fragment)
            iterator += 1

        self.indent -= 1

        self.endItem()


    def dumpFragment(self, fragment):
        text = fragment.text().replace('\u2028', '\n')

        charFormat = fragment.charFormat()
        styleClass = charFormat.property(QTextFormat.UserProperty)

        closeAnchor = False
        if styleClass == 'a':
            href = escape(charFormat.anchorHref())
            self.beginLink(href)
            closeAnchor = True
            styleClass = None
        
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
            self.fragment(text, styleClass)

        if closeAnchor:
            self.endLink()


    def beginDocument(self):
        pass

    def endDocument(self):
        pass

    def beginList(self, style):
        pass
    
    def beginItem(self):
        pass

    def endItem(self):
        pass

    def endList(self, style):
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
        return '\u00a0' * self.indent * 4

    def beginDocument(self):
        self.emitter('\n{}<QTextDocument>'.format(self.prefix()))

    def endDocument(self):
        self.emitter('\n{}</QTextDocument>'.format(self.prefix()))

    def beginList(self, style):
        self.emitter('\n{}<QTextList style="{}">'.format(self.prefix(), style))

    def beginItem(self):
        self.firstFragment = True
        self.emitter('\n{}<item>'.format(self.prefix()))

    def endItem(self):
        self.emitter('\n{}</item>'.format(self.prefix()))

    def endList(self, style):
        self.emitter('\n{}</QTextList>'.format(self.prefix()))

    def beginBlock(self, style):
        self.firstFragment = True
        self.emitter('\n{}<QTextBlock style="{}">'.format(self.prefix(), style))

    def endBlock(self, style):
        self.emitter('\n{}</QTextBlock>'.format(self.prefix()))

    def fragment(self, text, style):
        if self.firstFragment:
            self.firstFragment = False 
            self.emitter('\n{}!{}|{}|'.format(self.prefix(), style, text))
        else:
            self.emitter('!{}|{}|'.format(style, text))

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

#===============================================================================
# 
# class TextDocbookPrinter(TextDocumentTraversal):
#     '''Experimental'''
# 
#     def __init__(self, emitter):
#         self.emitter = emitter
#         self.currentSectionLevel = 0
# 
#     def prefix(self):
#         return '\u00a0' * self.indent * 2
# 
#     def beginDocument(self):
#         self.emitter('<?xml version="1.0" encoding="UTF-8"?>\n<page>\n')
# 
#     def endDocument(self):
#         self.emitter('\n{}</page>'.format(self.prefix()))
# 
#     def beginList(self):
#         self.emitter('\n{}<ul>'.format(self.prefix()))
# 
#     def beginItem(self):
#         self.emitter('\n{}<li>'.format(self.prefix()))
# 
#     def endItem(self):
#         self.emitter('</li>')
# 
#     def endList(self):
#         self.emitter('\n{}</ul>'.format(self.prefix()))
# 
#     def beginBlock(self, style):
#         if style in ['xmlcode', 'javacode', 'cppcode']:
#             style = 'programlisting'
#             self.emitter('\n{}<{} language="cpp">'.format(self.prefix(), style))
#         elif style in ['h1', 'h2', 'h3']:
#             level = int(style[-1])  # TODO
# 
#             for x in range(level, self.currentSectionLevel):
#                 self.emitter('</section>')
#                 self.currentSectionLevel -= 1
#             self.emitter('<section>')
#             self.emitter('<title>')
#         else:
#             self.emitter('\n{}<{}>'.format(self.prefix(), style))
# 
#     def endBlock(self, style):
#         if style in ['h1', 'h2', 'h3']:
#             self.emitter('</title>')
#         else:
#             self.emitter('</{}>'.format(style))
# 
#     def fragment(self, text, style):
#         if style is None:
#             self.emitter(text)
#         else:
#             self.emitter('<{}>{}</{}>'.format(style, text, style))
# 
#     def image(self, imgName):
#         self.emitter('<imageobject><imagedata fileref="{}"/></imageobject>'.format(imgName))
#===============================================================================

class TextXMLPrinter(TextDocumentTraversal):

    def __init__(self, emitter):
        self.emitter = emitter
        self.currentSectionLevel = 0

    def prefix(self):
        return ' ' * self.indent * 2

    def beginDocument(self):
        self.emitter('<?xml version="1.0" encoding="UTF-8"?>\n<page>')

    def endDocument(self):
        self.emitter('\n{}</page>'.format(self.prefix()))

    def beginList(self, style):
        self.emitter('\n{}<{}>'.format(self.prefix(), style))

    def beginItem(self):
        self.emitter('\n{}<li>'.format(self.prefix()))

    def endItem(self):
        self.emitter('</li>')

    def endList(self, style):
        self.emitter('\n{}</{}>'.format(self.prefix(), style))

    def beginBlock(self, style):
        if style in ['xmlcode', 'javacode', 'cppcode']:
            language = tools.rchop(style, 'code')
            self.emitter('\n{}<programlisting language="{}">'.format(self.prefix(), language))
        else:
            self.emitter('\n{}<{}>'.format(self.prefix(), style))

    def endBlock(self, style):
        if style in ['xmlcode', 'javacode', 'cppcode']:
            self.emitter('</programlisting>')
        else:
            self.emitter('</{}>'.format(style))

    def fragment(self, text, style):
        if style is None:
            self.emitter(text)
        else:
            self.emitter('<{}>{}</{}>'.format(style, text, style))

    def beginLink(self, destination):
        self.emitter('<a href="{}">'.format(destination))

    def endLink(self):
        self.emitter('</a>')

    def image(self, imgName):
        self.emitter('<img src="{}"/>'.format(imgName))


class TextHTMLPrinter(TextDocumentTraversal):

    def __init__(self, emitter):
        self.emitter = emitter
        self.currentSectionLevel = 0

    def prefix(self):
        return ' ' * self.indent * 2

    def beginDocument(self):
        self.emitter('''<!DOCTYPE html>
<html lang="en">
  <head></head>
  <body>\n''')
        self.indent += 1

    def endDocument(self):
        self.emitter('\n\n{}</body>\n</html>'.format(self.prefix()))

    def beginList(self, style):
        self.emitter('\n{}<{}>'.format(self.prefix(), style))

    def beginItem(self):
        self.emitter('\n{}<li>'.format(self.prefix()))

    def endItem(self):
        self.emitter('</li>')

    def endList(self, style):
        self.emitter('\n{}</{}>'.format(self.prefix(), style))

    def beginBlock(self, style):
        if style in ['xmlcode', 'javacode', 'cppcode']:
            language = tools.rchop(style, 'code')
            self.emitter('\n{}<pre><code class="{}">'.format(self.prefix(), language))
        elif style == 'screen':
            self.emitter('\n{}<pre>'.format(self.prefix()))
        else:
            self.emitter('\n{}<{}>'.format(self.prefix(), style))

    def endBlock(self, style):
        if style in ['xmlcode', 'javacode', 'cppcode']:
            self.emitter('</code></pre>')
        elif style == 'screen':
            self.emitter('\n{}</pre>'.format(self.prefix()))
        else:
            self.emitter('</{}>'.format(style))

    def fragment(self, text, style):
        if style is None:
            self.emitter(text)
        else:
            self.emitter('<{}>{}</{}>'.format(style, text, style))

    def beginLink(self, destination):
        self.emitter('<a href="{}">'.format(destination))

    def endLink(self):
        self.emitter('</a>')

    def image(self, imgName):
        self.emitter('<img src="{}"/>'.format(imgName))
