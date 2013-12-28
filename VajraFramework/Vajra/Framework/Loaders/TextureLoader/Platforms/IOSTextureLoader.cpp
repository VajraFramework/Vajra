#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Loaders/TextureLoader/TextureLoader.h"
#include "Vajra/Framework/Loaders/TextureLoader/Platforms/IOSTextureLoadHelper.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

#ifdef PLATFORM_IOS

#include <fstream>
#include <string>
#include <vector>

#define PNGSIGSIZE 8

GLuint loadGLTextureFromPNG(const char *imagePath, GLubyte** outTextureBytes) {
    

    int textureWidth, textureHeight;
    bool textureHasAlpha;
    int colorType;
    bool textureLoadSuccess = loadPngImage(imagePath, textureWidth, textureHeight, textureHasAlpha, colorType, outTextureBytes);
    if (textureLoadSuccess) {
        FRAMEWORK->GetLogger()->dbglog("\nSuccessfully loaded texture");
        FRAMEWORK->GetLogger()->dbglog("\n(w, h): (%d, %d)", textureWidth, textureHeight);
        FRAMEWORK->GetLogger()->dbglog("\nHas alpha: %s", textureHasAlpha? "yes" : "no");
    } else {
        FRAMEWORK->GetLogger()->dbglog("\nFailed to load texture");
    }
    
    GLuint myTexture;

    // make it
    glGenTextures(1, &myTexture);
    checkGlError("glGenTextures");
    // bind it
    glBindTexture(GL_TEXTURE_2D, myTexture);
    checkGlError("glBindTexture");
    // stretch it
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    checkGlError("glTexParameteri");
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    checkGlError("glTexParameteri 2");
    // technologic - I MEAN

    // gl color type
    GLuint glcolours = 0;
    int components = 0;
    if (colorType == COLOR_TYPE_PNG_GRAY) {
        glcolours = GL_LUMINANCE;
        components = 1;
    }
    if (colorType == COLOR_TYPE_PNG_GRAY_ALPHA) {
        glcolours = GL_LUMINANCE_ALPHA;
        components = 2;
    }
    if (colorType == COLOR_TYPE_PNG_RGB) {
        glcolours = GL_RGB;
        components = 3;
    }
    if (colorType == COLOR_TYPE_PNG_RGB_ALPHA) {
        glcolours = GL_RGBA;
        components = 4;
    }
    FRAMEWORK->GetLogger()->dbglog("\nNumComponents in Texture: %d", components);

    // glTexImage2D(GL_TEXTURE_2D, 0, components, textureWidth, textureHeight, 0, glcolours, GL_UNSIGNED_BYTE, *outTextureBytes);
    glTexImage2D(GL_TEXTURE_2D, 0, glcolours, textureWidth, textureHeight, 0, glcolours, GL_UNSIGNED_BYTE, *outTextureBytes);
    checkGlError("glTexImage2D");


    FRAMEWORK->GetLogger()->dbglog("\nmyTexture (GL Handle): %d", myTexture);

    return myTexture;
}

#endif // PLATFORM_IOS

