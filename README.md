# OpenGLScenes
A playground for learning OpenGL and real-time graphics. <br/>
This project contains many "scenes" which serve as contained experiments using various methods of real-time rendering.<br/>
Here are some examples of these real-time scenes:

KernelScene: Use of the stencil buffer, Blinn-Phong lighting with a constant directional lighting, a dynamic positional 
lighting with attenuation, and a spotlighting (virtual flashlight) with attenuation, textures with transparency, a real-time kernel 
post-processing shader, model data loaded with the help of [assimp](https://github.com/assimp/assimp), and use of a skybox
cubemap backdrop.
![Stencil And Transparency Example](http://connorahaskins.com/images/OpenGLScenes/StencilTransparency.png)
![Image Kernel Example](http://connorahaskins.com/images/OpenGLScenes/Kernel.png)

InfiniteCubeScene: Use of textures with transparency, using framebuffers as textures for future draw calls, 
blitting between framebuffers.
![Fun Infinite Cube Example](http://connorahaskins.com/images/OpenGLScenes/InfiniteCube.png)

InfiniteCapsuleScene: Use of ray marching to create infinitely many objects, diffuse lighting using normals
based on signed distance values of slight nudges in x/y/z away from surface point, dynamic point light that is "shot"
from center of screen from user input.
![Ray Marching Infinite Capsules Example](http://connorahaskins.com/images/OpenGLScenes/InfiniteCapsules.png)

MandelbrotScene: A 2D visualization of the mandelbrot set, supports zooming until nudges in the floating point coordinates
can no longer be distinguished from the next. In the future, this scene may use integers w/ fixed-point math to expanding
the level of supported zoom.
![Mandelbrot Example](http://connorahaskins.com/images/OpenGLScenes/Mandelbrot.png)

RoomScene: Use of dynamic (moving) point lighting with dynamic shadows, depth framebuffer with cubemap color attachment, 
geometry shader to render all six views with one draw call, Blinn-Phong shading, percentage-closer filtering (PCF) which
takes an average of shadow values around a surface point to give smoother shadows, utilizes a uniform buffer.
![Dynamic Point Light Shadows Example](http://connorahaskins.com/images/OpenGLScenes/DynamicShadowsPointLight.png)

MoonScene: A billboard quad with moon texture, Blinn-Phong lighting with the use of normal map and height maps used for 
parallax occlusion mapping, dynamic shadows from moving directional light source, manual gamma correction.
![Bump And Parallax Mapping Example](http://connorahaskins.com/images/OpenGLScenes/BumpParallax.png)

AsteroidScene: Instancing of 5000 "asteroid" models and one giant "planet" model, each surface type is perfectly reflective and simply 
reflect two separate skybox textures.
![Instancing Example](http://connorahaskins.com/images/OpenGLScenes/Instancing.png)

ReflectRefractScene: Instancing of cube objects, use of geometry shader to "blow up" models over time by pushing it's triangles 
out in the direction of their surface normals, perfectly reflective cubes that reflect the skybox, geometry shader used to 
display surface normals at each vertex, refractive "Starman" model with ability to cycle through various refractive index
values.
![Reflection And Refraction Example](http://connorahaskins.com/images/OpenGLScenes/ReflectRefract.png)

MengerSpongeScene: Deeper exploration of ray marching, fusing the ray marching scene with a rasterized cube by matching the view and
projection values, allowing the cube & ray marching scene to exist in the same space and appropriately occlude each other by 
manually writing the depth values through translation of the ray marching hit coordinate to NDC, adjustable super sampling slider that 
can be used to take 1 to 8 samples per pixel, rendered to separate framebuffer that allows various resolutions between <120w,68h> to 
<3840w,2160h>, shading is simply based on ray marching iterations, uses ray marching boolean operations and scaling operations (with 
major help from Inigo Quilez' [distance functions article](https://iquilezles.org/www/articles/distfunctions/distfunctions.htm). 
This scene also has an associated [blog post](http://connorahaskins.com/menger_sponge.html) and [ShaderToy](https://www.google.com/url?q=https://www.shadertoy.com/view/3lKSWh&sa=D&ust=1592278564027000).
![Ray Marching Menger Sponge Example](http://connorahaskins.com/images/OpenGLScenes/MengerPrison3840x2160.png)

MengerSpongeScene (modified): Fun stuff by throwing some "noise" into the distance field by adjusting the value of the SDF based
on arbitrary sin values. [Although calculating sin values is expensive, the main bog down when running in this in 4K seems to be due 
to the unusually large number of iterations of each ray (which, in turn, does require many more sin values to be calculated).]
![Menger Sponge With Noise Example](http://connorahaskins.com/images/OpenGLScenes/MengerNoisePrison.png)

MultiScene: Thanks to the way that the Scenes are structured, four scenes can run at once in real-time, all Scenes still
accepting input from peripherals, with the ability to cycle through all scenes while drawing 4 of them simultaneously, each
scene is rendered with a framebuffer equal to the size of their portion of the window, no hiccups taking FPS below 60 seen on my 
personal machine unless it features the MengerSpongeScene rendering at higher resolutions with many samples per pixel.
![Multiple Scenes Example](http://connorahaskins.com/images/OpenGLScenes/MultiScene.png)

There is also support for switching between "scenes" while the application is running.
![alt tag](http://connorahaskins.com/images/OpenGLScenes/SwitchScenes.png)

All snapshots are taking through the application with the hot key 'Alt-`', which grabs a Scene's framebuffer returned from 
Scene::draw() and saves the data to a simple .bmp image file.

## Special Thanks

Joey de Vries' amazing OpenGL tutorial: [Learn Open](https://learnopengl.com/)<br/>
Inigo Quilez' fantastic ray marching articles: [website](https://www.iquilezles.org/)<br/>
Dear ImGui, an extremely user friendly imgui library: [Dear ImGui](https://github.com/ocornut/imgui)<br/>
Assimp library for easy loading of .obj files: [assimp](https://github.com/assimp/assimp)<br/>

Free art used for this project:<br/>
* Skybox Artists - [StumpyStrust](https://opengameart.org/users/stumpystrust), [CustomMapMakers](http://www.custommapmakers.org/skyboxes.php), [Pieter ‘Spiney’ Verhoeve](https://opengameart.org/users/spiney)<br/>
* Model Artists - [ivangeta79](https://sketchfab.com/ivangeta79), [Kyra Anastasia Kabler](https://www.kyraanastasia.com/)
* Textures - [Free PBR](https://freepbr.com/)