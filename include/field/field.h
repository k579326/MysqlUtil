
#pragma once

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <exception>
#include <memory>

#include "fd_type.hpp"
//#include "easysql/sqlexpression.h"


namespace MysqlUtil
{

class Field
{
public:
	Field(field_type type, const std::string& name, uint8_t index, bool isUnsigned = false);
	virtual ~Field();
	
	//void Init(field_type type, const std::string& name, uint8_t index, bool isUnsigned = false);
	
	field_type Type() const;
	std::string Name() const;
	uint64_t MaxSize() const;
	bool IsUnsigned() const;
	uint8_t Index() const;
	
	//AssignmentExp operator= (const value& v);
private:
	
	bool unsigned_;
	uint32_t  maxSize_;
	field_type fieldType_;
	std::string fieldName_;
	uint8_t index_;
};

};



