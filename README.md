# RTEngine
Thats what I call a Rootin Tootin Engine partner

## Engine Info
RT Engine is a game engine developed in C++ using OpenGL, and right now, it features funcionality to load FBX, which are exported into our own fileformat, and saving and loading of scenes. In addition, the user can create or add LUA scripts to the gameobjects, as long as they follow the TEMPLATE structure. It is developed by Joel Cabaco Perez and Lorien Portella Lopez, both students of the Bachelor's Degree in Videogame Design and Development at CITM, a UPC center.


## Authors
[Joel Cabaco](https://github.com/dynamiczero99)
- Editor Configuration window (settings for application, camera, render, space partitioning, window, hardware and geometry).
- Primitive creation through par_shapes.
- Time management (play, pause, stop and component updates).
- Asset explorer window.
- Space partitioning (quadtree).
- FPS limitation and management.
- Hardware display.
- Frustum culling.
- Text editor.
### Tasks:
[Lorién Portella](https://github.com/Witiza)
- Model importing.
- Model rendering.
- Own file format.
- Hierarchy.
- Tri-based mouse picking.
- Inspector.
- Console.
- Texturing.
- Gameobject and component structure.
- Serialization.
- Scripting system.

## How to use
### Installation
Just extract the contents of the zip into a folder and click the .exe
### Model loading
- If an .fbx file is dropped into the engine, it will be copied into the assets folder, and then exported. To open it, navigate to the models folder in the assets explorer, and click the button with its name.
- If an .fbx file is placed within the assets folder of the engine (the models subfolder ideally), the user will be able to click it from the assets explorer of the engine. Doing so, will import the model, and clicking it again, will open it in the engine. 
- If any fbx is changed externally, the model will need to be manually deleted from the library folder, and then reimported as stated above.
### Scripting
- Creating Scripts: Scripts can be created or added in both ways. The user can select New Script and input a name (which can not have any blank spaces), or select an already existing script from the Assets//Scripts folder. External scripts can also be used, by placing them in the Assets//Scripts folder but at the moment, the scripting system is very limited, and ony accepts scripts with the TEMPLATE format
- TEMPLATE format: Created to imitate the class system of C, the script needs to be encapsulated in a function with its own name, that returns a table. Inside this table, there needs to be all the script funcionality.
- C++/LUA functions: Easy to expand, right now the engine has three namespaces: BASIC, INPUT, OUTPUT, with a limited API, displayed in the Tank scene, that acts as a demo. 
- BASIC:
- LUALog to log strings into the console
- LUAGetDT to get the dt of the frame in seconds
- DeleteThis to delete the gameobject attached to the script
- INPUT: 
- LUAGetKeyState to get the state of a key
- LUAGetButtonState to get the state of a button
- LUAGetMouseX/Y to get the position of the mouse onscreen
- LUAGetPositionX/Y/Z to get the value from the transform
- OUTPUT: 
- MoveX/Y/Z to move along those axis in global space
- MoveForward to move in the foward vector of the gameobject
- MoveSideways to move perpendicular to the forward of the gameobject
- RotateAlongY to rotate using the Y axis in world space
- Instantiate to instantiate a prefab by name
        
- Tank Scene: Control the tank with WASD. Shoot bullets with the mouse left click. To rotate the turret, move the mouse in the X axis of the screen.

### Camera Controls
- Right mouse button - Look around
- W, S - Move forward/back (while looking around)
- A, D - Move left/right (while looking around)
- Q, E - Move up/down (while looking around)
- R, F - Pan up/down (while looking around)
- Z, X - Roll left/right (while looking around)
- T - Reset camera

## Repo
[Engine Repo](https://github.com/RootinTootinCoodin/RTEngine)
