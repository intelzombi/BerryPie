# Berry Pie Rendering Engine

Blackberry pie is a rendering and asset pipeline that I created. That is to say I wrote the code that is contained in
this repository.  My motivation was to really drive an entire rendering pipeline from Artist tools all the way to the
shaders and draw calls.  This is written in C++ using DirectX 11.  Included here is a full pipeline of tools. I tried
mimick what real game engines use.  I'm not suggesting this is the basis for a game engine, but it would make a good
study for those interested in the components used in a game engine. My background is over 20 years of rendering engine
experience from my experience working at Intel on their graphics controllers and being fortunate enough to work with
so many game developers and peeking deep into their rendering engines.  It isn't a small task and there are so many
great tools out there that will do this for you. Unity or Unreal shortcuts all of this for you.

# Maya assets
The actual "Maya" model assets are not included in this project however they are reflected in the model files here.  I
built a plugin for Maya that does a custom export of the assets into the model (what I call AX) files. Additionally, the
plugin allowed for Texture and shader details (materials) to be output into the model file as well.  It was purely an
exercise in curiosity. FBX files and other formats are perfectly acceptable and better suited to most pipeline
structures for all but custom rendering pipelines. That plugin is part of a separate repository that I have not made
public at this point.

# Asset Importer
Starting from the beginning of this repository project is the Asset importer I wrote. It takes those AX models that the
Maya plugin created and imports them in. I'm highlighting this library, so it is clear where the assets are coming from
in this rendering harness.
This library is contained the root /extern folder. I it must be compiled as one of the library dependencies.

# Blackberry Grove
This is the main project it is located under the Thorns directory.  It is a standard C++ windows based rendering engine.
The BlackberryGrove.cpp file however is the only Windows specific file in the project.
The folders of interest here:
- FX: This folder contains Constant buffers, Vertex shaders, Pixel shaders and what I have defined as edf (effect
  definition files).  You can pretty much ignore the FX_archive folder and the compute shader they are not used in this
  rendition of the rendering engine.

- Media: This is nothing more than some texture maps and other sample rendering tidbits.  Most can be ignored as the
just extra stuff.  One of these days I'll clean it up.

- Models: These are the AX files that were created by the Asset Exporter I went over at the beginning.  These are what
Asset Importer finds and processes on request.   In here notice there are a couple of Scene files.  That is to say that
  it has the complete model and material definitions for a complete scene.

- Header and Source: These contain what you would expect.  BlackberryGrove.cpp is the main rendering engine core. The
other files are all the supporting functions; Scenegraph, ModelManager, ResourceManager, EffectsManager, Camera, Light.
  A couple of other items you might find interesting.
  - I added a shuttle class.  This will look a lot like a camera from
  a transform perspective but it is something that is attached to a model and then can be controlled with user input. In
  the Ghost_Plane_Game.ax (the default) scene the Plane has an associated shuttle attached you can see how it works.
  - There is a Vector3.h file.  This is basically a vector and matrix library.  It hijacked and replaces all of the
    D3DVector and D3DMatrix stuff.  I did this so the framework will remain independent outside of the BlackberryGrove
    file.  I have other rendering engines in Linux that I didn't want to have to rewrite all of the core transform
    pieces.

# Running the rendering engine.
After Compiling it BlackberryGrove.exe will need to have an ini file passed into it.  Note the BlackBerryGrove.ini file
in the BlackBerryGrove directory.  This is the file you pass in as a command line parameter to the exe.
note the different scenes to load
- //SCENE="\models\ShadowExample.ax"
- //SCENE="\models\GhostTown_Scene.ax"
- SCENE="\models\Ghost_Plane_Game.ax"
You point it to where your fx, model and media files
- TEXTURE_LOCATION="\Media\"
- EFFECTS_LOCATION="\FX\"
- MODEL_LOCATION="\models\"
The back color of the scene.  Black is so ... black.
- BACKCOLOR="0.83529 0.83529 0.88235 1.0"

Shader Light model constants
- SPEC1="0.1 0.1 0.1"
- POW=8

Positions of the 3 lights
- EL1="2.0 2.0 2.0"
- EL2="1.0 1.0 0.2"
- EL3="0.2 1.0 1.0"

Camera Lock default. This locks the camera on one target.  For this rendering framework I leave it set to 0;
- LOCKCAMERA=0

Resolution of window
- RESOLUTIONWIDTH=1600
- RESOLUTIONHEIGHT=900
- WINDOWEDMODE=0

# Controls
First lets talk about Locking and not locking.  As a framework this just gives you some hacky but interesting controls.
- 'L','U' lock and unlock. This is 'Not' the camera lock in the ini file.  This lock toggles between light control and
Camera control. It is defaulted to camera.

- Camera:
  - 'A' Camera.strafe
  - 'D' Camera.strafe
  - 'W' Camera.walk
  - 'S' Camera.walk
  - 'F' Camera.rise
  - 'C' Camera.rise
  - 'I' Camera.roll
  - 'O' Camera.roll
  - if Shuttle is attached to a model
    - VK_LEFT       Mdl_Plane->radianRoll
    - VK_RIGHT      Mdl_Plane->radianRoll
    - VK_UP         Mdl_Plane->radianPitch
    - VK_DOWN       Mdl_Plane->radianPitch
    - 'B'           Mdl_Plane->velocity
    - 'H'           Mdl_Plane->velocity

- Light
    - 'A'         Light1.strafe(-200.0f*0.0020f
    - 'D'         Light1.strafe(+200.0f*0.0020f
    - 'W'         Light1.walk(+200.0f*0.0020f);
    - 'S'         Light1.walk(-200.0f*0.0020f);
    - 'F'         Light1.rise(+200.0f*0.0020f);
    - 'C'         Light1.rise(-200.0f*0.0020f);
    - 'I'         Light1.roll(-0.0087266f);
    - 'O'         Light1.roll(0.0087266f);
    - VK_LEFT     Light1.rotateY(-0.01f);
    - VK_RIGHT    Light1.rotateY(0.01f);
    - VK_UP       Light1.pitch(-0.01f);
    - VK_DOWN     Light1.pitch(0.01f);

# Scene Files
- ShadowExample.ax" Jack in the box with a background to view shadows with.
- GhostTown_Scene.ax"
- Ghost_Plane_Game.ax"

# Code Points
There are places in the code that you can display the actual shadow map being generated. In the CreateSceneGraph 
in BlackBerryGrove.cpp look for a comment: "display Depth Buffer Scene Item."

It is clear upon inspection that there is some cleanup that needs to happen but in the interest of sharing the 
framework I leave that to a later time.  
