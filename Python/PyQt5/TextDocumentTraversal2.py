'''
Created on 10.03.2015

@author: afester
'''

from PyQt5.Qt import QTextTable, QTextFormat
from xml.sax.saxutils import escape
import tools
import urllib


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


# A Paragraph is a container for Fragments and paragraphs on a more indented level
class Paragraph(Node):

    def __init__(self, indentLevel, style):
        Node.__init__(self)

        self.indent = indentLevel
        self.style = style

    def __str__(self):
        return 'Paragraph[style={}, indent={}]'.format(self.style, self.indent)


class List(Node):

    def __init__(self):
        Node.__init__(self)


    def __str__(self):
        return 'List';

#===============================================================================
# # A list of Paragraphs and sub Lists
# class List:
#     def __init__(self, level):
#         self.children = []
#         self.level = level
# 
#     def add(self, child):
#         self.children.append(child)
# 
#     def __str__(self):
#         return 'List[level={}]'.format(self.level)
# 
#     def __repr__(self):
#         return self.__str__()
#===============================================================================


class Table:
    pass


# Fragments do not have children
class Fragment:

    def __init__(self, style):
        self.href = None
        self.images = []
        self.text = None
        self.style = style


    def addImage(self, img):
        self.images.append(img)


    def setText(self, text):
        self.text = text

    def setHref(self, text):
        self.href = text

    def __str__(self):
        return 'Fragment[style={}, text="{}", images={}, href={}]'.format(self.style, self.text, self.images, self.href)



class TextDocumentTraversal2:

    def __init__(self):
        pass

    def traverse(self, document):
        result = Frame()    # root frame
        self.nodeStack = [result]

        frm = document.rootFrame()
        self.dumpFrame(frm)

        print("RESULT:{}".format(result)) # [0]))
        return result   # None # self.nodeStack[0]


    def dumpFrame(self, frm):

        paraStack = []

        frame1 = self.nodeStack[-1]
        currentIndent = 0       # current indent within this frame
        if type(frm) is QTextTable:
            tbl = self.dumpTable(frm)
        else:
            iterator = frm.begin()
            while not iterator.atEnd():
                frame = iterator.currentFrame() # None if not a frame
                block = iterator.currentBlock() # Invalid block if its a frame

                if frame:
                    self.dumpFrame(frame)
                else:

                    # A block is not further structured (from a QTextDocument's point of view).
                    # We can get the whole block here and handle the inherent structure below.
                    para = self.getParagraph(block)
                    listLevel = para.indent

                    # Normal block, add as child to frame
                    if listLevel == 0:
                        frame1.add(para)
                        paraStack = []
                    else:

###############################################################################
                        if len(paraStack) == 0: # listLevel == 1:
                            list = List()
                            frame1.add(list)
                            paraStack = [frame1, list]
    
                        # Add additional artificial paragraphs if required
                        for x in range(currentIndent+1, listLevel):
                            #l = Paragraph(x, ('ARTIFICIAL'))
                            l = List()
                            print("    {}".format(l))

                            parent = paraStack[x-1]
                            parent.add(l)
                            parent = l
                            paraStack.append(parent)   # push

                        parent = paraStack[-1] # paraStack[listLevel-1]

                        parent.add(para)
                        if len(paraStack) <= listLevel:
                            paraStack.append(para)
                        else:
                            paraStack[listLevel] = para

                    currentIndent = listLevel
###############################################################################

                iterator += 1


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
            frag = self.getFragment(fragment)
            result.add(frag)
            iterator += 1

        return result


    def getFragment(self, fragment):
        text = fragment.text().replace('\u2028', '\n')

        charFormat = fragment.charFormat()
        style = charFormat.property(QTextFormat.UserProperty)

        result = Fragment(style)
        if style and style[0] == 'link':
            href = escape(charFormat.anchorHref())
            result.setHref(href)
            result.style = None     # Link implicitly defined by setting href

        isObject = (text.find('\ufffc') != -1)
        isImage = isObject and charFormat.isImageFormat()

        if isImage:
            imgFmt = charFormat.toImageFormat()
            imgName = tools.os_path_split(imgFmt.name())[-1]

            # If the same image repeats multiple times, it is simply represented by
            # yet another object replacement character ...
            for img in range(0, len(text)):
                result.addImage(imgName)
        else:
            text = escape(text)
            result.setText(text)

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
        if type(parent) == Paragraph or type(parent) == Frame:
            for c in parent.children:
                self.indent += 1
                self.recursiveTraverse(c)
                self.indent -= 1



