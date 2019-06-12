from array import array
from math import floor
from time import time
from os import system
from pixel import Pixel
import pixel
import state

class Fractal:
	def calc_pixels(self):
		timer = time()

		# set y coordinate
		y = self.min_y

		for py in range(self.height):

			# print progress after 3 seconds 
			if (time() - timer > 3):
				percent = py / self.height * 100;
				self.log(f'{percent:0.0f}% complete')

				# reset timer
				timer = time()

			# set x coordinate
			x = self.min_x

			for px in range(self.width):
		
				# calculate RGB for pixel and add to buffer
				color = pixel.calc(x, y);
				color.add(self.buffer)
			
				# move to next position
				x += self.step_x
			
			# move to next position
			y += self.step_y

	def write(self, state):
		# execution start
		start = time()
		
		# image name, width, and height
		self.name = state.next_name()
		self.width = state.width
		self.height = state.height

		self.ratio = self.width / self.height 	
	
		# coordinate plane bound
		self.delta_x = state.zoom * self.ratio
		self.delta_y = state.zoom 
		self.min_x   = state.center_x - self.delta_x
		self.min_y   = state.center_y - self.delta_y

		# image pixel coordinate plane step size
		self.step_x = 2.0 * self.delta_x / self.width
		self.step_y = 2.0 * self.delta_y / self.height

		# create new ppm file
		with open(f'{self.name}.ppm', 'wb') as file:
			# write ppm header
			header = f'P6 {state.width} {state.height} 255\n'
			file.write(bytearray(header, 'ascii'))

			# create pixel buffer
			self.buffer = array('B')

			# calculate pixel data and store in buffer
			self.log('0% complete')
			self.calc_pixels()
			self.log('100% complete')
	
			# write pixel buffer to file
			self.buffer.tofile(file)

		# convert ppm to png
		system(f'convert {self.name}.ppm {self.name}.png')
		self.log(f'see {self.name}.png')

		# remove ppm file
		system(f'rm -f {self.name}.ppm')

		# print elapsed time
		stop = time()
		sec = stop - start
		self.log(f'{sec:0.4f} seconds')
		print()

	# log message to console
	def log(self, msg):
		print(f'{self.name}: {msg}')


