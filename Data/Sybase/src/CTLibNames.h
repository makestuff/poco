#ifndef CTLIBNAMES_H
#define CTLIBNAMES_H

#include <string>
#include <ctpublic.h>

namespace Poco {
namespace Data {
namespace Sybase {

std::string resultTypeName(CS_INT resultType);
std::string dataTypeName(CS_INT dataType);

}}}  // namespace

#endif
