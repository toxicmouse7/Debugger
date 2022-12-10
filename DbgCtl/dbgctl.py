import argparse
import os
import sys

if __name__ == '__main__':
    

    parser = argparse.ArgumentParser()
    # add argument with string value
    parser.add_argument('-c', '--create', type=str, action='store', help='Create new module with the given name',
                        metavar='NAME')
    parser.add_argument('-t', '--type', help='Type of the module',
                        choices=['debug_event_handler', 'logger_message_handler'])
    args = parser.parse_args(sys.argv[1:])
    print(args)


# os.chdir(r'..')
