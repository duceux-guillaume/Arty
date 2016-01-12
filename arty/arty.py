#!/usr/bin/env python3

import subprocess
import readline

if __name__ == "__main__":
    print("Hello, How can I help you?")
    while 1:
        user_input = input();
        print("U: " + user_input)
        
        try:
            print("Trying python engine")
            eval_res = eval(user_input)
            print("R: " + str(eval_res))
            continue
        except Exception as e:
            print(e)
            print("E: python engine failed")

        try:
            print("Trying system engine")
            with subprocess.Popen(user_input, stdout=subprocess.PIPE, bufsize=1, 
                                  universal_newlines=True, shell=True) as child:
                for line in child.stdout:
                    print(line, end='')
                output = child.communicate()
                rc = child.returncode
                if rc == 0:
                    continue
                else:
                    print("E: system engine failed")
        except Exception as e:
            print(e)
            print("E: system engine failed")

