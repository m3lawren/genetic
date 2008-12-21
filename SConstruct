extras = Dir('extras')
tinyxml = Dir('tinyxml')
pwd = Dir('.')

env = Environment(CCFLAGS='-g -Wextra -Wall -Werror `sdl-config --cflags`', LIBPATH=[extras, tinyxml], CPPPATH=[extras, tinyxml, pwd])

SConscript('extras/SConscript')
SConscript('tinyxml/SConscript')

sources = [
	'dna.cc',
	'graphics.cc',
	'main.cc',
	'polygon.cc',
]

env.Program('genetic', sources, LIBS=['SDL', 'SDL_image', 'SDL_gfx', 'extras', 'png', 'tinyxml'])
