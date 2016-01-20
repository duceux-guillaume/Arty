#!/usr/bin/env python3

import subprocess
import readline
import engine
import string_matching

Commands = {'ls': ('shell', 'ls --color=auto'),
            'll': ('shell', 'ls -la --color=auto'),
            'what is your name?': ('chatter', 'Arty')}

class PythonEngine:
    def execute(self, cmd):
        try:
            eval_res = eval(cmd)
            print(eval_res)
            return True
        except:
            return False

class ChatterEngine:
    def execute(self, cmd):
        print(cmd)
        return True

if __name__ == "__main__":

    python = PythonEngine()
    shell = engine.ShellEngine()
    chatter = ChatterEngine()

    print("Hello, How can I help you?")
    
    while 1:
        user_input = input(">" + shell.get_current_directory() + " ");
       
        # Command lookup
        try:
            if len(user_input) == 0:
                chatter.execute(user_input)
                continue
            engine, cmd = Commands[user_input]
            returncode = False
            if engine == 'shell':
                returncode = shell.execute(cmd) 
            elif engine == 'python':
                returncode = python.execute(cmd)
            elif engine == 'chatter':
                returncode = chatter.execute(cmd)
            if not returncode: # Try all
                if python.execute(user_input):
                    continue
                if shell.execute(user_input):
                    continue
                if chatter.execute("wtf?"):
                    continue
        except Exception as e:
            # No match, try every engine
            if python.execute(user_input):
                continue
            if shell.execute(user_input):
                continue
            if chatter.execute("wtf?"):
                continue

