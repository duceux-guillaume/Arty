#!/usr/bin/env python3

if __name__ == "__main__":
    print("Hello, How can I help you?")
    while 1:
        user_input = input();
        print("U: " + user_input)
        # Handle python stuff
        try:
            eval_res = eval(user_input)
            print("R: " + str(eval_res))
        except:
            print("R: not python")
            pass
