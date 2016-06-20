#include <string>
#include <sstream>
#include <cstddef>
#include "CTLibNames.h"

namespace {
	const char* const resultTypeNameArray[] =
	{
		"CS_ROW_RESULT",
		"CS_CURSOR_RESULT",
		"CS_PARAM_RESULT",
		"CS_STATUS_RESULT",
		"CS_MSG_RESULT",
		"CS_COMPUTE_RESULT",
		"CS_CMD_DONE",
		"CS_CMD_SUCCEED",
		"CS_CMD_FAIL",
		"CS_ROWFMT_RESULT",
		"CS_COMPUTEFMT_RESULT",
		"CS_DESCRIBE_RESULT"
	};
	const std::size_t resultTypeNameCount = sizeof(resultTypeNameArray)/sizeof(*resultTypeNameArray);

	const char* const dataTypeNameArray[] =
	{
		"CS_ILLEGAL_TYPE",
		"CS_CHAR_TYPE",
		"CS_BINARY_TYPE",
		"CS_LONGCHAR_TYPE",
		"CS_LONGBINARY_TYPE",
		"CS_TEXT_TYPE",
		"CS_IMAGE_TYPE",
		"CS_TINYINT_TYPE",
		"CS_SMALLINT_TYPE",
		"CS_INT_TYPE",
		"CS_REAL_TYPE",
		"CS_FLOAT_TYPE",
		"CS_BIT_TYPE",
		"CS_DATETIME_TYPE",
		"CS_DATETIME4_TYPE",
		"CS_MONEY_TYPE",
		"CS_MONEY4_TYPE",
		"CS_NUMERIC_TYPE",
		"CS_DECIMAL_TYPE",
		"CS_VARCHAR_TYPE",
		"CS_VARBINARY_TYPE",
		"CS_LONG_TYPE",
		"CS_SENSITIVITY_TYPE",
		"CS_BOUNDARY_TYPE",
		"CS_VOID_TYPE",
		"CS_USHORT_TYPE",
		"CS_UNICHAR_TYPE",
		"CS_BLOB_TYPE",
		"CS_DATE_TYPE",
		"CS_TIME_TYPE",
		"CS_UNITEXT_TYPE",
		"CS_BIGINT_TYPE",
		"CS_USMALLINT_TYPE",
		"CS_UINT_TYPE",
		"CS_UBIGINT_TYPE",
		"CS_XML_TYPE",
		"CS_BIGDATETIME_TYPE",
		"CS_BIGTIME_TYPE",
		"CS_TEXTLOCATOR_TYPE",
		"CS_IMAGELOCATOR_TYPE",
		"CS_UNITEXTLOCATOR_TYPE"
	};
	const std::size_t dataTypeNameCount = sizeof(dataTypeNameArray)/sizeof(*dataTypeNameArray);

	inline std::string outOfRange(CS_INT x)
	{
		std::ostringstream s;
		s << "<out-of-range: " << x << ">";
		return s.str();
	}
}


namespace Poco {
namespace Data {
namespace Sybase {


std::string resultTypeName(const CS_INT resultType)
{
	const CS_INT index = resultType - CS_ROW_RESULT;  // because CS_ROW_RESULT != 0
	if ( index < 0 || index >= resultTypeNameCount )
		return outOfRange(resultType);
	else
		return resultTypeNameArray[index];
}


std::string dataTypeName(const CS_INT dataType)
{
	const CS_INT index = dataType + 1;  // because CS_ILLEGAL_TYPE == -1
	if ( index < 0 || index >= dataTypeNameCount )
		return outOfRange(dataType);
	else
		return dataTypeNameArray[index];
}


}}}  // namespace
