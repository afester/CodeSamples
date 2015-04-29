'''
Created on 19.02.2015

@author: afester
'''

import os, urllib
from xml.sax.saxutils import escape
from StylableTextEdit.StylableTextModel import TextDocumentTraversal, List, Paragraph, TextFragment, ImageFragment, MathFragment


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
#===============================================================================
#         if type(node) == Fragment:
#             self.emitFragment(node)
# 
#         el
#===============================================================================
        if type(node) == Paragraph:
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
            

class XMLExporter:
    def __init__(self, contentPath, contentFile):
        self.contentPath = contentPath
        self.contentFile = contentFile


    def exportDocument(self, document):
        contentFilePath = os.path.join(self.contentPath, self.contentFile)
        with open(contentFilePath, 'w', encoding='utf-8') as content_file:
            content_file.write(self.getXmlString(document))


    def getXmlString(self, document):
        self.contents = ''

        traversal = TextDocumentTraversal()
        documentModel = traversal.traverse(document)

        dp = DocbookPrinter(documentModel, self.collect)
        dp.traverse()

        return self.contents


    def collect(self, data):
        if data is not None:
            self.contents += data
