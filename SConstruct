extras = Dir('extras')
extrasIMG = Dir('extras/IMG')
extrasTinyxml = Dir('extras/tinyxml')
pwd = Dir('.')

debug = ARGUMENTS.get('debug', 0)
ctags = ARGUMENTS.get('ctags', 0)

env = Environment(CXXFLAGS='-O2 -Wextra -Wall -Werror `sdl-config --cflags` -std=c++98 -pedantic', CFLAGS='-O2', LIBPATH=[extras], CPPPATH=[extrasIMG, extrasTinyxml, pwd])

if int(debug):
	env.Append(CXXFLAGS=' -g')

if int(ctags):
	import os
	os.system('ctags -R *')

SConscript('extras/SConscript')

sources = [
	'config.cc',
	'dna.cc',
	'graphics.cc',
	'history.cc',
	'main.cc',
	'marshall.cc',
	'mtwist.c',
	'polygon.cc',
	'utils.cc',
]

env.Program('genetic', sources, LIBS=['SDL', 'SDL_image', 'SDL_gfx', 'extras', 'png'])
env.Program('merge', 'merge.cc', LIBS=['SDL', 'SDL_image', 'SDL_gfx', 'extras', 'png'])
