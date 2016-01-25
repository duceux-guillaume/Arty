#!/usr/bin/env python3

#https://github.com/ystk/debian-python3.1/blob/master/Lib/cmd.py

import subprocess
import readline
import string_matching
import os
import sys
import cmd
import pickle

'A class which contains all persistant information called context'
class Context:
    path = os.getcwd()
    path_history = [path]
    aliases = {'ls': 'ls --color=auto',
            'll': 'ls -la --color=auto'}
    cmds = []
    scope = {}
    # persistant memory
    history_file = os.path.expanduser("~/.arty/user_history.log")
    cmds_file = os.path.expanduser("~/.arty/cmds_history.p")
    scope_file = os.path.expanduser("~/.arty/scope_history.p")
    aliases_file = os.path.expanduser("~/.arty/aliases_history.p")
    path_file = os.path.expanduser("~/.arty/path_history.p")
    
    @classmethod
    def load(kls):
        import readline
        try:
            if os.path.exists(Context.history_file):
              readline.read_history_file(Context.history_file)
            tmp_file = open(Context.cmds_file, 'rb')
            Context.cmds = pickle.load(tmp_file)
            tmp_file = open(Context.scope_file, 'rb')
            Context.scope = pickle.load(tmp_file)
            tmp_file = open(Context.aliases_file, 'rb')
            Context.aliases = pickle.load(tmp_file)
            tmp_file = open(Context.path_file, 'rb')
            Context.path_history = pickle.load(tmp_file)
        except Exception as e:
            pass

    @classmethod
    def save(kls):
        folder = os.path.expanduser("~/.arty")
        if not os.path.exists(folder):
            os.mkdir(folder)
        import readline
        readline.write_history_file(Context.history_file)
        tmp_file = open(Context.cmds_file, 'wb')
        pickle.dump(Context.cmds, tmp_file)
        tmp_file = open(Context.scope_file, 'wb')
        pickle.dump(Context.scope, tmp_file)
        tmp_file = open(Context.aliases_file, 'wb')
        pickle.dump(Context.aliases, tmp_file)
        tmp_file = open(Context.path_file, 'wb')
        pickle.dump(Context.path_history, tmp_file)

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
        #TODO: user path: ~/path
        try:
            ll = [x.strip() for x in line.rstrip().split(' ')]
            cd = ''
            abs = False
            if len(ll) > 1:
                pos = ll[1].rfind('/')
                if pos >= 0:
                    cd = ll[1][:pos+1]
                if len(cd) > 0 and cd[0] == '/':
                    abs = True
            if abs:
                return [path+'/' for path in os.listdir(cd) 
                        if path.startswith(text) and os.path.isdir(cd + path)]
            else:
                return [path+'/' for path in os.listdir(Context.path + "/" + cd) 
                        if path.startswith(text) and os.path.isdir(Context.path + "/" + cd + path)]
        except Exception as e:
            print(e)
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

    def do_shell(self, arg):
        'execute cmd line without parsing:  execute cmd'
        # calling bash
        try:
            with subprocess.Popen(["/bin/bash", "-c", arg], 
                                  cwd=Context.path) as child:
                child.wait()
                return
        except KeyboardInterrupt:
            return
        except Exception as e:
            print(e)
            return
            
    def do_python(self, arg):
        'compute expression without parsing:  compute expr'
        try:
            exec("tmp =" + arg, Context.scope)
            print(arg, "=", Context.scope['tmp'])
            return
        except Exception as e:
            pass
            
        try:
            exec(arg, Context.scope)
        except Exception as e:
            print(e)

    # ----- hooks -----
    def postcmd(self, stop, line):
        ArtyShell.prompt = os.path.basename(Context.path) + ": "
        return stop
        
    def emptyline(self):
        pass

    # ------ magic stuff ----
    def default(self, arg):
        # Command lookup
        try:
            cmd = Context.aliases[arg]
            self.do_shell(cmd) 
            return
        except KeyError as key:
            pass
        except Exception as e:
            print(e)
        # if cmd not found, try the arg instead
        try:
            self.do_shell(arg) 
            Context.cmds.append(arg)
            return
        except Exception as e:
            print(e)


    def completedefault(self, text, line, begidx, endidx):
        from_dotext = []
        dotext = 'do_'+line
        from_dotext = [a[3:] for a in self.get_names() if a.startswith(dotext)]
        from_aliases = [key[begidx:] for key, val in Context.aliases if key.startswith(line)]
        from_cmds = [cmd[begidx:] for cmd in Context.cmds if cmd.startswith(line)]
        return from_aliases + from_dotext + from_cmds

if __name__ == '__main__':
    import atexit
    Context.load()
    atexit.register(Context.save)
    ArtyShell().cmdloop()

