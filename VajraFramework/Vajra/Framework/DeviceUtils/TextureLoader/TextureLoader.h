#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include "Vajra/Utilities/OpenGLIncludes.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>

GLint loadGLTextureFromPNG(const char *name, GLubyte** outTextureBytes);

#endif // TEXTURELOADER_H
