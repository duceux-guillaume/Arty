#!/usr/bin/env python3

import re

#Path features
AbsPath = re.compile(r'(^/.*)+')
RelPath = re.compile(r'(.+/.*)+')

class Parser:

    def __init__(self):
        self.tokens = []
        self.tags = []

    def tokenize(self, sentence):
        self.tokens = []
        return sentence.split(' ')

    def parse(self, sentence):
        self.tokens = self.tokenize(sentence)
        for token in self.tokens:
            if RelPath.match(token):
                self.tags.append("rpath")                
            elif AbsPath.match(token):
                self.tags.append("apath")
            else:
                self.tags.append("None")

if __name__ == "__main__":
    parser = Parser()
    import sys
    if len(sys.argv) >= 2:
        parser.parse(' '.join(sys.argv[1:]))