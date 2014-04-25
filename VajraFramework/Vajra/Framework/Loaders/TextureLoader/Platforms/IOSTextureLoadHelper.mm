#ifdef PLATFORM_IOS

#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Loaders/TextureLoader/TextureLoader.h"
#include "Vajra/Framework/Loaders/TextureLoader/Platforms/IOSTextureLoadHelper.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

#include <UIKit/UIKit.h>

bool loadPngImage(const char *path, int &outWidth, int &outHeight, bool &outHasAlpha, int &colorType, GLubyte **outData) {
    
    // TODO [Implement] actually use texture name:
    // std::string tempImageName = "pictures/square.png";
    
    NSString* pathNSString = [NSString stringWithUTF8String:path];
    UIImage* uiImage = [UIImage imageWithContentsOfFile:pathNSString];
    if (uiImage != nullptr) {
        CGImageRef cgImage = uiImage.CGImage;
        //
        outWidth = (int)CGImageGetWidth(cgImage);
        outHeight = (int)CGImageGetHeight(cgImage);
		std::size_t bytesPerPixel = CGImageGetBitsPerPixel(cgImage) / 8;
        
        // TODO [Implement] alpha
        outHasAlpha = true;
        
        // TODO [Implement] color space
        // CGColorSpaceRef colorSpaceRef = CGImageGetColorSpace(cgImage);
        colorType = COLOR_TYPE_PNG_RGB_ALPHA;
        
        CFDataRef m_imageData = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
		
		std::size_t numBytes = sizeof(GLubyte) * outWidth * outHeight * bytesPerPixel;
		*outData = (GLubyte*)malloc(numBytes);
		memcpy(*outData, CFDataGetBytePtr(m_imageData), numBytes);
        
		uiImage = nil;
        
		CFRelease(m_imageData);
		
        return true;
    }
    return false;
}

#endif // PLATFORM_IOS
