'''
Created on 10.03.2015

@author: afester
'''

from PyQt5.Qt import QTextTable, QTextDocument, QTextCursor, QTextFormat, QTextCharFormat, QTextImageFormat
from xml.sax.saxutils import escape
import tools
import os, urllib
from EditorWidget import MathFormula

class Node:

    def __init__(self):
        self.children = []

    def add(self, child):
        self.children.append(child)

    def __repr__(self):
        return self.__str__()


# A Frame is a container for child Frames and Paragraphs
class Frame(Node):

    def __init__(self):
        Node.__init__(self)

    def __str__(self):
        return 'Frame'


# A Paragraph is a container for Fragments
class Paragraph(Node):

    def __init__(self, indentLevel, style):
        Node.__init__(self)

        self.indent = indentLevel
        self.style = style

    def __str__(self):
        return 'Paragraph[style={}]'.format(self.style)


# A List is a container for Paragraphs and Lists on a deeper level
class List(Node):

    def __init__(self, style):
        Node.__init__(self)
        self.style = style

    def __str__(self):
        return 'List[style={}]'.format(self.style)



class Table:
    pass


# Fragments do not have children
class Fragment:

    def __init__(self, style):
        self.href = None

        self.style = style

    def setHref(self, text):
        self.href = text

    def __repr__(self):
        return self.__str__()


class TextFragment(Fragment):

    def __init__(self, style):
        Fragment.__init__(self, style)
        self.text = None

    def setText(self, text):
        self.text = text

    def __str__(self):
        return 'TextFragment[style={}, text="{}", href={}]'.format(self.style, self.text, self.href)


class ImageFragment(Fragment):

    def __init__(self):
        Fragment.__init__(self, (None, None, None))
        self.image = None

    def setImage(self, img):
        self.image = img

    def __str__(self):
        return 'ImageFragment[image={}, href={}]'.format(self.image, self.href)


class MathFragment(Fragment):

    def __init__(self):
        Fragment.__init__(self, (None, None, None))
        self.image = None
        self.text = None

    def setText(self, text):
        self.text = text

    def setImage(self, img):
        self.image = img

    def __str__(self):
        return 'MathFragment[text="{}", image={}, href={}]'.format(self.text, self.image, self.href)



class TextDocumentTraversal:

    def __init__(self):
        pass

    def traverse(self, document):
        frm = document.rootFrame()
        result = self.dumpFrame(frm)

        return result


    def dumpFrame(self, frm):

        parentStack = [Frame()]

        currentIndent = 0       # current indent within this frame
        if type(frm) is QTextTable:
            tbl = self.dumpTable(frm)
        else:
            iterator = frm.begin()
            while not iterator.atEnd():
                frame = iterator.currentFrame() # None if not a frame
                block = iterator.currentBlock() # Invalid block if its a frame

                if frame:
                    subframe = self.dumpFrame(frame)
                    # ... TODO
                else:

                    # A block is not further structured (from a QTextDocument's point of view).
                    # We can get the whole block here and handle the inherent structure below.
                    para = self.getParagraph(block)
                    listLevel = para.indent

                    # Add additional List elements as required
                    for x in range(currentIndent, listLevel):
                        l = List(('itemizedlist', 'level', x))
                        parent = parentStack[x]
                        parent.add(l)
                        parent = l
                        if len(parentStack) > x+1:
                            parentStack[x+1] = parent
                        else:
                            parentStack.append(parent)

                    parent = parentStack[listLevel]
                    parent.add(para)

                    currentIndent = listLevel

                iterator += 1

        return parentStack[0]


    def dumpTable(self, table):
        raise None
    
        for row in range(0, table.rows()):
            for column in range(0, table.columns()):
                tableCell = table.cellAt(row, column)

                # BUG: PyQt's QTextTableCell does not expose the frame iterator (begin(), end()) 
                iterator = tableCell.begin()        # AttributeError: 'QTextTableCell' object has no attribute 'begin'
                while not iterator.atEnd():
                    pass

        return Table()  # Mock


    def getParagraph(self, block):
        blockFormat = block.blockFormat()
        blockStyle = blockFormat.property(QTextFormat.UserProperty)

        listLevel = 0
        textList = block.textList()
        if textList:
            listFormat = textList.format()
            listLevel = listFormat.indent()
       
        result = Paragraph(listLevel, blockStyle)

        iterator = block.begin()
        while not iterator.atEnd():
            fragment = iterator.fragment()

            for frag in self.getFragments(fragment):   # Fragment could contain more than one image!
                result.add(frag)
            iterator += 1

        return result


    def getFragments(self, fragment):
        result = []

        text = fragment.text().replace('\u2028', '\n')

        charFormat = fragment.charFormat()
        style = charFormat.property(QTextFormat.UserProperty)

        href = None
        if style and style[0] == 'link':
            href = charFormat.anchorHref()  #escape(charFormat.anchorHref())
            style = None     # Link implicitly defined by setting href

        isObject = (text.find('\ufffc') != -1)
        if isObject:
            if charFormat.isImageFormat():
                imgFmt = charFormat.toImageFormat()
                imgName = tools.os_path_split(imgFmt.name())[-1]
    
                # If the same image repeats multiple times, it is simply represented by
                # yet another object replacement character ...
                for img in range(0, len(text)):
                    frag = ImageFragment()
                    frag.image = imgName
                    frag.setHref(href)
                    result.append(frag)
            else:
                mathFormula = charFormat.property(QTextCharFormat.UserProperty+1)
                frag = MathFragment()
                frag.setText(mathFormula.formula)
                frag.image = mathFormula.image
                result.append(frag)
        else:
            # text = escape(text)
            frag = TextFragment(style)
            frag.setHref(href)
            frag.setText(text)
            result.append(frag)

        return result


