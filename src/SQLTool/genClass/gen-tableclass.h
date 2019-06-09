#pragma once


#include <map>
#include <iostream>
#include <vector>
#include <memory>
#include "field/fd_type.hpp"
#include "field/field.h"


namespace MysqlUtil
{

int GenTableClass(const std::string& path, const std::string& tableName, const std::vector<Field>& tableStructure);

};

