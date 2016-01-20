#!/usr/bin/env python3

import os
import sys
import re
import subprocess

class ShellEngine:
    def __init__(self):
        self.path = os.getcwd()
        self.history = [self.path]
         
    def execute(self, cmd):
        cmdlist = cmd.split(' ')
        # look for cd cmds
        if len(cmdlist) > 0 and cmdlist[0] == "cd":
            # going back
            if len(cmdlist) > 1 and cmdlist[1] == '--':
                self.path = self.history[-1]
                return True
            # absolute path
            if len(cmdlist) > 1 and len(cmdlist[1].rstrip()) > 0 and cmdlist[1][0] == '/':
                try:
                    subp = subprocess.Popen('pwd', cwd=cmdlist[1], 
                                            stdout=subprocess.PIPE)
                    self.history.append(self.path)
                    self.path = subp.communicate()[0].decode().rstrip()
                    return True
                except Exception as e:
                    print(e)
                    return False
            # relative path
            if len(cmdlist) > 1 and len(cmdlist[1].rstrip()) > 0:
                try:
                    subp = subprocess.Popen('pwd', cwd=self.path + "/" + cmdlist[1], 
                                            stdout=subprocess.PIPE)
                    self.history.append(self.path)
                    self.path = subp.communicate()[0].decode().rstrip()
                    return True
                except Exception as e:
                    print(e)
                    return False
            # going home
            self.history.append(self.path)
            self.path = os.path.expanduser('~')
            return True

        try:
            with subprocess.Popen(cmd.split(' '), 
                                  cwd=self.path) as child:
                child.wait()
                return child.returncode == 0
        except Exception as e:
            print(e)
            return False

    def get_current_directory(self):
        return os.path.basename(self.path)
		

if __name__ == "__main__":
    sys.argv.pop(0)
    cmd = ' '.join(sys.argv)
    engine = ShellEngine()
    engine.execute(cmd)