class StructurePrinter:
    
    def __init__(self, root, out):
        self.rootFrame = root
        self.indent = 0
        self.out = out

    def prefix(self):
        return '\u00a0' * self.indent * 2

    def traverse(self):
        self.recursiveTraverse(self.rootFrame)

    def recursiveTraverse(self, parent):
        self.out("{}{}\n".format(self.prefix(), parent))
        if type(parent) == Paragraph or type(parent) == Frame or type(parent) == List:
            for c in parent.children:
                self.indent += 1
                self.recursiveTraverse(c)
                self.indent -= 1


class HtmlPrinter:

    def __init__(self, root, out, baseDir):
        self.rootFrame = root
        self.indent = 0
        self.out = out
        self.listLevel = 0
        self.baseDir= baseDir

    def prefix(self):
        return '\u00a0' * self.indent * 2

    def traverse(self):
        self.out('''<!DOCTYPE html>
<html lang="en">
  <head></head>
  <body>''')

        self.indent = 2
        for c in self.rootFrame.children:
            self.visitNode(c)

        self.out('''

  </body>
</html>''')



    def visitNode(self, node):
        if type(node) == Fragment:
            self.emitFragment(node)

        elif type(node) == Paragraph:
            if node.style[0] == 'programlisting':
                self.out('\n{}<pre class="code"><code class="{}">'.format(self.prefix(), node.style[2]))
            elif node.style[0] == 'screen':
                self.out('\n{}<pre>'.format(self.prefix()))
            elif node.style[0] == 'title':
                self.out('\n\n{}<h{}>'.format(self.prefix(), int(node.style[2])))
            elif node.style[0] == 'tip':
                self.out('\n{}<table class="tip"><tr><td><img src="icons/tip.png" /></td><td>'.format(self.prefix()))
            elif node.style[0] == 'warning':
                self.out('\n{}<table class="warning"><tr><td><img src="icons/warning.png" /></td><td>'.format(self.prefix()))
            elif node.style[0] == 'blockquote':
                self.out('\n{}<blockquote>'.format(self.prefix()))
            elif node.style[0] == 'para':
                if self.listLevel == 0:
                    self.out('\n{}<p>'.format(self.prefix()))
                else:
                    self.out('\n{}<li>'.format(self.prefix()))

            self.indent += 1
            for c in node.children:
                self.emitFragment(c)
            self.indent -= 1

            if node.style[0] == 'programlisting':
                self.out('</code></pre>')
            elif node.style[0] == 'screen':
                self.out('</pre>')
            elif node.style[0] == 'title':
                self.out('</h{}>'.format(int(node.style[2])))
            elif node.style[0] in ('tip', 'warning'):
                self.out('</td></tr></table>')
            elif node.style[0] == 'blockquote':
                self.out('</blockquote>')
            elif node.style[0] == 'para':
                if self.listLevel == 0:
                    self.out("</p>")
                else:
                    self.out('</li>')

        elif type(node) == List:
            self.out('\n{}<ul>'.format(self.prefix()))
            self.listLevel += 1

            self.indent += 1
            for c in node.children:
                self.visitNode(c)

            self.indent -= 1

            self.out('\n{}</ul>'.format(self.prefix()))
            self.listLevel -= 1


    def emitFragment(self, fragment):
        if fragment.href is not None:
            self.out('<a href="{}">'.format(fragment.href))

        if type(fragment) == ImageFragment:
            imageName = escape(fragment.image)
            prefix = "file:///{}/".format(self.baseDir)
            self.out('<img src="{}"/>'.format(prefix + imageName))
        elif type(fragment) == MathFragment:
