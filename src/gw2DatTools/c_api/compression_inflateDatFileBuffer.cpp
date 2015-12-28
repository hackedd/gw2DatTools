#include "gw2DatTools/c_api/compression_inflateDatFileBuffer.h"

#include <exception>

#include "gw2DatTools/compression/inflateDatFileBuffer.h"
#include "gw2DatTools/compression/inflateTextureFileBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

GW2DATTOOLS_API uint8_t* GW2DATTOOLS_APIENTRY compression_inflateDatFileBuffer(const uint32_t iInputSize, uint8_t* iInputTab, uint32_t* ioOutputSize, uint8_t* ioOutputTab)
{
    if (ioOutputSize == nullptr)
    {
        printf("GW2DATTOOLS_C_API(compression_inflateDatFileBuffer): ioOutputSize is NULL.");
        return NULL;
    }

    try
    {
        return gw2dt::compression::inflateDatFileBuffer(iInputSize, iInputTab, *ioOutputSize, ioOutputTab == NULL ? nullptr : ioOutputTab);
    }
    catch(std::exception& iException)
    {
        printf("GW2DATTOOLS_C_API(compression_inflateDatFileBuffer): %s", iException.what());
        return NULL;
    }
}

GW2DATTOOLS_API uint8_t* GW2DATTOOLS_APIENTRY compression_inflateTextureBlockBuffer(uint16_t iWidth, uint16_t iHeight, uint32_t iFormatFourCc, uint32_t iInputSize, const uint8_t* iInputTab, uint32_t* ioOutputSizePtr, uint8_t* ioOutputTab)
{
    try
    {
        uint32_t ioOutputSize = *ioOutputSizePtr;
        uint8_t* ret = gw2dt::compression::inflateTextureBlockBuffer(iWidth, iHeight, iFormatFourCc, iInputSize, iInputTab, ioOutputSize, ioOutputTab);
        *ioOutputSizePtr = ioOutputSize;
        return ret;
    }
    catch(std::exception& iException)
    {
        printf("GW2DATTOOLS_C_API(compression_inflateTextureBlockBuffer): %s", iException.what());
        return NULL;
    }
}

GW2DATTOOLS_API uint8_t* GW2DATTOOLS_APIENTRY compression_inflateTextureFileBuffer(uint32_t iInputSize, const uint8_t* iInputTab,  uint32_t* ioOutputSizePtr, uint8_t* ioOutputTab)
{
    try
    {
        uint32_t ioOutputSize = *ioOutputSizePtr;
        uint8_t* ret = gw2dt::compression::inflateTextureFileBuffer(iInputSize, iInputTab, ioOutputSize, ioOutputTab);
        *ioOutputSizePtr = ioOutputSize;
        return ret;
    }
    catch(std::exception& iException)
    {
        printf("GW2DATTOOLS_C_API(compression_inflateTextureFileBuffer): %s", iException.what());
        return NULL;
    }
}

#ifdef __cplusplus
}
#endif
