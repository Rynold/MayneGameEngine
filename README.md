# MayneGameEngine
This is a work in progress GameEngine 

So Far it is a little messy, but I am proud of the progress I have made so far with OpenGL Rendering.

To get this to run you will need to move the files from ".dll Files" into "ComponentFramework1.0/Debug"
You will also have to change the paths of the library directories (Properties->Linker->General->Additional Library Directories)
to match where you put them in your system. For some reason I could not make these paths relative due to a linker error.

All of the necessary files are included in the repo.


This Graphics Renderer is the begining of a game engine I have been working on during my spare time. It uses modern OpenGL
to create great looking scenes. So far the rendering capabilities include: Textures, Normal Maps, Parralax Mapping, Directional Lights,
Point Lights, Soft Shadows, SkyBoxes and Model Loading.

It also has basic Networking right now, so once the program starts it will prompt you in the window to choose whether your the server
or not. Select either 'Y' or 'N' and be on your way. It makes no difference which one you choose for a single app, however if you want
to use the networking you must select the server then open another instance and select the client. Moving the camera in the client will
now also move the camera on the server.