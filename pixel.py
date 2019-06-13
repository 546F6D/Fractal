from math import *

class Pixel:
	def __init__(self, r, g, b):
		self.r = int(r)
		self.g = int(g)
		self.b = int(b)

	# add RGB values to pixel buffer
	def add(self, buffer):
		buffer.append(self.r)
		buffer.append(self.g)
		buffer.append(self.b)

MAX  = 1000
BAIL = 4.0

# find RGB values for pixel at (px, py)
def calc(px, py):
	
	# find number of iterations	to escape
	x = 0.0
	y = 0.0
	it = 0
	
	# iterations to escape
	while (x**2 + y**2 <= BAIL and it < MAX):
		tmp = x**2 - y**2 + px
		y = 2*x*y + py
		x = tmp
		it += 1

	if (it == MAX):
		return color(MAX)

	if (it + 1 == MAX):
		return color(it)
	
	# smooth coloring with interpolation
	zn = log(x**2 + y**2) / 2
	nu = log(zn / log(2)) / log(2)
	it = it + 1 - nu

	# end points
	a = color(floor(it))
	b = color(floor(it) + 1)

	# final color
	r = interpolate(a.r, b.r, it % 1)
	g = interpolate(a.g, b.g, it % 1)
	b = interpolate(a.b, b.b, it % 1)

	return Pixel(r, g, b)

# return RGB given value in range [0, MAX]
def color(value):
	
	# normalize value
	normal = value / MAX

	r = normal * 35
	g = normal * 35
	b = normal * 200

	return Pixel(r, g, b)

# linear interpolation
def interpolate(a, b, t):
	u = 1 - t
	return a*u + b*t









