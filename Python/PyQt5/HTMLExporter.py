'''
Created on 29.04.2015

@author: afester
'''

import os, urllib
from xml.sax.saxutils import escape
from StylableTextEdit.StylableTextModel import List, Paragraph, TextFragment, ImageFragment, MathFragment, TextDocumentTraversal

class HtmlPrinter:

    def __init__(self, root, out, baseDir):
        self.rootFrame = root
        self.indent = 0
        self.out = out
        self.listLevel = 0
        self.baseDir= baseDir
        self.isPre = False

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
#===============================================================================
#         if type(node) == Fragment:
#             self.emitFragment(node)
# 
#         el
#===============================================================================
        if type(node) == Paragraph:
            if node.style[0] == 'programlisting':
                self.out('\n{}<pre class="code"><code class="{}">'.format(self.prefix(), node.style[2]))
                self.isPre = True
            elif node.style[0] == 'screen':
                self.out('\n{}<pre>'.format(self.prefix()))
                self.isPre = True

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

            self.isPre = False
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
            import tempfile, uuid
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
            if not self.isPre:
                text = text.replace('\n', '<br/>')

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



class HTMLExporter:
    
    def __init__(self, baseDir):
        self.baseDir = baseDir


    def getHtmlString(self, document):
        self.contents = ''

        traversal = TextDocumentTraversal()
        documentModel = traversal.traverse(document)

        hp = HtmlPrinter(documentModel, self.collect, self.baseDir)
        hp.traverse()

        return self.contents


    def collect(self, data):
        if data is not None:
            self.contents += data
