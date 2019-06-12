global state

class State:
	ZOOM = 1.1
	PAN  = 0.2

	def __init__(self, width, height):
		self.width    = width
		self.height   = height
		self.reset()

	def view(self):
		print()
		print(f'width    = {self.width}')
		print(f'height   = {self.height}')
		print(f'center_x = {self.center_x}')
		print(f'center_y = {self.center_y}')
		print(f'zoom     = {self.zoom}')
		print(f'next_id  = {self.next_id}')
		print()
	
	def reset(self):
		self.center_x = 0.0
		self.center_y = 0.0
		self.zoom     = 1.0
		self.next_id  = 0
	
	def zoom_in(self):
		print('  zoom in')
		self.zoom /= State.ZOOM
		self.view()
	
	def zoom_out(self):	
		print('  zoom out')
		self.zoom *= State.ZOOM
		self.view()

	def pan_up(self):
		print('  pan up')
		self.center_y += State.PAN * self.zoom
		self.view()

	def pan_down(self):
		print('  pan down')
		self.center_y -= State.PAN * self.zoom
		self.view()

	def pan_left(self):
		print('  pan left')
		self.center_x += State.PAN * self.zoom
		self.view()

	def pan_right(self):
		print('  pan left')
		self.center_x -= State.PAN * self.zoom
		self.view()

	def next_name(self):
		name = f'img_{self.next_id:{0}{4}}'
		self.next_id += 1
		return name


