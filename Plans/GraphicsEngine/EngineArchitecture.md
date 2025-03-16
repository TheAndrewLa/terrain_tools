# ARCHITECTURE OF GRAPHICS ENGINE

## General systems

1. Scene
1. ECS
1. Lights & materials
1. Camera
1. Execution system (events & synchronization)
1. Memory management (different allocators, smart pointers and other stuff)
1. Debug system _(optional)_

## Platform-dependent systems

1. Window + Platform Events
1. Virtual memory
1. IO
	- File path abstraction
	- Creating files
	- Opening files
	- Closing files
	- Deleting files
	- Creating file mappings
1. Dynamic libraries (.dll/.so) _(optional)_:
	- Loading dynamic libraries
	- Getting proc addresses with converting it to function pointer
	- Releasing dynamic libraries

## Graphics-dependent systems

1. Swapchain
1. Device
1. CommandQueue & synchronization
1. Textures & samplers
1. Buffers
1. Shaders

# 1st milestone

- Implement listed graphics systems for modern OpenGL (4 or higher, it matters)
- Implement listed platform systems for Windows (mostly 10, smth from 11, if possible)
- Implement listed general systems
- Create several demos:
	1. Displaying texture (loaded from file)
	1. Cubes with 3D & lighting
	1. Flying camera
