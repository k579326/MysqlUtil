

#pragma once

#include <stdint.h>
#include <map>
#include "mysql/mysql.h"
// TINYINT	                    MYSQL_TYPE_TINY	                    signed char
// SMALLINT	                    MYSQL_TYPE_SHORT	                short int
// MEDIUMINT	                MYSQL_TYPE_INT24	                int
// INT, INTEGER                 MYSQL_TYPE_LONG	                    int
// BIGINT	                    MYSQL_TYPE_LONGLONG	                long long int
// FLOAT	                    MYSQL_TYPE_FLOAT	                float
// DOUBLE	                    MYSQL_TYPE_DOUBLE	                double
// DATETIME	                    MYSQL_TYPE_DATETIME	                MYSQL_TIME
// TIMESTAMP	                MYSQL_TYPE_TIMESTAMP	            MYSQL_TIME
// BLOB, TEXT	                MYSQL_TYPE_BLOB	                    char[]
// MEDIUMBLOB, MEDIUMTEXT	    MYSQL_TYPE_MEDIUM_BLOB	            char[]
// VARCHAR, VARBINARY	        MYSQL_TYPE_VAR_STRING	            char[] 
// TINYBLOB, TINYTEXT	        MYSQL_TYPE_TINY_BLOB	            char[]
// CHAR, BINARY	                MYSQL_TYPE_STRING	                char[]


typedef enum : uint8_t
{
    ftTiny      = MYSQL_TYPE_TINY,
    ftShort     = MYSQL_TYPE_SHORT,
    ftInt24     = MYSQL_TYPE_INT24,
    ftLong      = MYSQL_TYPE_LONG,
    ftLonglong  = MYSQL_TYPE_LONGLONG,
    ftFloat     = MYSQL_TYPE_FLOAT,
    ftDouble    = MYSQL_TYPE_DOUBLE,
    ftDatetime  = MYSQL_TYPE_DATETIME,		// 32位程序下，此字段处理会有问题，不建议在32位程序下使用
    ftTimestamp = MYSQL_TYPE_TIMESTAMP,
    ftBlob      = MYSQL_TYPE_BLOB,
    ftMediumBlob = MYSQL_TYPE_MEDIUM_BLOB,
    ftVarString = MYSQL_TYPE_VAR_STRING,
    ftTinyBlob  = MYSQL_TYPE_TINY_BLOB,
    ftString    = MYSQL_TYPE_STRING,

}field_type;

static std::map< std::string, field_type> g_ft_map =
{
    { "TINYINT", field_type::ftTiny },
    { "SMALLINT", field_type::ftShort },
    { "MEDIUMINT", field_type::ftInt24 },
    { "INT", field_type::ftLong },
    { "INTEGER", field_type::ftLong },
    { "BIGINT", field_type::ftLonglong },
    { "FLOAT", field_type::ftFloat },
    { "DOUBLE", field_type::ftDouble },
    { "DATETIME", field_type::ftDatetime },
    { "TIMESTAMP", field_type::ftTimestamp },
    { "BLOB",   field_type::ftBlob },
    { "TEXT", field_type::ftBlob },
    { "MEDIUMBLOB", field_type::ftMediumBlob },
    { "MEDIUMTEXT", field_type::ftMediumBlob },
    { "VARCHAR", field_type::ftVarString },
    { "VARBINARY", field_type::ftVarString },
    { "TINYBLOB", field_type::ftTinyBlob },
    { "TINYTEXT", field_type::ftTinyBlob },
    { "CHAR",   field_type::ftString },
    { "BINARY", field_type::ftString }
};


static const std::map<field_type, std::string> g_fieldTypeName =
{
    { ftTiny,       "ftTiny" },
    { ftShort,      "ftShort" },
    { ftInt24,      "ftInt24" },
    { ftLong,       "ftLong" },
    { ftLonglong,   "ftLonglong" },
    { ftFloat,      "ftFloat" },
    { ftDouble,     "ftDouble" },
    { ftDatetime,   "ftDatetime" },
    { ftTimestamp,  "ftTimestamp" },
    { ftBlob,       "ftBlob" },
    { ftMediumBlob, "ftMediumBlob" },
    { ftVarString,  "ftVarString" },
    { ftTinyBlob,   "ftTinyBlob" },
    { ftString,     "ftString" }
};

// 字段的数据类型最大长度限制
static const std::map<field_type, uint32_t> g_fieldSizeLimit =
{
	{ ftTiny,       1},
    { ftShort,      2},
    { ftInt24,      3},
    { ftLong,       4},
    { ftLonglong,   8},
    { ftFloat,      4},
    { ftDouble,     8},
    { ftDatetime,   8},
    { ftTimestamp,  4},
    { ftBlob,       65535},
    { ftMediumBlob, 16777215},
    { ftVarString,  16777215},
    { ftTinyBlob,   255},
    { ftString,     1}
}; 


