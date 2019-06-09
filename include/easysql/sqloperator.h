

#pragma once

#include "sqlexpression.h"

namespace MysqlUtil
{

CompareExp operator== (const Field& fd, const value& v);
CompareExp operator!= (const Field& fd, const value& v);
CompareExp operator>= (const Field& fd, const value& v);
CompareExp operator<= (const Field& fd, const value& v);
CompareExp operator>  (const Field& fd, const value& v);
CompareExp operator<  (const Field& fd, const value& v);
AssignmentExp Bind(const Field& fd, const value& v);

};
