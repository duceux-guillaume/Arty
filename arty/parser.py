#!/usr/bin/env python3

from memory import *
import features

class Parser:

    def __init__(self):
        self.tokens = []
        self.tags = []
        self.features = [attr for attr in vars(features.Features) if not attr.startswith("__")]

    def tokenize(self, sentence):
        self.tokens = []
        return sentence.split(' ')

    def parse(self, sentence):
        self.tokens = self.tokenize(sentence)
        for token in self.tokens:
            found = False
            for feature in self.features:
                func = getattr(features.Features, feature)
                if func.match(token):
                    self.tags.append(feature)
                    found = True
                    break
            if not found:
                self.tags.append('None')

if __name__ == "__main__":
    try:
        parser = Parser()
        import sys
        if len(sys.argv) >= 2:
            parser.parse(' '.join(sys.argv[1:]))
            print(parser.tokens)
            print(parser.tags)
    except Exception as e:
        print(e)