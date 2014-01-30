#ifndef PLACEHOLDER_RENDERER_H
#define PLACEHOLDER_RENDERER_H

#include "Vajra/Utilities/OpenGLIncludes.h"

#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/type_ptr.hpp"
#include "Libraries/glm/gtx/transform.hpp"
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <map>
#include <string>

////////////////////////////////////////////////////////////////////////////////
// temp
bool setupGraphics(int w, int h);

// Returns false if not enough time has passed since the last frame and no work was done, true otherwise
bool renderFrame();


////////////////////////////////////////////////////////////////////////////////


#endif // PLACEHOLDER_RENDERER_H
