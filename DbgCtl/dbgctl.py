import argparse
import sys
import os

from debug_exception_handler import debug_exception_handler_path, debug_exception_handler_header, \
    debug_exception_handler_cpp
from logger_message_handler import logger_message_handler, logger_message_handler_path

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    # add argument with string value
    parser.add_argument('-c', '--create', type=str, action='store', help='Create new module with the given name',
                        metavar='NAME', dest='name')
    parser.add_argument('-t', '--type', help='Type of the module',
                        choices=['debug_exception_handler', 'logger_message_handler'])
    args = parser.parse_args(sys.argv[1:])

    if args.type == 'debug_exception_handler':
        os.mkdir(f'../Debug Event Handlers/{args.name}')
        with open(f'{debug_exception_handler_path}/{args.name}/{args.name}.hpp', 'w') as f:
            f.write(debug_exception_handler_header.format(name=args.name, inc_name=args.name.upper()))
        with open(f'{debug_exception_handler_path}/{args.name}/{args.name}.cpp', 'w') as f:
            f.write(debug_exception_handler_cpp.format(name=args.name, inc_name=args.name.upper()))
    elif args.type == 'logger_message_handler':
        with open(f'{logger_message_handler_path}/{args.name}.hpp', 'w') as f:
            f.write(logger_message_handler.format(name=args.name, inc_name=args.name.upper()))
