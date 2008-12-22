extras = Dir('extras')
extrasIMG = Dir('extras/IMG')
extrasTinyxml = Dir('extras/tinyxml')
pwd = Dir('.')

env = Environment(CCFLAGS='-g -Wextra -Wall -Werror `sdl-config --cflags` -std=c++98 -pedantic -DTIXML_USE_STL', LIBPATH=[extras], CPPPATH=[extrasIMG, extrasTinyxml, pwd])

SConscript('extras/SConscript')

sources = [
	'config.cc',
	'dna.cc',
	'graphics.cc',
	'history.cc',
	'main.cc',
	'marshall.cc',
	'mutations.cc',
	'polygon.cc',
]

env.Program('genetic', sources, LIBS=['SDL', 'SDL_image', 'SDL_gfx', 'extras', 'png'])
