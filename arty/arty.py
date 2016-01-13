#!/usr/bin/env python3

import subprocess
import readline

Commands = {'git log': ('shell', 'git log | more'),
            'what is your name?': ('chatter', 'Arty')}

class PythonEngine:
    def execute(self, cmd):
        try:
            eval_res = eval(cmd)
            print(eval_res)
            return True
        except:
            return False

class ShellEngine:
    def execute(self, cmd):
        try:
            with subprocess.Popen(cmd, stdout=subprocess.PIPE, bufsize=1, 
                                  universal_newlines=True, shell=True) as child:
                for line in child.stdout:
                    print(line, end='')
                #output = child.communicate()
                #rc = child.returncode
            return True
        except:
            return False

class ChatterEngine:
    def execute(self, cmd):
        print(cmd)
        return True

if __name__ == "__main__":

    python = PythonEngine()
    shell = ShellEngine()
    chatter = ChatterEngine()

    print("Hello, How can I help you?")
    
    while 1:
        user_input = input();
       
        # Command lookup
        try:
            engine, cmd = Commands[user_input]
            if engine == 'shell':
                shell.execute(cmd) 
            elif engine == 'python':
                python.execute(cmd)
            elif engine == 'chatter':
                chatter.execute(cmd)
        except Exception as e:
            # No match, try every engine
            if python.execute(user_input):
                continue
            if shell.execute(user_input):
                continue
            if chatter.execute("wtf?"):
                continue

