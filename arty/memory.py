import os
import readline
import pickle

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
        import readline
        try:
            if os.path.exists(kls.history_file):
              readline.read_history_file(kls.history_file)
            tmp_file = open(kls.cmds_file, 'rb')
            kls.cmds |= pickle.load(tmp_file)
            tmp_file = open(kls.scope_file, 'rb')
            kls.scope = pickle.load(tmp_file)
            tmp_file = open(kls.aliases_file, 'rb')
            kls.aliases.update(pickle.load(tmp_file))
            tmp_file = open(kls.path_file, 'rb')
            kls.path_history |= pickle.load(tmp_file)
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
        import readline
        readline.write_history_file(kls.history_file)
        tmp_file = open(kls.cmds_file, 'wb')
        pickle.dump(kls.cmds, tmp_file)
        tmp_file = open(kls.scope_file, 'wb')
        pickle.dump(kls.scope, tmp_file)
        tmp_file = open(kls.aliases_file, 'wb')
        pickle.dump(kls.aliases, tmp_file)
        tmp_file = open(kls.path_file, 'wb')
        pickle.dump(kls.path_history, tmp_file)
