extras = Dir('extras')
pwd = Dir('.')

env = Environment(CCFLAGS='-g -Wextra -Wall -Werror `sdl-config --cflags`', LIBPATH='extras', CPPPATH=[extras, pwd])

SConscript('extras/SConscript')

sources = [
	'graphics.cc',
	'main.cc',
	'marshall.cc',
]

env.Program('genetic', sources, LIBS=['SDL', 'SDL_image', 'SDL_gfx', 'extras', 'png'])
