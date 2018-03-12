#!/usr/bin/python3

""" Generates payload in arducky_femto.ino from a Rubber Ducky payload file """

__author__ = "Aurélien JANVIER"
__license__ = "MIT"

import re
import argparse
import json

# Payload Regex
PAYLOAD_REGEX = r"(char\s\*payload\[\]\s=\s{)(.*)(};)"

# Formats payload for usage in arduino code
def formatPayload(payload):
    formattedPayload = "\n"
    for line in payload.split("\n"):
        formattedLine = "\t\t\"" + line + "\",\n"
        formattedLine = formattedLine.replace("\\\"", "\\\\\\\"") # Escape the f*cking backslash
        formattedPayload += formattedLine

    formattedPayload += "\t\tNULL\n"
    return formattedPayload

def __main__():
    # Parses the arguments
    argumentParser = argparse.ArgumentParser(description='Generates payload in arducky_femto.ino from an input Rubber Ducky payload file')
    argumentParser.add_argument('payload_file', type=str, help='Input Rubber Ducky payload file')
    argumentParser.add_argument('--ino', type=str, help='Input arducky ino file', metavar='ino_file', dest='ino_file', default='arducky_femto.ino')
    argumentParser.add_argument('-o', type=str, help='Output arducky ino file', metavar='output_file', dest='output_file', default='arducky_femto_with_payload.ino')

    args = argumentParser.parse_args()

    try:
        code = ""
        payload = ""
        codeWithPayload = ""

        with open(args.ino_file, 'r') as inoFile:
            code = inoFile.read()

        with open(args.payload_file, 'r') as payloadFile:
            payload = payloadFile.read()

        codeWithPayload = re.sub(PAYLOAD_REGEX, "\g<1>" + formatPayload(payload) + "\t\g<3>", code, flags=re.DOTALL)

        with open(args.output_file, 'w') as outputFile:
            outputFile.write(codeWithPayload)
    except IOError as e:
        print("No such file or directory:", e.filename)

__main__()
