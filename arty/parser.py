#!/usr/bin/env python3

from memory import Memory
import features
import string_matching as sm

class Parser:

    def __init__(self):
        self.tokens = []
        self.tags = []
        self.features = [attr for attr in vars(features.Features) if not attr.startswith("__")]

    def tokenize(self, sentence):
        self.tokens = sentence.split(' ')
        for i in range(len(self.tokens)):
            best = 1000
            argbest = None
            # Look for aliases
            for key, val in Memory.aliases.items():
                dist = sm.levenshtein(self.tokens[i], key)
                if dist <= best:
                    best = dist
                    argbest = key
            if best <= 1 and argbest != None:
                self.tokens[i] = Memory.aliases[argbest]
        return self.tokens

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
        Memory.load()
        parser = Parser()
        import sys
        if len(sys.argv) >= 2:
            parser.parse(' '.join(sys.argv[1:]))
            print(parser.tokens)
            print(parser.tags)
    except Exception as e:
        print(e)