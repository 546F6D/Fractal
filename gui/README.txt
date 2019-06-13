Usage
	./mandelbrot

	- The screen size (and a few other variables) can be changed in config.h 
    - It takes a couple seconds to render after each command. During that time
      additional commands are ignored to prevent overloading.

Window Controls
	Pan Up		Up Arrow Key
	Pan Down	Down Arrow Key
	Pan Left	Left Arrow Key
	Pan Right	Right Arrow Key		
	Zoom In		Page Up
	Zoom Out	Page Down
	Shell		Space Bar
	Exit		Escape Key 


Shell Commands
	- Activate the shell by pressing the space bar while focused on the window.
    - Only one command is run each time the shell is activated.
    - There's no error checking.

	Set base color for fractal
		set [red|green|blue] [0-255]
	
	Set center point 
		set [cx|cx] <value>	

	Set zoom
		set zoom <value>

	Exit program
		exit


Build Requirements
	g++ with GLUT and OpenMP libraries


