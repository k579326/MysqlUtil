


#include "field/valuecheck.h"
#include "field/value.h"
#include "field/field.h"


namespace MysqlUtil
{

Field::Field(field_type type, const std::string& name,  uint8_t index, bool isUnsigned)
		: fieldType_(type), fieldName_(name), index_(index), unsigned_(isUnsigned)
{
	maxSize_ = g_fieldSizeLimit.find(type)->second;
}
	
Field::~Field(){
	
}

//void Field::Init(field_type type, const std::string& name, uint8_t index, bool isUnsigned)
//{
//	fieldType_ = type; 
//	fieldName_ = name;
//	index_ = index;
//	unsigned_ = isUnsigned;
//	maxSize_ = g_fieldSizeLimit.find(type)->second;
//}


field_type Field::Type() const
{
	return fieldType_;
}
std::string Field::Name() const
{
	return fieldName_;
}
uint64_t Field::MaxSize() const
{
	return maxSize_;
}

bool Field::IsUnsigned() const
{
	return unsigned_;
}
	
uint8_t Field::Index() const
{
	return index_;
}




	
};


