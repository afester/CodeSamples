#!/usr/bin/env python3

# This program creates a specified file with a given size and a given
# byte value at each position.
#
# Example:
# $ ./genFile.py -c 128 -v 0xAB test.dat
# Creating file "test.dat" with 128 bytes of value 0xab ...
# 
# $ od -v -Ax -t x1 test.dat 
# 000000 ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab
# 000010 ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab
# 000020 ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab
# 000030 ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab
# 000040 ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab
# 000050 ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab
# 000060 ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab
# 000070 ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab ab
# 000080


import argparse
import sys


def doCreateFile(params):
    # Create array of 512 bytes with specified value
    byteArray = bytes([params.value] * 512)

    # Write the file in chunks of len(byteArray)
    count = params.count
    with open(params.fileName, 'wb') as destFile:
        while count > 0:
            if count >= len(byteArray):
                destFile.write(byteArray)
            elif count > 0:
                destFile.write(byteArray[:count])
            count -= len(byteArray)


def main(argv):
    # Parse the command line
    parser = argparse.ArgumentParser()
    parser.add_argument('-c', '--count', type=int, required=True, help='Set the number of bytes to write')
    parser.add_argument('-v', '--value', type=str, required=True, help='Specifies the value to write into each byte of the file, either a decimal number between 0..255 or a hex number prepended with "0x" between 0x00..0xff')
    parser.add_argument('fileName', help='The name of the destination file')
    args = parser.parse_args()

    # Convert args.value into int between 0 and 255
    try:
        if args.value.startswith('0x'):
            args.value = int(args.value, 16)
        else:
            args.value = int(args.value)
    except ValueError as ve:
        print('Invalid byte value specified.')
        sys.exit(1)

    if args.value < 0 or args.value > 255:
        print('Invalid byte value specified.')
        sys.exit(1)

    # Create the file with the specified parameters
    print('Creating file "{}" with {} bytes of value 0x{:02x} ...'.format(args.fileName, args.count, args.value))
    doCreateFile(args)


if __name__ == '__main__':
    main(sys.argv)

