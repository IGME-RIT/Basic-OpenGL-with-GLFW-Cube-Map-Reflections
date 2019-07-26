/*
Title: Specular Maps
File Name: cubeMap.cpp
Copyright ? 2016, 2019
Author: David Erbelding, Niko Procopi
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

#include "cubeMap.h"


//filePaths.push_back("../assets/skyboxLeft.png");
//filePaths.push_back("../assets/skyboxRight.png");
//filePaths.push_back("../assets/skyboxBottom.png");
//filePaths.push_back("../assets/skyboxTop.png");
//filePaths.push_back("../assets/skyboxBack.png");
//filePaths.push_back("../assets/skyboxFront.png");

// use the same sampler for every texture
static GLuint sampler;

CubeMap::CubeMap(std::vector<char*> filePaths)
{
    // Create an OpenGL texture.
    glGenTextures(1, &m_cubeMap);

	// build the sampler if it does not exist
	if (sampler == 0)
		glGenSamplers(1, &sampler);

    // Bind our texture as a cube map.
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);

	// Bind our sampler.
	glBindSampler(m_cubeMap, sampler);

    // Fill our openGL side texture object.
    for (GLuint i = 0; i < filePaths.size(); i++)
    {
        // Load the face and convet it to 32 bit.
        FIBITMAP* bitmap = FreeImage_ConvertTo32Bits(FreeImage_Load(FreeImage_GetFileType(filePaths[i]), filePaths[i]));

        // Load the image into OpenGL memory.
        // GL_TEXTURE_CUBE_MAP_POSITIVE_X indicates the side of the skybox. Incrementing that value gives us the constant used by each side.
        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap),
            0, GL_BGRA, GL_UNSIGNED_BYTE, static_cast<void*>(FreeImage_GetBits(bitmap)));

        // We can unload the image now.
        FreeImage_Unload(bitmap);
    }

	// Get the maximum anisotropy that our GPU supports
	GLfloat maxAniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);

	// set the sampler parameters
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

    // These prevent artifacts from appearing near the edges.
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
	// This creates the mipmap layers
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // Unbind
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubeMap::~CubeMap()
{
    glDeleteTextures(1, &m_cubeMap);
}

void CubeMap::IncRefCount()
{
    m_refCount++;
}

void CubeMap::DecRefCount()
{
    m_refCount--;
    if (m_refCount == 0)
    {
        delete this;
    }
}

GLuint CubeMap::GetGLCubeMap()
{
    return m_cubeMap;
}
