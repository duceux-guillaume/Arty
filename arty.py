#!/usr/bin/env python3

import subprocess

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
            stdoutdata = subprocess.getoutput(user_input)
            print("R: " + stdoutdata)
        except Exception as e:
            print(e)
            print("E: system engine failed")

