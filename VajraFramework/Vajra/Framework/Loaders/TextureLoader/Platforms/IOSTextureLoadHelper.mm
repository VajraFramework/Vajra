#ifdef PLATFORM_IOS

#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/DeviceUtils/TextureLoader/TextureLoader.h"
#include "Vajra/Framework/DeviceUtils/TextureLoader/Platforms/IOSTextureLoadHelper.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

#include <UIKit/UIKit.h>

CFDataRef m_imageData;

bool loadPngImage(const char *path, int &outWidth, int &outHeight, bool &outHasAlpha, int &colorType, GLubyte **outData) {
    
    // TODO [Implement] actually use texture name:
    // std::string tempImageName = "pictures/square.png";
    
    NSString* pathNSString = [NSString stringWithUTF8String:path];
    UIImage* uiImage = [UIImage imageWithContentsOfFile:pathNSString];
    if (uiImage != nullptr) {
        CGImageRef cgImage = uiImage.CGImage;
        //
        outWidth = CGImageGetWidth(cgImage);
        outHeight = CGImageGetHeight(cgImage);
        
        // TODO [Implement] alpha
        outHasAlpha = true;
        
        // TODO [Implement] color space
        // CGColorSpaceRef colorSpaceRef = CGImageGetColorSpace(cgImage);
        colorType = COLOR_TYPE_PNG_RGB_ALPHA;
        
        m_imageData = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
        
        *outData = (GLubyte*) CFDataGetBytePtr(m_imageData);
        
        return true;
    }
    return false;
}

#endif // PLATFORM_IOS
