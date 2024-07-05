#include <zxcode.hh>
#include <zlib.h>

std::optional<std::string> compress(const std::string &input)
{
    uLongf destSize = compressBound(input.size());
    std::string output(destSize, '\0');

    int result = compress2((Bytef *)output.data(), &destSize,
                           (const Bytef *)input.data(), input.size(),
                           Z_BEST_COMPRESSION);

    if (result != Z_OK)
    {
        return std::nullopt;
    }
    else
    {
        output.resize(destSize);
        return output;
    }
}

std::optional<std::string> decompress(const std::string &input)
{
    uLongf destSize = compressBound(input.size());
    std::string output(destSize, '\0');

    int result = uncompress((Bytef *)output.data(), &destSize,
                            (const Bytef *)input.data(), input.size());

    if (result != Z_OK)
    {
        return std::nullopt;
    }
    else
    {
        output.resize(destSize);
        return output;
    }
}