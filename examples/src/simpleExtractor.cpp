#include <iostream>
#include <cstdint>

#include <sstream>
#include <fstream>

#include "gw2DatTools/interface/ANDatInterface.h"
#include "gw2DatTools/compression/inflateDatFileBuffer.h"

// We make the assumption that no file is bigger than 100 M
static const uint32_t aBufferSize = 1024 * 1024 * 100;

bool extractFile(const std::string & outputDir, const std::unique_ptr<gw2dt::interface::ANDatInterface> & pANDatInterface, const gw2dt::interface::ANDatInterface::FileRecord & aFileRecord)
{
    uint32_t aOriSize = aBufferSize;
    std::ofstream aOFStream;
    std::ostringstream oss;

    uint8_t* pOriBuffer = new uint8_t[aBufferSize];

    std::cerr << "Processing File " << aFileRecord.fileId << std::endl;

    pANDatInterface->getBuffer(aFileRecord, aOriSize, pOriBuffer);

    if (aOriSize == aBufferSize)
    {
        std::cerr << "File " << aFileRecord.fileId << " has a size greater than (or equal to) " << (aBufferSize / 1024 / 1024) << "Mo." << std::endl;
        delete[] pOriBuffer;
        return false;
    }

    std::cerr << "Size: " << aFileRecord.size << "; compressed: " << aFileRecord.isCompressed << std::endl;

    oss << outputDir << aFileRecord.fileId << ".raw";
    aOFStream.open(oss.str().c_str(), std::ios::binary | std::ios::out);

    if (!aOFStream.is_open())
    {
        std::cerr << "Unable to open " << oss.str() << std::endl;
        delete[] pOriBuffer;
        return false;
    }

    if (aFileRecord.isCompressed)
    {
        uint32_t aInfSize = aBufferSize;
        uint8_t* pInfBuffer = new uint8_t[aBufferSize];

        try
        {
            gw2dt::compression::inflateDatFileBuffer(aOriSize, pOriBuffer, aInfSize, pInfBuffer);
            aOFStream.write(reinterpret_cast<const char*>(pInfBuffer), aInfSize);
        }
        catch(std::exception& iException)
        {
            std::cerr << "File " << aFileRecord.fileId << " failed to decompress: " << std::string(iException.what()) << std::endl;
        }

        delete[] pInfBuffer;
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

    delete[] pOriBuffer;

    return true;
}

int main(int argc, char* argv[])
{
    std::string outputDir;

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " DATFILE [OUTDIR [FILEID...]]" << std::endl;
        return 1;
    }

    if (argc > 2)
    {
        outputDir = argv[2];
        if (*outputDir.rbegin() != '/' && *outputDir.rbegin() != '\\')
            outputDir.append("/");
    }

    auto pANDatInterface = gw2dt::interface::createANDatInterface(argv[1]);

    if (argc > 3)
    {
        uint32_t fileId;
        for (int i = 3; i < argc; i++)
        {
            std::istringstream(argv[i]) >> fileId;
            auto aFileRecord = pANDatInterface->getFileRecordForFileId(fileId);
            extractFile(outputDir, pANDatInterface, aFileRecord);
        }
    }
    else
    {
        auto aFileRecordVect = pANDatInterface->getFileRecordVect();
        for (auto it = aFileRecordVect.begin(); it != aFileRecordVect.end(); ++it)
        {
            extractFile(outputDir, pANDatInterface, *it);
        }
    }

    return 0;
}
