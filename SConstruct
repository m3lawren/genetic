extras = Dir('extras')
extrasIMG = Dir('extras/IMG')
extrasTinyxml = Dir('extras/tinyxml')
pwd = Dir('.')

env = Environment(CXXFLAGS='-g -Wextra -Wall -Werror `sdl-config --cflags` -std=c++98 -pedantic', CFLAGS='-O2', LIBPATH=[extras], CPPPATH=[extrasIMG, extrasTinyxml, pwd])

SConscript('extras/SConscript')

sources = [
	'config.cc',
	'dna.cc',
	'graphics.cc',
	'history.cc',
	'main.cc',
	#'marshall.cc',
	'mtwist.c',
	'polygon.cc',
	'utils.cc',
]

env.Program('genetic', sources, LIBS=['SDL', 'SDL_image', 'SDL_gfx', 'extras', 'png'])
