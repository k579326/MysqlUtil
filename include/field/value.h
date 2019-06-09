

#pragma once

#include <string>
#include <memory>
#include <time.h>

namespace MysqlUtil
{

enum ValueType
{
	NumberType,
	StringType,
	TimeType,
};



class value
{
protected:	
	
	value(const void* binptr, size_t len, ValueType t) : valuetype_(t)
	{
		bin_.assign((const char*)binptr, len);
	}
	
	value(const std::string& bin, ValueType t) : valuetype_(t)
	{
		bin_ = bin;
	}
	
	// for TimeValue
	value(ValueType t) : valuetype_(t)
	{
		bin_ = "";
	}
	// for TimeValue
	void SetValue(uint64_t t)
	{
		bin_.assign((const char*)&t, sizeof(t));
	}
	
	virtual ~value() {}
	
public:
	virtual uint64_t toUInt() const
	{
		if (!IsValidInt())
		{
			throw "";
		}
		
		return *(uint64_t*)GetPtr();
	}
	virtual int64_t toInt() const
	{
		if (!IsValidInt())
		{
			throw "";
		}
		
		return *(int64_t*)GetPtr();
	}
	virtual double toDouble() const
	{
		if (!IsValidInt())
		{
			throw "";
		}
		
		return *(double*)GetPtr();
	}
	
	virtual std::string toString() const
	{
		if (valuetype_ != StringType)
		{
			throw "";
		}
		return bin_;
	}
	virtual std::string toTimeString() const
	{
		if (valuetype_ != TimeType)
		{
			throw "";
		}
		return "";
	}
	
	const void* GetPtr() const 
	{
		return (const void*)bin_.c_str();
	}
	
	size_t size() const {
		return bin_.size();
	}
	
	const ValueType valuetype_;
	
private:

	bool IsValidInt() const
	{
		if (valuetype_ != NumberType && valuetype_ != TimeType)
		{
			return false;
		}
		if (size() != 8)
		{
			return false;
		}
		return true;
	}

	std::string bin_;
};


class IntValue : public value
{
public:
	IntValue(uint32_t v) : value(NumberType)
	{
		uint64_t vll = v;
		SetValue(vll);
	}
	IntValue(int32_t v) : value(NumberType)
	{
		int64_t vll = v;
		SetValue(vll);
	}
	
	IntValue(int64_t v) : value(&v, sizeof(v), NumberType) {}
	IntValue(uint64_t v) : value(&v, sizeof(v), NumberType) {}
	IntValue(double v) : value(&v, sizeof(v), NumberType) {}
};



class StringValue : public value
{
public:
	StringValue(const char* str) : value(str, strlen(str), StringType) {}
	StringValue(const void* ptr, size_t len) : value(ptr, len, StringType) {}
	StringValue(const std::string& str) : value(str, StringType) {}
};

class TimeValue : public value
{
public:
	TimeValue(time_t t) : value(TimeType)
	{
		uint64_t lt = t;
		SetValue(lt);
	}
	
	TimeValue(const std::string& timeString) : value(TimeType)
	{
		int ret = 0;
		uint64_t timestamp;
		struct tm t = { 0 };
		ret = sscanf(timeString.c_str(), "%d-%d-%d %d:%d:%d", &t.tm_year, &t.tm_mon, &t.tm_mday,
					&t.tm_hour, &t.tm_min, &t.tm_sec);
		if (ret != 0)
		{
			throw "";
		}
		
		t.tm_year -= 1900;
		t.tm_mon -= 1;
		
		timestamp = mktime(&t);
		SetValue(timestamp);
	}

	std::string toTimeString() const override
	{
		time_t tstamp = (time_t)toInt();
		struct tm* t = localtime(&tstamp);
		char timestring[64] = "";
		if (!t)
		{
			throw "";
			return "";
		}

		sprintf(timestring, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
				t->tm_hour, t->tm_min, t->tm_sec);

		return timestring;
	}
};


static std::shared_ptr<value> _CreateValuePtr(const value& v)
{
	std::shared_ptr<value> p = nullptr;
	if (v.valuetype_ == NumberType)
	{
		p = std::make_shared<IntValue>(dynamic_cast<const IntValue&>(v));
	}
	else if (v.valuetype_ == StringType)
	{
		p = std::make_shared<StringValue>(dynamic_cast<const StringValue&>(v));
	}
	else if (v.valuetype_ == TimeType)
	{
		p = std::make_shared<TimeValue>(dynamic_cast<const TimeValue&>(v));
	}
	
	return p;
}




};