########################### TODO ##################################
            import tempfile, os, uuid
            imgPath = tempfile.gettempdir()
            fileName = str(uuid.uuid4()).replace('-', '') + '.png'
            filePath = os.path.join(imgPath, fileName)
            fragment.image.save(filePath)
###################################################################

            imgName = escape(filePath)
            formula = escape(fragment.text)
            self.out('<img class="math" alt="{}" src="file:///{}"/>'.format(formula, imgName))
        elif type(fragment) == TextFragment:

            text = escape(fragment.text)
            if fragment.style is None:
                self.out(text)
            elif fragment.style[0] == 'olink':
                linkend = urllib.parse.quote(text, '')
                self.out('<a href="{}">{}</a>'.format(linkend, text))
            elif fragment.style[0] == 'emphasis':
                if fragment.style[2] is not None and fragment.style[2] == 'highlight':
                    self.out('<em>{}</em>'.format(text))
                else:
                    self.out('<strong>{}</strong>'.format(text))
            elif fragment.style[0] == 'code':
                self.out('<tt>{}</tt>'.format(text))
            else:
                self.out('<{}>{}</{}>'.format(fragment.style[0], text, fragment.style[0]))

        if fragment.href is not None:
            self.out('</a>')


class DocbookPrinter:

    def __init__(self, root, out):
        self.rootFrame = root
        self.indent = 0
        self.out = out
        self.sectionLevel = 0

    def prefix(self):
        return ' ' * self.indent * 2

    def traverse(self):
        self.out('''<?xml version="1.0" encoding="utf-8"?>
<article version="5.0" xml:lang="en"
         xmlns="http://docbook.org/ns/docbook"
         xmlns:xlink="http://www.w3.org/1999/xlink">
  <title></title>''')

        self.indent = 1
        for c in self.rootFrame.children:
            self.visitNode(c)

        for x in range(0, self.sectionLevel):
            self.indent -= 1
            self.out('\n{}</section>'.format(self.prefix()))

        self.out('\n</article>\n')


    def visitNode(self, node):
        if type(node) == Fragment:
            self.emitFragment(node)

        elif type(node) == Paragraph:
            if node.style[0] == 'programlisting':
                    self.out('\n{}<programlisting language="{}">'.format(self.prefix(), node.style[2]))
            elif node.style[0] == 'title':
                # close previous sections
                newLevel = int(node.style[2])
                if newLevel == self.sectionLevel:
                    self.indent -= 1
                    self.out('\n{}</section>'.format(self.prefix()))
                    self.out('\n\n{}<section>'.format(self.prefix()))
                    self.indent += 1
                else:
                    for x in range(newLevel, self.sectionLevel):
                        self.out('\n{}</section>'.format(self.prefix()))
                        self.sectionLevel -= 1
                        self.indent -= 1
    
                    # Open sections until the required level is reached
                    for x in range(self.sectionLevel, newLevel):
                        self.out('\n\n{}<section>'.format(self.prefix()))
                        self.indent += 1
                        self.sectionLevel += 1
    
                self.out('\n{}<title>'.format(self.prefix()))

            elif node.style[0] == 'tip':
                self.out('\n{}<tip><para>'.format(self.prefix()))
            elif node.style[0] == 'warning':
                self.out('\n{}<warning><para>'.format(self.prefix()))
            elif node.style[0] == 'blockquote':
                self.out('\n{}<blockquote><para>'.format(self.prefix()))
            else:
                self.out('\n{}<{}>'.format(self.prefix(), node.style[0]))

            self.indent += 1
            for c in node.children:
                self.emitFragment(c)
            self.indent -= 1

            if node.style[0] == 'tip':
                self.out('</para></tip>'.format())
            elif node.style[0] == 'warning':
                self.out('</para></warning>'.format())
            elif node.style[0] == 'blockquote':
                self.out('</para></blockquote>'.format())
            else:
                self.out('</{}>'.format(node.style[0]))

        elif type(node) == List:
            self.out('\n{}<itemizedlist>'.format(self.prefix()))

            first = True
            self.indent += 1
            for c in node.children:

                if type(c) == Paragraph and not first:
                    self.indent -= 1
                    self.out('\n{}</listitem>'.format(self.prefix()))
                if type(c) == Paragraph:
                    self.out('\n{}<listitem>'.format(self.prefix()))
                    first = False
                    self.indent += 1

                self.visitNode(c)

            if not first:
                self.indent -= 1
                self.out('\n{}</listitem>'.format(self.prefix()))
            self.indent -= 1

            self.out('\n{}</itemizedlist>'.format(self.prefix()))


    def emitFragment(self, fragment):

        if fragment.href is not None:
            href = escape(fragment.href) 
            self.out('<link xlink:href="{}">'.format(href))

        if type(fragment) == ImageFragment:
            imageName = escape(fragment.image)
            self.out('<mediaobject><imageobject><imagedata fileref="{}"/></imageobject></mediaobject>'.format(imageName))
        elif type(fragment) == MathFragment:
            formula = escape(fragment.text)
            self.out('<inlineequation><mathphrase>{}</mathphrase></inlineequation>'.format(formula))
        elif type(fragment) == TextFragment:

            text = escape(fragment.text)
            if fragment.style is None:
                self.out(text)
            elif fragment.style[0] == 'olink':
                linkend = urllib.parse.quote(fragment.text, '')
                self.out('<olink targetdoc="{}">{}</olink>'.format(linkend, text))
            elif fragment.style[0] == 'emphasis':
                if fragment.style[2] is not None and fragment.style[2] == 'highlight':
                    self.out('<emphasis role="highlight">{}</emphasis>'.format(text))
                else:
                    self.out('<emphasis>{}</emphasis>'.format(text))
            else:
                self.out('<{}>{}</{}>'.format(fragment.style[0], text, fragment.style[0]))

        if fragment.href is not None:
            self.out('</link>')


