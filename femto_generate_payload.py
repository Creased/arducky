#!/usr/bin/env python3
# -*- coding:Utf-8 -*-

"""Generates payload in arducky_femto.ino from a Rubber Ducky payload file."""

import re
import argparse

__author__ = 'Aurélien JANVIER <contact@ajanvier.fr>'
__license__ = 'MIT'

# Payload Regex
PAYLOAD_REGEX = r'(char\s\*payload\[\]\s=\s{)(.*)(};)'

def format_payload(payload):
    """Formats payload for usage in arduino code."""
    formatted_payload = '\n'
    for line in payload.split('\n'):
        formatted_line = '\t\t"' + line + '",\n'
        formatted_line = formatted_line.replace('\\"', '\\\\\\"')  # Escape the f*cking backslash
        formatted_payload += formatted_line

    formatted_payload += '\t\tNULL\n'
    return formatted_payload

def parse_args():
    """Arguments parsing."""
    parser = argparse.ArgumentParser(
        description='Generates payload in arducky_femto.ino '
                    'from an input Rubber Ducky payload file'
    )

    parser.add_argument('payload_file',
                        type=str,
                        help='input Rubber Ducky payload file')

    parser.add_argument('-i', '--ino',
                        type=str,
                        default='arducky_femto.ino',
                        metavar='ino_file',
                        dest='ino_file',
                        help='input Arducky ino file')

    parser.add_argument('-o', '--out',
                        type=str,
                        default='arducky_femto_with_payload.ino',
                        metavar='output_file',
                        dest='output_file',
                        help='output Arducky ino file')

    args = parser.parse_args()

    return args

def main():
    """Main function."""
    try:
        # Arguments parsing
        args = parse_args()

        code = ''
        payload = ''
        code_with_payload = ''

        with open(args.ino_file, 'r') as ino_file:
            code = ino_file.read()

        with open(args.payload_file, 'r') as payload_file:
            payload = payload_file.read()

        code_with_payload = re.sub(PAYLOAD_REGEX,
                                   r'\g<1>' + format_payload(payload) + r'\t\g<3>',
                                   code,
                                   flags=re.DOTALL)

        with open(args.output_file, 'w') as output_file:
            output_file.write(code_with_payload)
    except IOError as exception_:
        print('No such file or directory: {}'.format(exception_.filename))
    except (ValueError, TypeError) as exception_:
        print('An error occured: {}'.format(exception_))

# Runtime processor
if __name__ == '__main__':
    main()
