# ARCHITECTURE OF GRAPHICS ENGINE
## _Will be updated during work_

## General systems:

"App startup" = a boilerplate code with loading all engine systems and starting app

```
#include <engine.h>
#include <game.h>

int main(int argc, char** argv)
{
	engine::load_runtime(argc, argv);
	engine::load_systems(argc, argv);	

	auto terrain_visualizer = engine::load_game("My own app!");

	while (terrain_visualizer.is_running())
	{
		// Dispatch events
		// Do *actions* depends on events
		// Clear event queue
	}

	engine::release_systems();
	engine::release_runtime();
	
	return EXIT_STATUS;
}
```

1. Scene
1. ECS
1. Lights & materials
1. Camera
1. Scripts Loader & executor _(optional)_
1. Memory management _(optional)_

## Platform-dependent systems:

1. Window
1. Input
1. Execution & synchronization
1. Virtual memory
1. IO
1. Dynamic libraries (.dll/.so) _(optional)_

## Graphics-dependent systems:

1. Textures & samplers
1. Buffers
1. Shaders
1. Execution & synchronization

# 1st milestone

- Implement listed graphics systems for modern OpenGL (4 or higher, it matters)
- Implement listed platform systems for Windows (mostly 10, smth from 11)
- Implement listed general systems
- Realize several demos:
	1. Displaying texture (loaded from file)
	1. Cubes with 3D & lighting
	1. Flying camera
