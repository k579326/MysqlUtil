

#pragma once

#include <string>
#include <memory>
#include <time.h>
#include "mysql/mysql.h"

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
		
		return *(uint64_t*)bin_.c_str();
	}
	virtual int64_t toInt() const
	{
		if (!IsValidInt())
		{
			throw "";
		}
		
		return *(int64_t*)bin_.c_str();
	}
	virtual double toDouble() const
	{
		if (!IsValidInt())
		{
			throw "";
		}
		
		return *(double*)bin_.c_str();
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
	
	virtual const std::string toMysqlValue() const 
	{
		return bin_;
	}
	
	size_t size() const {
		return bin_.size();
	}
	
	const ValueType valuetype_;
	
protected:

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
	
	IntValue(const void* ptr, size_t len) : value(ptr, len, NumberType) {}
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

	TimeValue(MYSQL_TIME* t) : value(TimeType)
	{
		struct tm tm = { 0 };

		tm.tm_year = t->year - 1900;
		tm.tm_mon = t->month - 1;
		tm.tm_mday = t->day;
		tm.tm_hour = t->hour;
		tm.tm_min = t->minute;
		tm.tm_sec = t->second;

		uint64_t timestamp = (uint64_t)mktime(&tm);
		SetValue(timestamp);
	}

	virtual const std::string toMysqlValue() const override
	{
		struct tm* t = localtime((time_t*)bin_.c_str());	// 不支持32位程序
		MYSQL_TIME mt = { 0 };
		mt.year = t->tm_year + 1900;
		mt.month = t->tm_mon + 1;
		mt.day = t->tm_mday;
		mt.hour = t->tm_hour;
		mt.minute = t->tm_min;
		mt.second = t->tm_sec;
		mt.neg = false;
		mt.time_type = MYSQL_TIMESTAMP_DATETIME;

		std::string str((char*)& mt, sizeof(mt));

		return str;
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

static std::shared_ptr<value> _CreateValuePtr(const void* ptr, size_t len, field_type type)
{
	std::shared_ptr<value> p = nullptr;

	switch (type)
	{
	case ftTiny:
	case ftShort:
	case ftInt24:
	case ftLong:
	case ftLonglong:
	case ftFloat:
	case ftDouble:
		p = std::make_shared<IntValue>(ptr, len);
		break;
	case ftDatetime:
	case ftTimestamp:
		if (len == sizeof(MYSQL_TIME))
		p = std::make_shared<TimeValue>((MYSQL_TIME*)ptr);
		break;
	case ftBlob:
	case ftMediumBlob:
	case ftVarString:
	case ftTinyBlob:
	case ftString:
		p = std::make_shared<StringValue>(ptr, len);
		break;
	default:
		p = nullptr;
	}

	return p;
}


};
