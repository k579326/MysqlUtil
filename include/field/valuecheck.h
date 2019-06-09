
#pragma once


#include "field.h"
#include "value.h"

using namespace MysqlUtil;

class CHK
{
public:
	static bool IsNumBerType(field_type t);
	static bool AssignmentCheck(const Field& fd, const value& v);
};	
	