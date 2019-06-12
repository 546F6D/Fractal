from sys import argv
from fractal import Fractal
from state import State

global state

def print_menu():
	print()
	print('write fractal  w')
	print('pan up         1')
	print('pan down       2')
	print('pan left       3')
	print('pan right      4')
	print('zoom in        +')
	print('zoom out       -')
	print('view state     v')
	print('reset state    r')
	print('view help      h')
	print('quit           q')
	print()

def main_loop():
	print_menu()	
	
	while True:
		try:
			cmd = input('$ ')

		except KeyboardInterrupt:
			print()
			continue
	
		if cmd is 'q':
			break

		elif cmd is 'h':
			print_menu()

		elif cmd is 'r':
			state.reset()
			state.view()

		elif cmd is 'v':
			state.view()

		elif cmd is '+':
			state.zoom_in()

		elif cmd is '-':
			state.zoom_out()

		elif cmd is '1':
			state.pan_up()

		elif cmd is '2':
			state.pan_down()

		elif cmd is '3':
			state.pan_left()

		elif cmd is '4':
			state.pan_right()

		elif cmd is 'w':
			print('  write fractal')
			print()
			
			fractal = Fractal() 
			fractal.write(state)	

		else:
			print('  command not found')
			print_menu()

if __name__ == '__main__':

	# without arguments, print usage and quit
	if len(argv) < 3:
		print('Usage: python3 main.py <width> <height>')
		exit(1)

	# retrieve width and height
	try:
		width = int(argv[1])
		height = int(argv[2])

		if width < 1 or height < 1:
			raise Exception

	except:
		print('Error: invalid width or height')
		exit(1)
	
	# create state object
	global state
	state = State(width, height)		

	# enter main command loop	
	main_loop()

	

