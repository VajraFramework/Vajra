#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include "Vajra/Utilities/OpenGLIncludes.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>

#define USING_MIPMAPS

bool textureMustUseMipmaps(std::string imagePath);
GLuint loadGLTextureFromPNG(const char *imagePath, GLubyte** outTextureBytes, bool useMipmapping);

#endif // TEXTURELOADER_H