class HtmlPrinter:

    def __init__(self, root, out):
        self.rootFrame = root
        self.indent = 0
        self.out = out
        self.listIndent = 0

    def prefix(self):
        return '\u00a0' * self.indent * 2

    def traverse(self):
        self.out('''<!DOCTYPE html>
<html lang="en">
  <head></head>
  <body>''')

        self.indent = 2
        self.visitFrame(self.rootFrame)
#        for c in self.rootFrame.children:
#           self.recursiveTraverse(c)

#        for i in range(0, self.listIndent):
#            self.indent -= 1
#            self.out("\n{}</ul>".format(self.prefix()))

        self.out('''

  </body>
</html>''')


    def visitFrame(self, frame):
        for c in frame.children:
            if type(c) == Frame:
                self.visitFrame(c)
            elif type(c) == Paragraph:
                self.visitParagraph(c)


    def visitParagraph(self, paragraph):

        if paragraph.indent == 0:
            self.out("<p>")
        else:
            self.out("<ul>")

        for c in paragraph.children:
            if type(c) == Fragment:
                self.emitFragment(c)
            elif type(c) == Paragraph:
                self.visitParagraph(c)

        if paragraph.indent == 0:
            self.out("</p>")
        else:
            self.out("</ul>")



    def recursiveTraverse(self, node):
        if type(node) == Fragment:
            self.emitFragment(node)
        elif type(node) == Paragraph:

            for i in range(node.indent, self.listIndent):
                self.listIndent -= 1
                self.indent -= 1
                self.out("\n{}</ul>".format(self.prefix()))

            if node.indent == 0:
                if node.style[0] == 'programlisting':
                    self.out('\n{}<pre class="code"><code class="{}">'.format(self.prefix(), node.style[2]))
                elif node.style[0] == 'screen':
                    self.out('\n{}<pre>'.format(self.prefix()))
                elif node.style[0] == 'title':
                    self.out('\n\n{}<h{}>'.format(self.prefix(), int(node.style[2])))
                elif node.style[0] == 'tip':
                    self.out('\n{}<table class="tip"><tr><td><img src="icons/tip.png" /></td><td>'.format(self.prefix()))
                elif node.style[0] == 'para':
                    self.out("\n{}<p>".format(self.prefix()))
                else:
                    raise RuntimeError('Unexpected format: {}'.format(node.style))
            else:
                for i in range(self.listIndent, node.indent):
                    self.listIndent += 1
                    self.out("\n{}<ul>".format(self.prefix()))
                    self.indent += 1
                self.out("\n{}<li>".format(self.prefix()))

            self.indent += 1
            for c in node.children:
                self.recursiveTraverse(c)
            self.indent -= 1

            if node.indent == 0:
                if node.style[0] == 'programlisting':
                    self.out('</code></pre>')
                elif node.style[0] == 'screen':
                    self.out('</pre>')
                elif node.style[0] == 'title':
                    self.out('</h{}>'.format(int(node.style[2])))
                elif node.style[0] == 'tip':
                    self.out('</td></tr></table>')
                elif node.style[0] == 'para':
                    self.out("</p>")
                else:
                    raise RuntimeError('Unexpected format: {}'.format(node.style))
            else:
                self.out("</li>")
        else:
            raise RuntimeError('Unsupported structure: {} can not be a child'.format(node))


    def emitFragment(self, fragment):
        if fragment.href is not None:
            self.out('<a href="{}">'.format(fragment.href))

        if fragment.style is None or fragment.style[0] is None:
            self.out(fragment.text)
        elif fragment.style[0] == 'olink':
            self.out('<i>{}</i>'.format(fragment.text))
        elif fragment.style[0] == 'code':
            self.out('<tt>{}</tt>'.format(fragment.text))
        elif fragment.style[0] == 'emphasis':
            if fragment.style[1] == 'role' and fragment.style[2] == 'highlight':
                self.out('<em>{}</em>'.format(fragment.text))
            else:
                self.out('<strong>{}</strong>'.format(fragment.text))
        else:
            raise RuntimeError("Unsupported style {}".format(fragment.style))

        if fragment.href is not None:
            self.out('</a>')


class DocbookPrinter:

    def __init__(self, root, out):
        self.rootFrame = root
        self.indent = 0
        self.out = out
        self.listIndent = 0
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

