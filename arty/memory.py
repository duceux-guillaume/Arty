import os
import readline
import pickle

__all__ = ["Config", "Context", "Memory"]


'A class wich contains Arty\'s configuration'
class Config:
    bash_prefix = "/bin/bash -c"

'A class which contains current context'
class Context:
    path = os.getcwd()
    last_path = path


'A class which contains all persistant information called memory'
class Memory:
    path_history = {Context.path}
    aliases = {'ls': 'ls --color=auto',
            'll': 'ls -la --color=auto'}
    cmds = set()
    scope = {}
    # persistant memory
    history_file = os.path.expanduser("~/.arty/user_history.log")
    cmds_file = os.path.expanduser("~/.arty/cmds_history.p")
    scope_file = os.path.expanduser("~/.arty/scope_history.p")
    aliases_file = os.path.expanduser("~/.arty/aliases_history.p")
    path_file = os.path.expanduser("~/.arty/path_history.p")
    
    @classmethod
    def load(kls):
        try:
#FIXME those file become too big, find another way to save history
#            import readline
#            if os.path.exists(Context.history_file):
#              readline.read_history_file(Context.history_file)
            with open(Context.cmds_file, 'rb') as tmp_file:
                Context.cmds |= pickle.load(tmp_file)
            with open(Context.scope_file, 'rb') as tmp_file:
                Context.scope = pickle.load(tmp_file)
            with open(Context.aliases_file, 'rb') as tmp_file:
                Context.aliases.update(pickle.load(tmp_file))
            with open(Context.path_file, 'rb') as tmp_file:
                Context.path_history |= pickle.load(tmp_file)
        except Exception as e:
            print(e)
            pass

    @classmethod
    def save(kls):
        # first load to avoid overwriting newer version
        kls.load()
        folder = os.path.expanduser("~/.arty")
        if not os.path.exists(folder):
            os.mkdir(folder)
        try:
#FIXME those file become too big, find another way to save history
#        import readline
#        readline.write_history_file(Context.history_file)
            with open(Context.cmds_file, 'wb') as tmp_file:
                pickle.dump(Context.cmds, tmp_file)
            with open(Context.scope_file, 'wb') as tmp_file:
                pickle.dump(Context.scope, tmp_file)
            with open(Context.aliases_file, 'wb') as tmp_file:
                pickle.dump(Context.aliases, tmp_file)
            with open(Context.path_file, 'wb') as tmp_file:
                pickle.dump(Context.path_history, tmp_file)
        except Exception as e:
            print(e)
            pass
