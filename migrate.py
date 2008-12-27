#!/usr/bin/python

import os
import shutil
import sys

def usage():
	print 'usage: migrate.py destdir'
	sys.exit(1)

def toletter(i):
	assert(i >= 0 and i < 26 * 26)
	return chr(i / 26 + ord('a')) + chr(i % 26 + ord('a'))

def getindex(destdir):
	for i in range(0, 26 * 26):
		letters = toletter(i)
		try:
			f = open(os.path.join(destdir, 'best.' + letters + '.png'))
			continue
		except IOError:
			pass
		try:
			f = open(os.path.join(destdir, 'state.' + letters + '.xml'))
			continue
		except IOError:
			pass
		return letters
	assert(1 == 2)

def migrate(destdir):
	try:
		f = open('best.png')
		f.close()
		f = open('state.xml')
		f.close()
	except IOError:
		print 'best.png or state.xml not found in the current directory'
		sys.exit(1)

	idx = getindex(destdir)

	shutil.copy('best.png', os.path.join(destdir, 'best.' + idx + '.png'))
	shutil.copy('state.xml', os.path.join(destdir, 'state.' + idx + '.xml'))

if __name__ == '__main__':
	if len(sys.argv) != 2:
		usage()
	migrate(sys.argv[1]);
