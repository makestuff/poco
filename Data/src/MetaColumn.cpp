//
// MetaColumn.cpp
//
// $Id: //poco/Main/Data/src/MetaColumn.cpp#2 $
//
// Library: Data
// Package: DataCore
// Module:  MetaColumn
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/MetaColumn.h"

namespace Poco {
namespace Data {

const char* const g_colDTNames[] = {
	"FDT_BOOL",
	"FDT_INT8",
	"FDT_UINT8",
	"FDT_INT16",
	"FDT_UINT16",
	"FDT_INT32",
	"FDT_UINT32",
	"FDT_INT64",
	"FDT_UINT64",
	"FDT_FLOAT",
	"FDT_DOUBLE",
	"FDT_STRING",
	"FDT_WSTRING",
	"FDT_BLOB",
	"FDT_CLOB",
	"FDT_DATE",
	"FDT_TIME",
	"FDT_TIMESTAMP",
	"FDT_UNKNOWN"
};

MetaColumn::MetaColumn()
{
}


MetaColumn::MetaColumn(std::size_t columnPosition,
	const std::string& rName,
	ColumnDataType columnType,
	std::size_t columnLength,
	std::size_t columnPrecision,
	bool nullable): 
	_name(rName),
	_length(columnLength),
	_precision(columnPrecision),
	_position(columnPosition),
	_type(columnType),
	_nullable(nullable)
{
}


MetaColumn::~MetaColumn()
{
}


std::ostream& operator<<(std::ostream& os, const MetaColumn& mc)
{
	os << "MetaColumn(name=\"" << mc._name << "\", length=" << mc._length << ", type=" << g_colDTNames[mc._type] << ")";
	return os;
}


} } // namespace Poco::Data
