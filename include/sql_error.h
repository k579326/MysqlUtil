
#pragma once


namespace MysqlUtil
{

enum SQLERR
{
	SQL_OK =					0x00000000,

	SQL_ERR_FIELD_LENGTH =		0x00000101,
	SQL_ERR_FIELD_VALUE =		0x00000102,

	SQL_ERR_EMPTY =				0x00010001,
	SQL_ERR_NO_TABLE =			0x00010002,
};



};

