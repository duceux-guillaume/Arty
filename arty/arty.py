#!/usr/bin/env python3

import subprocess
import readline
import string_matching
import os
import sys
import cmd
import atexit

'A class which contains all persistant information called context'
class Context:
    path = os.getcwd()
    path_history = [path]
    cmds = {'ls': 'ls --color=auto',
            'll': 'ls -la --color=auto'}
    history_file = os.path.expanduser("~/.arty/user_history.log")

class ArtyShell(cmd.Cmd):
    # ----- Cmd variables ---------
    intro = 'Welcome to Arty shell.   Type help or ? to list commands.\n'
    prompt = os.path.basename(Context.path) + ": "

    # ----- built-in commands -----    
    def do_cd(self, arg):
        'Change current directory:  cd [rpath, path, empty, ~/path, foldername]'
        arg.rstrip()
        # going back
        if arg == '-':
            Context.path = Context.path_history[-1]
            return
        # absolute path
        if len(arg) > 0 and arg[0] == '/':
            try:
                subp = subprocess.Popen('pwd', cwd=arg, 
                                        stdout=subprocess.PIPE)
                Context.path_history.append(Context.path)
                Context.path = subp.communicate()[0].decode().rstrip()
                return
            except Exception as e:
                print(e)
                return
        # relative path
        if len(arg) > 0:
            try:
                subp = subprocess.Popen('pwd', cwd=Context.path + "/" + arg, 
                                        stdout=subprocess.PIPE)
                Context.path_history.append(Context.path)
                Context.path = subp.communicate()[0].decode().rstrip()
                return
            except Exception as e:
                print(e)
                return
        # going home
        if len(arg) == 0:
            Context.path_history.append(Context.path)
            Context.path = os.path.expanduser('~')
            return
        #TODO: user path: ~/path
        #TODO: folder lookup to avoid typing the whole path and jump around

    def complete_cd(self, text, line, begidx, endidx):
        try:
            ll = line.rstrip().split(' ')
            cd = ''
            if len(ll) > 1:
                pos = ll[1].rfind('/')
                if pos >= 0:
                    cd = ll[1][:pos]
            return [path+'/' for path in os.listdir(Context.path + "/" + cd) 
                    if path.startswith(text) and os.path.isdir(Context.path + "/" + cd + "/" + path)]
        except Exception as e:
            return []

    def do_display(self, arg):
        'display some context information:  display ctx_attr'
        #TODO use arg to display some context information
        list_of_var = [attr for attr in vars(Context) if not attr.startswith("__")]
        for attr in list_of_var:
            if attr == arg: 
                print(getattr(Context, attr))
                return

    def complete_display(self, text, line, begidx, endidx):
        return [attr for attr in vars(Context) if attr.startswith(text) and not attr.startswith("__")]

    def do_execute(self, arg):
        'execute cmd line without parsing:  execute cmd'
        # calling bash
        try:
            with subprocess.Popen(["/bin/bash", "-c", arg], 
                                  cwd=Context.path) as child:
                child.wait()
                return
        except Exception as e:
            print(e)
            return

    # ----- hooks -----
    def postcmd(self, stop, line):
        ArtyShell.prompt = os.path.basename(Context.path) + ": "
        return stop

    # ------ magic stuff ----
    def default(self, arg):
        # Command lookup
        try:
            cmd = Context.cmds[arg]
            self.do_execute(cmd) 
            return
        except KeyError as key:
            pass
        except Exception as e:
            print(e)
        # if cmd not found, try the arg instead
        try:
            self.do_execute(arg) 
            return
        except Exception as e:
            print(e)


def save_history(history_file=Context.history_file):
    import readline
    readline.write_history_file(history_file)

if __name__ == '__main__':
    if os.path.exists(Context.history_file):
        readline.read_history_file(Context.history_file)
    atexit.register(save_history)
    ArtyShell().cmdloop()

