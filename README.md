# Split Screen

I am Albert Robles Muñoz, student of the [Bachelor’s Degree in Video Games by UPC at CITM](https://www.citm.upc.edu/ing/estudis/graus-videojocs/). 
This content is generated for the second year’s subject Project 2, under supervision of lecturer [Ramon Santamaria](https://www.linkedin.com/in/raysan/).

This C++ project has been programmed using [Visual Studio](https://visualstudio.microsoft.com/es/vs/)

Feel free to visit:
- [Split Screen Repository](https://github.com/Albertito029/Split-Screen-Research)

## General Controls

- Player 1:

	Up Arrow: Throttle / Accelerate
	
	Down Arrow: Brake
	
	Left Arrow: Turn Left
	
	Right Arrow: Turn Right
	
	
- Player 2:

	W Key: Throttle / Accelerate
	
	S Key: Brake
	
	A Key: Turn Left
	
	D Key: Turn Right
	

## License
Licensed under the [MIT License](LICENSE)

## Description

This project is meant to be a guide to teach how to implement a "Split-Screen" in a videogame. We're using a little sample of a project built with Bullet Physics SDK where we have two cars in a plane. The goal is to create two separate viewports with a camera attatched to them that follow each car.

## Code implementation

This handout has a basic program that lets you move two cars in the space with the Arrows and WASD.

### TODO 1: Creating a new camera

First we have to create a new module for the second camera.

<img src="https://github.com/Albertito029/Split-Screen-Research/blob/master/Docs/Images/Todo%201.PNG" alt="" class="inline"/>
<img src="https://github.com/Albertito029/Split-Screen-Research/blob/master/Docs/Images/Todo%201%20(2).PNG" alt="" class="inline"/>

### Solution:

<img src="https://github.com/Albertito029/Split-Screen-Research/blob/master/Docs/Images/Todo%201%20Solution.PNG" alt="" class="inline"/>
<img src="https://github.com/Albertito029/Split-Screen-Research/blob/master/Docs/Images/Todo%201%20Solution%20(2).PNG" alt="" class="inline"/>

### TODO 2: Setting up first viewport

<img src="https://github.com/Albertito029/Split-Screen-Research/blob/master/Docs/Images/Todo%202.PNG" alt="" class="inline"/>

### Solution:

<img src="https://github.com/Albertito029/Split-Screen-Research/blob/master/Docs/Images/Todo%202%20Solution.PNG" alt="" class="inline"/>

### TODO 3: Setting up more than one viewport

<img src="https://github.com/Albertito029/Split-Screen-Research/blob/master/Docs/Images/Todo%203.PNG" alt="" class="inline"/>

### Solution:

<img src="https://github.com/Albertito029/Split-Screen-Research/blob/master/Docs/Images/Todo%203%20Solution.PNG" alt="" class="inline"/>

### TODO 4: Assign the new camera to the player

<img src="https://github.com/Albertito029/Split-Screen-Research/blob/master/Docs/Images/Todo%204.PNG" alt="" class="inline"/>

### Solution:

<img src="https://github.com/Albertito029/Split-Screen-Research/blob/master/Docs/Images/Todo%204%20Solution.PNG" alt="" class="inline"/>


## References

<a href="https://en.wikipedia.org/wiki/Split_screen_(video_games)">Split screen in video games</a>

<a href="https://docs.microsoft.com/en-us/windows/win32/opengl/opengl">OpenGL libraries</a>

