#!/usr/bin/env python3

#https://github.com/ystk/debian-python3.1/blob/master/Lib/cmd.py

import subprocess
import readline
import string_matching
import os
import sys
import cmd
from parser import *
from memory import *

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
            Context.path, Context.last_path = Context.last_path, Context.path
            return
        # absolute path
        if len(arg) > 0 and arg[0] == '/':
            try:
                subp = subprocess.Popen('pwd', cwd=arg, 
                                        stdout=subprocess.PIPE)
                Context.path = subp.communicate()[0].decode().rstrip()
                Memory.path_history.add(Context.path)
                return
            except Exception as e:
                print(e)
                return
        # user path
        if len(arg) > 0 and arg[0] == '~':
            try:
                subp = subprocess.Popen('pwd', cwd=os.path.expanduser(arg), 
                                        stdout=subprocess.PIPE)
                Context.path = subp.communicate()[0].decode().rstrip()
                Memory.path_history.add(Context.path)
                return
            except Exception as e:
                print(e)
                return

        # relative path
        if len(arg) > 0:
            try:
                subp = subprocess.Popen('pwd', cwd=Context.path + "/" + arg, 
                                        stdout=subprocess.PIPE)
                Context.path = subp.communicate()[0].decode().rstrip()
                Memory.path_history.add(Context.path)
                return
            except Exception as e:
                #folder lookup to avoid typing the whole path and jump around
                try:
                    for candidate in Context.path_history:
                        if os.path.basename(candidate) == arg:
                            subp = subprocess.Popen('pwd', cwd=candidate, 
                                                    stdout=subprocess.PIPE)
                            Context.path = subp.communicate()[0].decode().rstrip()
                            Memory.path_history.add(Context.path)
                            return
                    # here we got nothing
                    print(e)
                    return
                except Exception as e:
                    print(e)
                    return
        # going home
        if len(arg) == 0:
            Context.path = os.path.expanduser('~')
            Memory.path_history.add(Context.path)
            return
        
    def complete_cd(self, text, line, begidx, endidx):
        try:
            ll = [x.strip() for x in line.rstrip().split(' ')]
            cd = ''
            type = 0
            if len(ll) > 1:
                pos = ll[1].rfind('/')
                if pos >= 0:
                    cd = ll[1][:pos+1]
                if len(cd) > 0 and cd[0] == '/':
                    type = 1 # absolute
                if len(cd) > 0 and cd[0] == '~':
                    type = 2 # user
            if type == 1:
                return [path+'/' for path in os.listdir(cd) 
                        if path.startswith(text) and os.path.isdir(cd + path)]
            if type == 2:
                return [path +'/' for path in os.listdir(os.path.expanduser(cd)) 
                        if path.startswith(text) and os.path.isdir(os.path.expanduser(cd) + path)]
            else:
                local = []
                lookup = []
                if os.path.isdir(Context.path + "/" + cd):
                    local = [path+'/' for path in os.listdir(Context.path + "/" + cd) 
                            if path.startswith(text) and os.path.isdir(Context.path + "/" + cd + path)]
                if len(text) > 0:
                        lookup = [os.path.basename(path) for path in Memory.path_history
                            if os.path.basename(path).startswith(text)]
                return local + lookup
        except Exception as e:
            print(e)
            return []

    def do_display(self, arg):
        'display some context information:  display ctx_attr'
        list_of_ctx = [attr for attr in vars(Context) if not attr.startswith("__")]
        for attr in list_of_ctx:
            if attr == arg: 
                print(getattr(Context, attr))
                return
        list_of_mem = [attr for attr in vars(Memory) if not attr.startswith("__")]
        for attr in list_of_mem:
            if attr == arg: 
                print(getattr(Memory, attr))
                return
        list_of_con = [attr for attr in vars(Config) if not attr.startswith("__")]
        for attr in list_of_con:
            if attr == arg: 
                print(getattr(Config, attr))
                return

    def complete_display(self, text, line, begidx, endidx):
        return ([attr for attr in vars(Context) if attr.startswith(text) and not attr.startswith("__")] +
            [attr for attr in vars(Memory) if attr.startswith(text) and not attr.startswith("__")] +
            [attr for attr in vars(Config) if attr.startswith(text) and not attr.startswith("__")])

    def do_self(self, arg):
        parser = Parser()
        parser.parse(arg)
        if len(parser.tokens) <= 4:
            return False
        if parser.tokens[0] != "alias":
            return False
        alias =''
        id = 1
        while parser.tokens[id] != "is" and id < len(parser.tokens):
            alias += " " + parser.tokens[id]
            id += 1
        alias = alias.lstrip()
        if parser.tokens[id] != "is":
            return False
        id += 1
        cmd =''
        while id < len(parser.tokens):
            cmd += " " + parser.tokens[id]
            id += 1
        cmd = cmd.lstrip()
        Memory.aliases[alias] = cmd
        return True

    def do_shell(self, arg):
        'execute cmd line without parsing:  execute cmd'
        # calling bash
        try:
            with subprocess.Popen(["/bin/bash", "-c", arg], 
                                  cwd=Context.path) as child:
                child.wait()
                return child.returncode == 0
        except KeyboardInterrupt:
            return False
        except Exception as e:
            return False
            
    def do_python(self, arg):
        'compute expression without parsing:  compute expr'
        try:
            exec("tmp =" + arg, Memory.scope)
            print(arg, "=", Memory.scope['tmp'])
            return True
        except Exception as e:
            pass
            
        try:
            exec(arg, Memory.scope)
            return True
        except Exception as e:
            pass
        return False

    # ----- hooks -----
    def postcmd(self, stop, line):
        ArtyShell.prompt = os.path.basename(Context.path) + ": "
        return stop
        
    def emptyline(self):
        pass

    # ------ magic stuff ----
    def default(self, arg):
        parser = Parser()
        parser.parse(arg)
        print(parser.tokens)
        print(parser.tags)
        # Command lookup
        try:
            cmd = Memory.aliases[arg]
            self.do_shell(cmd) 
            return
        except KeyError as key:
            pass
        except Exception as e:
            print(e)
        # if cmd not found, try the arg instead
        try:
            if self.do_self(arg):
                return
            if self.do_python(arg):
                return
            if self.do_shell(arg): 
                Memory.cmds.add(arg)
                return
        except Exception as e:
            print(e)


    def completedefault(self, text, line, begidx, endidx):
        from_dotext = []
        from_aliases = []
        from_cmds = []
        dotext = 'do_'+line
        from_dotext = [a[3:] for a in self.get_names() if a.startswith(dotext)]
        if len(line.strip()) > 0:
            from_aliases = [key[begidx:] for key, val in Memory.aliases if key.startswith(line)]
            from_cmds = [cmd[begidx:] for cmd in Memory.cmds if cmd.startswith(line)]
        return from_aliases + from_dotext + from_cmds + self.complete_cd(text, line, begidx, endidx)

if __name__ == '__main__':
    import atexit
    Memory.load()
    atexit.register(Memory.save)
    ArtyShell().cmdloop()

