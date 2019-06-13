from os import system
from sys import argv
from fractal import Fractal
from threading import Thread, Lock
from state import State
from time import sleep

global state
global lock
global threads

lock = Lock()
threads = []

def print_menu():
	print()
	print('write fractal  w')
	print('view status    s')
	print('make movie     m')
	print('remove files   x')
	print('pan up         u')
	print('pan down       d')
	print('pan left       l')
	print('pan right      r')
	print('zoom in        i')
	print('zoom out       o')
	print('view state     v')
	print('reset state    r')
	print('view help      h')
	print('quit           q')
	print()

def view_status():
	lock.acquire()
	for val in threads:
		print(f'{val.name}: {val.status:0.0f}%')
	print()
	lock.release() 

def cleanup():
	system('rm -f img_*')	

def main_loop():
	cleanup()
	print_menu()	
	
	while True:
		try:
			# wait until threads have been fully initialized
			lock.acquire()

			#  no threads are being started
			lock.release()
	
			cmd = input('$ ')

		except KeyboardInterrupt:
			print()
			continue
	
		if cmd is 'q':
			break

		elif cmd is 'h':
			print_menu()

		elif cmd is 'r':
			# wait until quiet
			lock.acquire()

			# reset values
			state.reset()
			
			# clear out list
			global threads
			threads = []

			lock.release()

		elif cmd is 'v':
			state.view()

		elif cmd is 'i':
			state.zoom_in()

		elif cmd is 'o':
			state.zoom_out()

		elif cmd is 'u':
			state.pan_up()

		elif cmd is 'd':
			state.pan_down()

		elif cmd is 'l':
			state.pan_left()

		elif cmd is 'r':
			state.pan_right()

		elif cmd is 'w':
			# acquire lock and expect worker to release
			lock.acquire()

			# start write fractal thread
			thread = Thread(target = write)
			thread.start()
		
		elif cmd is 's':
			view_status()		

		elif cmd is 'x':
			cleanup()		

		elif cmd is 'm':
			print('status: looking for active threads')

			lock.acquire() 
			old_count = len(threads)
			lock.release() 

			# wait for all threads to finish
			while True:
				lock.acquire() 
				active = [t for t in threads if t.status < 100]
				new_count = len(active)
				lock.release() 

				if new_count == 0:
					break

				elif new_count < old_count:
					print('status: waiting for threads to finish')
					view_status()
				
				sleep(1)
				old_count = new_count
	
			print('status: no active threads')
			print('status: running ffmpeg command')
			system('rm -f output.mp4')
			system('ffmpeg -framerate 1 -i img_%04d.png -r 30 output.mp4 > /dev/null 2>&1')	
			print('status: see output.mp4')

		else:
			print('  command not found')
			print_menu()

def write():
	# put thread object in list
	fractal = Fractal() 
	threads.append(fractal)

	# write is expected to release lock
	fractal.write(state, lock)	
	

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

	

