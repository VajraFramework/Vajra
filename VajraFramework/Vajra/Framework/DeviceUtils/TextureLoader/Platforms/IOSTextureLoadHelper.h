#ifndef IOS_TEXTURE_LOAD_HELPER_H
#define IOS_TEXTURE_LOAD_HELPER_H

enum COLOR_TYPE {
    COLOR_TYPE_PNG_GRAY,
    COLOR_TYPE_PNG_GRAY_ALPHA,
    COLOR_TYPE_PNG_RGB,
    COLOR_TYPE_PNG_RGB_ALPHA,
};

bool loadPngImage(const char *name, int &outWidth, int &outHeight, bool &outHasAlpha, int &colorType, GLubyte **outData);

#endif // IOS_TEXTURE_LOAD_HELPER_H
