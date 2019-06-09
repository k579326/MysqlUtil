

#pragma once

#include "field/valuecheck.h"
#include "field/field.h"
#include "field/value.h"

using namespace MysqlUtil;

bool CHK::IsNumBerType(field_type t)
{
	switch (t)
	{
	case ftTiny:
	case ftShort:     
	case ftInt24:    
	case ftLong:    
	case ftLonglong:
	case ftFloat: 
	case ftDouble:    
	case ftDatetime:
	case ftTimestamp: 
		return true;
	default: return false;
	}
	
	return false;
}


bool CHK::AssignmentCheck(const Field& fd, const value& v)
{
	switch (fd.Type())
	{
	case ftTiny:
	case ftShort:     
	case ftInt24:    
	case ftLong:    
	case ftLonglong:
	case ftFloat: 
	case ftDouble:    
	case ftDatetime:
	case ftTimestamp:
	{
		if (v.valuetype_ != NumberType && v.valuetype_ != TimeType)
		{
			return false;
		}
		
		uint8_t len = (uint8_t)fd.MaxSize() * 8 - 1;
		if (v.toUInt() > (1ll << len))
		{
			return false;
		}
		return true;
	}
	case ftBlob:
	case ftMediumBlob:
	case ftVarString:
	case ftTinyBlob:
	case ftString:
	{
		if (v.valuetype_ != StringType) return false;
		if (v.size() > fd.MaxSize()) return false;
		return true;
	}
	default:
		return false;
	}

	return false;
}
	
	
	
	
	
	
