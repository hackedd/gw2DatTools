#include <iostream>
#include <cstdint>

#include <sstream>
#include <fstream>

#include "gw2DatTools/interface/ANDatInterface.h"
#include "gw2DatTools/compression/inflateDatFileBuffer.h"

int main(int argc, char* argv[])
{
    const uint32_t aBufferSize = 1024 * 1024 * 30; // We make the assumption that no file is bigger than 30 M

    auto pANDatInterface = gw2dt::interface::createANDatInterface("D:\\GuildWars2\\Gw2.dat");

    auto aFileRecordVect = pANDatInterface->getFileRecordVect();

    uint8_t* pOriBuffer = new uint8_t[aBufferSize];
    uint8_t* pInfBuffer = new uint8_t[aBufferSize];

    for (auto it = aFileRecordVect.begin(); it != aFileRecordVect.end(); ++it)
    {
        uint32_t aOriSize = aBufferSize;
        pANDatInterface->getBuffer(*it, aOriSize, pOriBuffer);

        std::cout << "Processing File " << it->fileId << std::endl;

        std::ofstream aOFStream;
        std::ostringstream oss;
        oss << "D:\\gw2Unpack\\" << it->fileId;
        aOFStream.open(oss.str().c_str(), std::ios::binary | std::ios::out);

        if (aOriSize == aBufferSize)
        {
            std::cout << "File " << it->fileId << " has a size greater than (or equal to) 30Mo." << std::endl;
        }

        if (it->isCompressed)
        {
            uint32_t aInfSize = aBufferSize;

            try
            {
                gw2dt::compression::inflateDatFileBuffer(aOriSize, pOriBuffer, aInfSize, pInfBuffer);
                aOFStream.write(reinterpret_cast<const char*>(pInfBuffer), aInfSize);
            }
            catch(std::exception& iException)
            {
                std::cout << "File " << it->fileId << " failed to decompress: " << std::string(iException.what()) << std::endl;
            }
        }
        else
        {
            const char *p = reinterpret_cast<const char*>(pOriBuffer);
            uint32_t remaining = aOriSize - 4;
            const uint32_t chunk = 0x10000;

            while (remaining >= chunk)
            {
                aOFStream.write(p, chunk - 4);
                p += chunk;
                remaining -= chunk;
            }

            if (remaining > 0)
                aOFStream.write(p, remaining);
        }

        aOFStream.close();
    }

    delete[] pOriBuffer;
    delete[] pInfBuffer;

    return 0;
};
