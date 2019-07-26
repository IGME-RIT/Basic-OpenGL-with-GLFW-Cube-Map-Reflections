Documentation Author: Niko Procopi 2019

This tutorial was designed for Visual Studio 2017 / 2019
If the solution does not compile, retarget the solution
to a different version of the Windows SDK. If you do not
have any version of the Windows SDK, it can be installed
from the Visual Studio Installer Tool

Welcome to the Cubemap Reflection-Flat Tutorial!
Prerequesites: Skybox, OBJ Loading

Rather than drawing an object in the world with
a regular texture on it, we will be drawing a perfect
reflection of teh skybox on our model, as if our
model is made of flawless glass

Remove the color-texture from the fragment shader
and replace it with the same cubemap that we used for
the Skybox.

As discussed in the last tutorial, we want to use
a 3D coordinate system for the cubemap UV coordinates,
but how do we get the direction that is reflected off
the surface of an object?

Step 1:
Send the camera position to the fragment shader
	char cameraPosFS[] = "cameraPosition";
	rockMat->SetVec3(cameraPosFS, controller.GetTransform().Position());

Step 2:
Get the direction from the surface to the camera
	vec3 surfaceToEye = cameraPosition - position;
	
Step 3:
Reflecting this direction off the surface is actually simple,
GLSL has a function called "reflect". We need to reflect
the direction off of an axis (like X, Y, or Z). This axis
will be the per-pixel normal. If you don't understand why,
draw it on paper and it will make sense.
	vec3 reflectedDir = reflect(surfaceToEye, norm);
	
Step 4:
Get the color from the skybox
	vec4 skyColor = texture(cubeMap, reflectedDir);
	
Step 5:
Submit the pixel color
	gl_FragColor = skyColor;
	
How to Improve:
Rather than using an interpolated vertex normal,
try using normals from a normal map 
	(see Reflections-Bumpy Tutorial)
