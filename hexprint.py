#!/usr/bin/python

import sys


def printhex(instr):
	i = 0
	for a in instr:
		if i != 0 :
			sys.stdout.write(',')
		sys.stdout.write('0x%02x'%(ord(a)))
		i += 1

	sys.stdout.write('\n')

def main():
	for a in sys.argv[1:]:
		printhex(a)
	return

main()