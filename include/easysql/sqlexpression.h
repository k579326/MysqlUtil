

#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <list>
#include <assert.h>
#include "field/field.h"
#include "field/value.h"


using namespace std;

namespace MysqlUtil
{
using ValueContainer = list<pair<Field, shared_ptr<value>>>;
	
class IExpression
{
protected:
	IExpression(const std::string& exp, const pair<Field, shared_ptr<value>>& fv)
		: exp_(exp)
	{
		container_.push_back(fv);
	}
	IExpression(const string& exp, const ValueContainer& fvs)
		: exp_(exp)
	{
		container_ = fvs;
	}
	IExpression() { }
	virtual ~IExpression() {}
	

public:	

	virtual void clear(){
		exp_ = "";
		container_.clear();
	}
	virtual bool empty() const {
		return container_.empty();
	}
	std::string exp_;
	ValueContainer container_;
};


class AssignmentExp : public IExpression
{
public:
	AssignmentExp(const pair<Field, std::shared_ptr<value>>& fv)
		: IExpression("?", fv)
	{		
		
	}
	
	AssignmentExp(const AssignmentExp& exp)
		: IExpression(exp.exp_, exp.container_)
	{
	}

	AssignmentExp& operator&& (const AssignmentExp& ae)
	{
		exp_ = exp_ + "," + ae.exp_;
		std::for_each(ae.container_.begin(), ae.container_.end(), [&](const pair<Field, shared_ptr<value>>& e)->void { container_.push_back(e); });
		return *this;
	}
};
	
class CompareExp : public IExpression
{
public:
	CompareExp() {}
	CompareExp(const std::string& opr, const pair<Field, std::shared_ptr<value>>& fv)
		: IExpression(fv.first.Name() + opr + "?", fv)
	{
	}
	
	CompareExp(const CompareExp& e) 
		: IExpression(e.exp_, e.container_){
	}
	
	CompareExp& operator&& (const CompareExp& ae)
	{
		exp_ = "(" + exp_ + " and " + ae.exp_ + ")";
		std::for_each(ae.container_.begin(), ae.container_.end(), [&](const pair<Field, shared_ptr<value>>& e)->void { container_.push_back(e); });
		return *this;
	}
	
	CompareExp& operator|| (const CompareExp& ae)
	{
		exp_ = "(" + exp_ + " or " + ae.exp_ + ")";
		std::for_each(ae.container_.begin(), ae.container_.end(), [&](const pair<Field, shared_ptr<value>>& e)->void { container_.push_back(e); });
		return *this;
	}
};
	
	
};


