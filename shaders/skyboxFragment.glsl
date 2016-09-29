/*
Title: Sky Boxes
File Name: skyboxFragment.glsl
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

// This is what a cubemap texture sampler looks like:
uniform samplerCube cubeMap;

void main(void)
{
	// Output texture the same way you would with uvs
	// OpenGL has a special sampler that takes in a direction, and gets the pixel color from the cube map for you.
	// This is a nice feature, because if we didn't have it, we would have 6 separate textures, and this shader would be a mess.
	gl_FragColor = texture(cubeMap, -position);

	// We have to invert the position otherwise everything will be upside-down
	// I'm not entirely sure why this is. I think it might just be the way OpenGL samples from the texture.
}