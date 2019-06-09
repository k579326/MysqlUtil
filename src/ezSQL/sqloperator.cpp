

#include "field/field.h"
#include "easysql/sqloperator.h"
#include "field/valuecheck.h"


namespace MysqlUtil
{

CompareExp operator== (const Field& fd, const value& v){
	std::shared_ptr<value> ptr = _CreateValuePtr(v);
	std::pair<Field, std::shared_ptr<value>> fv(fd, ptr);
	
	return CompareExp("=", fv);
}
CompareExp operator!= (const Field& fd, const value& v){
	std::shared_ptr<value> ptr = _CreateValuePtr(v);
	std::pair<Field, std::shared_ptr<value>> fv(fd, ptr);
	
	return CompareExp("!=", fv);
}
CompareExp operator>= (const Field& fd, const value& v){
	std::shared_ptr<value> ptr = _CreateValuePtr(v);
	std::pair<Field, std::shared_ptr<value>> fv(fd, ptr);
	
	return CompareExp(">=", fv);
}
CompareExp operator<= (const Field& fd, const value& v){
	
	std::shared_ptr<value> ptr = _CreateValuePtr(v);
	std::pair<Field, std::shared_ptr<value>> fv(fd, ptr);
	
	return CompareExp("<=", fv);
}

CompareExp operator> (const Field& fd, const value& v){
	std::shared_ptr<value> ptr = _CreateValuePtr(v);
	std::pair<Field, std::shared_ptr<value>> fv(fd, ptr);
	
	return CompareExp(">", fv);
}

CompareExp operator< (const Field& fd, const value& v){
	std::shared_ptr<value> ptr = _CreateValuePtr(v);
	std::pair<Field, std::shared_ptr<value>> fv(fd, ptr);
	
	return CompareExp("<", fv);
}

AssignmentExp Bind(const Field& fd, const value& v)
{
	if (!CHK::AssignmentCheck(fd, v))
	{
		throw "";
	}

	std::shared_ptr<value> ptr = _CreateValuePtr(v);

	return AssignmentExp(std::make_pair(fd, ptr));
}


};
