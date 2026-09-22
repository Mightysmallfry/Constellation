#ifndef CONSTELLATION_IPARSER_H
#define CONSTELLATION_IPARSER_H

#include <string>
#include <vector>

#include "Geometry/SurfaceMesh.h"
#include "Parsers/ParseResults.h"

namespace constellation
{
    class IParser
    {
    public:
        virtual ~IParser() = default;
        
        virtual bool CanParse(const std::string& filePath) const = 0;
        virtual ParseResult Parse(const std::string& filepath) = 0;
    };

}

#endif