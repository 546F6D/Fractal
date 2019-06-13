from array import array
from math import floor
from os import system
from pixel import Pixel
import pixel
import state

class Fractal:
	def __init__(self):
		self.status = 0 

	def calc_pixels(self):
		# set y coordinate
		y = self.min_y

		for py in range(self.height):

			# set x coordinate
			x = self.min_x
		
			# update status
			self.status = (py / self.height) * 100

			for px in range(self.width):
		
				# calculate RGB for pixel and add to buffer
				color = pixel.calc(x, y);
				color.add(self.buffer)
			
				# move to next position
				x += self.step_x
			
			# move to next position
			y += self.step_y

	def write(self, state, lock):
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

		# finished with state data, release lock
		lock.release()

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
			self.calc_pixels()
	
			# write pixel buffer to file
			self.buffer.tofile(file)

		# convert ppm to png
		system(f'convert {self.name}.ppm {self.name}.png')

		# remove ppm file
		system(f'rm -f {self.name}.ppm')

		# status goes to done
		self.status = 100

