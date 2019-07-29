Documentation Author: Niko Procopi 2019

This tutorial was designed for Visual Studio 2017 / 2019
If the solution does not compile, retarget the solution
to a different version of the Windows SDK. If you do not
have any version of the Windows SDK, it can be installed
from the Visual Studio Installer Tool

Welcome to the Point Lighting Tutorial!
Prerequesites: Basic Lighting

In the previous tutorial, every pixel had a fixed
direction that the light would come from, and we
also had all the lighting variables pre-set in the
shader. In this tutorial, we will be adding a few variables
to the fragment shader:
	vec3 pointLightPosition = vec3(-3, 0, -3);
	vec3 pointLightAttenuation = vec3(3, 1, 0);
	vec4 pointLightColor = vec4(0, 1, 1, 1);
	float pointLightRange = 5;
We have the position of the light, and the range
of the light. The first thing we have to do is check
to see if a pixel on the screen is within range of the light.
To do this, we need to know where in the world the pixel is,
we need to know the X, Y, Z position of the pixel in the world.

To find this, we go back to the vertex shader to add some 
variables. First we set the vertex shader to give the
world-position of each vertex to the rasterizer:
	out vec3 position;
To get the position of the vertex in the world,
we multiply the vertex by the model matrix, that's it:
	vec4 worldPosition = worldMatrix * vec4(in_position, 1);
	position = vec3(worldPosition);
	
The rasterizer interpolates these values, in the same way 
how it interpolates vertex colors, or texture coordinates.
This will allow the fragment shader to get the position of
each pixel in the world. The fragment shader needs to get
this variable from the rasterizer:
	in vec3 position;
	
Now that we have the position of the pixel in the world, we
need the direction from the light, to the pixel, so that we
can use the direction for NdotL. Getting the direction is easy:
	vec3 lightDirection = pointLightPosition - position;
	
By subtracting one position from the other, we get direction.
We can get the length of this direction, to get the distance between
the light and the pixel:
	length(lightDirection)
The distance from the light will make the brightness of the light, on 
that particular pixel, vary between bright and dark. We can adjust it by
dividing the range, which we set to 5

Finally, we use attenuation, to make sure that a light will not exceed
a certain range. Attenuation is a number between 0 and 1. Attenuation
will be zero if a pixel is outside the range of the light, and within
the range of the light, attenuation gets closer to 1 as distance from
the light decreases.

To get the final light value, we multiply the color of the light
by the NdotL, and then we multiply that by attenuation. Then we
throw the light value into the formula with ambient light and the 
color from the texture (that was done in basic lighting), and you're done,
you have a point light