#===============================================================================
# 
#         self.indent = 1
#         for c in self.rootFrame.children:
#             self.recursiveTraverse(c)
# 
#         if self.listIndent > 0:
#             self.indent -= 1
#             self.out("\n{}</listitem>".format(self.prefix()))
#             for i in range(0, self.listIndent):
#                 self.indent -= 1
#                 self.out("\n{}</itemizedlist>".format(self.prefix()))
# 
#         for x in range(0, self.sectionLevel):
#             self.indent -= 1
#             self.out('\n{}</section>'.format(self.prefix()))
#===============================================================================

        self.out('\n</article>')


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
            else:
                self.out('\n{}<{}>'.format(self.prefix(), node.style[0]))

            self.indent += 1
            for c in node.children:
                self.recursiveTraverse(c)
            self.indent -= 1

            if node.style[0] == 'tip':
                self.out('</para></tip>'.format())
            else:
                self.out('</{}>'.format(node.style[0]))

        elif type(node) == None: # List:
            self.out('\n\n{}<itemizedlist>'.format(self.prefix()))

            self.indent += 1
            for c in node.children:
                if type(c) == Paragraph:
                    self.out('\n{}<listitem>'.format(self.prefix()))

                self.visitNode(c)

                if type(c) == Paragraph:
                    self.out('\n{}</listitem>'.format(self.prefix()))
            self.indent -= 1

            self.out('\n{}</itemizedlist>'.format(self.prefix()))


    def recursiveTraverse(self, node):

        if type(node) == Fragment:
            self.emitFragment(node)

        elif type(node) == Paragraph:

            # if we are currently in a list, then close previous list item
            # if node.indent <= 
#===============================================================================
#             if self.listIndent > 0: #  and node.indent <= self.listIndent:
#                 self.indent -= 1
#                 self.out('\n{}</listitem>'.format(self.prefix()))
# 
#             # if the level goes back, then close any previous lists
#             for x in range(node.indent, self.listIndent):
#                 self.indent -= 1
#                 self.out('\n{}</itemizedlist>'.format(self.prefix()))
# #                self.indent -= 1
# #                self.out('\n{}</listitem>'.format(self.prefix()))  # TODO: This is NOT always correct.
#===============================================================================
                                                                        # If a new list starts with a level > 1, 
                                                                        # there will be NO corresponding <listitem>!
            #===================================================================
            # if self.listIndent > 0: #  and node.indent <= self.listIndent:
            #     # if the level goes up, then open any number of required lists
            #     for x in range(node.indent, self.listIndent):
            #         self.out('\n{}</itemizedlist>'.format(self.prefix()))
            #         self.indent -= 1
            #===================================================================
#===============================================================================
# 
#             if node.indent < self.listIndent:
#                 self.indent -= 1
#                 self.out('\n{}</listitem>'.format(self.prefix()))
# 
#                 for x in range(node.indent, self.listIndent):
#                     self.indent -= 1
#                     self.out('\n{}</itemizedlist>'.format(self.prefix()))
# 
#             # Open a new list item?                                                        
#             if node.indent > 0:
# 
#                 # if the level goes up, then open any number of required lists
#                 for x in range(self.listIndent, node.indent):
#                     self.out('\n{}<itemizedlist>'.format(self.prefix()))
#                     self.indent += 1
# 
#                 self.out('\n{}<listitem>'.format(self.prefix()))
#                 self.indent += 1
#             self.listIndent = node.indent
#===============================================================================

            if node.indent > self.listIndent:
                self.out(" XXX")
                self.listIndent +=1
                self.recursiveTraverse(node)
                self.listIndent -=1
                self.out(" YYY")
            else:
    
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
                else:
                    self.out('\n{}<{}>'.format(self.prefix(), node.style[0]))
    
                self.indent += 1
                for c in node.children:
                    self.recursiveTraverse(c)
                self.indent -= 1
    
                if node.style[0] == 'tip':
                    self.out('</para></tip>'.format())
                else:
                    self.out('</{}>'.format(node.style[0]))
                # self.out('\n{}</para></listitem>'.format(self.prefix())) # Need to leave listitem open since the next level might be indented

        else:
            raise RuntimeError('Unsupported structure: {} can not be a child'.format(node))


    def emitFragment(self, fragment):
        if fragment.style is None:
            self.out(fragment.text)
        elif fragment.style[0] == 'olink':
            linkend = urllib.parse.quote(fragment.text, '')
            self.out('<olink targetdoc="{}">{}</olink>'.format(linkend, fragment.text))
        elif fragment.style[0] == 'emphasis':
            if fragment.style[2] is not None and fragment.style[2] == 'highlight':
                self.out('<emphasis role="highlight">{}</emphasis>'.format(fragment.text))
            else:
                self.out('<emphasis>{}</emphasis>'.format(fragment.text))
        else:
            self.out('<{}>{}</{}>'.format(fragment.style[0], fragment.text, fragment.style[0]))
