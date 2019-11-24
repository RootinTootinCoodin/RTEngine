RT Engine

That's what I call a rootin' tootin' engine, partner!

RT Engine is a game engine developed in C++ using OpenGL. It is developed by Joel Cabaco Perez and Lorien Portella Lopez, both students of the Bachelor's Degree in Videogame Design and Development at CITM, a UPC center.

You can see the source code of the engine in our Github repository, here:
https://github.com/RootinTootinCoodin/RTEngine

Installation:
	Just extracts the contents of the zip and double click 	the .exe

How to use:
	Drag and drop any fbx to the engine to view it as a 	gameobject
	Drag and drop any png or dds to apply this texture to all 	the gameobjects.
Camera Controls:
		   
 Middle mouse button - Drag camera
  Right mouse button - Look around
		W, S - Move forward/back (while looking around)
		A, D - Move left/right (while looking around)
		Q, E - Move up/down (while looking around)
    	    Left Alt - Move around origin
		   F - Reset camera

Development Notes:
	We are using maps as containers for the Component and 	Children lists. This is because we realized in Project 3 	that by doing so, the performance of the engine increases 	greatly, despite right now not making a difference
