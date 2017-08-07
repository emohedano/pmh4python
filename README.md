pmh4python
========================

>Copyright 2017-2018 Eduardo Mohedano

This is a python C extension wrapper for [peg-markdown-highlight] that provides syntax highlighting for the [Markdown] language.

[Markdown]: http://daringfireball.net/projects/markdown
[peg-markdown-highlight]: https://github.com/ali-rantakari/peg-markdown-highlight
[peg-markdown]: https://github.com/jgm/peg-markdown

## Install

```
pip install pmh4python
```

## Usage

```python

import pmh4python

result_types = pmh4python.parse_markdown(markdown_text)

```


where `result_types ` dict containing one or more of the following keys:

- LINK
- AUTO\_LINK\_URL
- AUTO\_LINK\_EMAIL
- IMAGE
- CODE
- HTML
- HTML_ENTITY
- EMPH
- STRONG
- LIST_BULLET
- LIST_ENUMERATOR
- COMMENT
- H1
- H2
- H3
- H4
- H5
- H6
- BLOCKQUOTE
- VERBATIM
- HTMLBLOCK
- HRULE
- REFERENCE
- NOTE
- STRIKE

And inside each key a list of dicts is

- start `int`. The position where the element starts
- end `int`. The position where the element ends


### A QTextEdit example using QT 5.9:

```python

from PyQt5.QtGui import QFont, QSyntaxHighlighter, QTextCharFormat

import pmh4python

class Highlighter(QSyntaxHighlighter):
    def __init__(self, parent=None):

        super(Highlighter, self).__init__(parent)

        self.highlightingRules = {}

        H1 = QTextCharFormat()
        H1.setFontPointSize(30)

        STRONG = QTextCharFormat()
        STRONG.setFontWeight(QFont.Bold)

        EMPH = QTextCharFormat()
        EMPH.setFontItalic(True)

        self.highlightingRules['H1'] = H1
        self.highlightingRules['STRONG'] = STRONG
        self.highlightingRules['EMPH'] = EMPH

    def highlightBlock(self, text):

        results = pmh4python.parse_markdown(text)

        for element_type, elements in results.items():

            if element_type in self.highlightingRules:

                element_styles = self.highlightingRules[element_type]

                for element in elements:

                    index = element['start']
                    length = element['end'] - index

                    self.setFormat(index, length, element_styles)


        self.setCurrentBlockState(0)
```