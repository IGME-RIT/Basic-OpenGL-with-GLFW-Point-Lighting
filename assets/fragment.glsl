/*
Title: Point Lighting
File Name: fragment.glsl
Copyright ? 2016
Author: David Erbelding
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#version 400 core

in vec3 position;
in vec3 normal;
in vec2 uv;
uniform sampler2D tex;

void main(void)
{
	vec4 ambientLight = vec4(.2, .2, .3, 1);

	// LIGHTS: in practice, these values would be passed to the shader from the cpu, but we're hardcoding them for simplicity.
	// You could even pass them in as an array.

	vec3 pointLightPosition = vec3(-3, 0, -3); // The position of our light in world space
	vec3 pointLightAttenuation = vec3(3, 1, 0); // The attenuation constants a, b, and c for quadratic attenuation. (shown used below)
	vec4 pointLightColor = vec4(0, 1, 1, 1); // The color of the light
	float pointLightRange = 5; // The radius of the light influence
	
	// Subtract the difference between the position of the light and the position of the vertex being lit.
	vec3 lightDirection = pointLightPosition - position;
	
	// Here we get the distance from the light to our pixel position, as a value between 0 and 1
	float distance = length(lightDirection) / pointLightRange;

	// Attenuation is calculated by dividing 1 by a quadratic function ax^2 + bx + c.
	// This makes a curve that falls off with distance.
	float attenuation = 1 / (distance * distance * pointLightAttenuation.x + distance * pointLightAttenuation.y + pointLightAttenuation.z);

	// Calculate ndotl with that light direction
	float ndotl = dot(normalize(lightDirection), normalize(normal));

	// Here we scale our light color by the value of the light on the surface.
	vec4 lightValue = pointLightColor * ndotl * attenuation;

	// Add the ambient light to the light value. (This can put us over 1, so clamp it again)
	lightValue = clamp(lightValue + ambientLight, 0, 1);

	// Finally, sample from the texuture and multiply the value we get by the color of the light.
	gl_FragColor = texture(tex, uv) * lightValue;
}