class DocumentFactory:
    
    def __init__(self, contentPath, formatManager):
        self.formatManager = formatManager
        self.contentPath = contentPath


    def createDocument(self, rootFrame):
        
        # Create empty document
        self.document = QTextDocument()
        self.document.setUndoRedoEnabled(False)
        self.document.setIndentWidth(20)
        self.cursor = QTextCursor(self.document)
        self.listLevel = 0
        self.paraFormat = None

        # add all root paragraphs
        for n in rootFrame.children:
            self.addNode(n)

        # Clean up the first paragraph if document is not empty
        self.cursor.movePosition(QTextCursor.Start)
        b = self.cursor.block()
        if b.length() == 1:
            cursor = QTextCursor(self.document.findBlockByLineNumber(0))
            cursor.select(QTextCursor.BlockUnderCursor)
            cursor.deleteChar()

        return self.document


    def addNode(self, node):
        if type(node) == Paragraph:
            self.paraFormat = self.formatManager.getFormat(node.style)

            # NOTE: "The block char format is the format used when inserting 
            #        text at the beginning of an empty block."
            #       See also below.
            self.cursor.insertBlock(self.paraFormat.getBlockFormat(), self.paraFormat.getCharFormat())
            # self.cursor.insertFragment(QTextDocumentFragment.fromPlainText(''))

            if self.listLevel > 0:
                # TODO: use list style from list node - requires a stack, though ...
                listStyle = ('itemizedlist', 'level', str(self.listLevel))
                newList = self.cursor.createList(self.formatManager.getFormat(listStyle).getListFormat())
            for n in node.children:
                self.addNode(n)

        elif type(node) == List:
            self.listLevel += 1
            for n in node.children:
                self.addNode(n)
            self.listLevel -= 1

        elif type(node) is ImageFragment:
            imageFmt = QTextImageFormat()
            imagePath = os.path.join(self.contentPath, node.image)
            imageFmt.setName(imagePath)
            self.cursor.insertImage(imageFmt)
        elif type(node) is MathFragment:
            mathFormula = MathFormula()
            mathFormula.setFormula(node.text)
            mathFormula.image = node.image #  renderFormula()

            mathObjectFormat = QTextCharFormat()
            mathObjectFormat.setVerticalAlignment(QTextCharFormat.AlignNormal)
            mathObjectFormat.setObjectType(QTextFormat.UserObject + 1)
            mathObjectFormat.setProperty(QTextFormat.UserProperty + 1, mathFormula)
            self.cursor.insertText('\ufffc', mathObjectFormat);
        elif type(node) is TextFragment:
            text = node.text.replace('\n', '\u2028')
            if node.href is not None:
                fmt = self.formatManager.getFormat(('link', None, None))   # TODO!
                charFmt = fmt.getCharFormat()
                charFmt.setAnchorHref(node.href)
                self.cursor.insertText(text, charFmt)
            else:
                # "The block char format is the format used when inserting text at the beginning of an empty block.
                # Hence, the block char format is only useful for the first fragment -
                # once a fragment is inserted with a different style, and afterwards
                # another fragment is inserted with no specific style, we need to reset
                # the char format to the block's char format explicitly!
    
                if node.style is not None:
                    fmt = self.formatManager.getFormat(node.style)
                else:
                    fmt = self.paraFormat

                self.cursor.insertText(text, fmt.getCharFormat())
