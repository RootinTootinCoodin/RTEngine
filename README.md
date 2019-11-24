# RTEngine
Thats what I call a Rootin Tootin Engine partner

## Engine Info
RT Engine is a game engine developed in C++ using OpenGL, and right now, it features funcionality to load FBX, which are exported into our own fileformat, and saving and loading of scenes. It is developed by Joel Cabaco Perez and Lorien Portella Lopez, both students of the Bachelor's Degree in Videogame Design and Development at CITM, a UPC center.


## Authors
[Joel Cabaco](https://github.com/dynamiczero99)
- Asset window
- ComponentCamera
- Frustum Culling
- Quadtree
- Time Manager
### Tasks:
[Lorién Portella](https://github.com/Witiza)
- Transform Structure
- Own Fileformat
- Scene saving and loading
- Resource Manager
- Mousepicking

## How to use
### Installation
Just extract the contents of the zip into a folder and click the .exe
### Model loading
- If an .fbx file is dropped into the engine, it will be copied into the assets folder, and then exported. To open it, navigate to the models folder in the assets explorer, and click the button with its name.
- If an .fbx file is placed within the assets folder of the engine (the models subfolder ideally), the user will be able to click it from the assets explorer of the engine. Doing so, will import the model, and clicking it again, will open it in the engine. 
- If any fbx is changed externally, the model will need to be manually deleted from the library folder, and then reimported as stated above.
### Camera Controls
- Right mouse button - Look around
- W, S - Move forward/back (while looking around)
- A, D - Move left/right (while looking around)
- Q, E - Move up/down (while looking around)
- R, F - Pan up/down (while looking around)
- Z, X - Roll left/right (while looking around)
- T - Reset camera


## Other
### Development Notes:
We are using maps as containers for the Component and 	Children lists. This is because we realized in Project 3 	that by doing so, the performance of the engine increases 	greatly, despite right now not making a difference.
### Know Bugs:
Right now, there is an important bug in the engine, which renders the AABBS useless when using the camera or the quadtree, spite them working perfectly when drawn. We have looked into this issue but haven't found a solution. Basically, when using the culling or quadtree, the AABB's are treated as if they were in the center, without the transformation applied.



## Repo
[Engine Repo](https://github.com/RootinTootinCoodin/RTEngine)
