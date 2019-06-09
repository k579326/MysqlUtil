# MysqlUtil
简化数据库操作的C++工具

## 说明
该工具分两部分SQLTOOL和MYSQLUTIL；
SQLTOOL为生成数据库表类的工具，生成的结果为tablename.hpp，如下例中t_data.hpp，t_datakey.hpp等；
MYSQLUTIL为数据库操作接口库，目前提供查询、插入操作，后续继续增加

## 接口使用Demo

该工具目前实现不完善，demo只用于展示接口使用方式

#include "t_data.hpp"		
#include "t_datakey.hpp"
#include "t_license.hpp"
#include "t_licKey.hpp"
#include "t_userinfo.hpp"
#include "t_userkey.hpp"
#include "easysql/sqlcommand.h"
#include "easysql/sqloperator.h"

using namespace MysqlUtil;

int main()
{
    t_userinfo UserInfo = t_userinfo::GetInstance();
   
	SQLInsert si(&UserInfo);
    
	si.SetValues(Bind(UserInfo.appId, StringValue("11111")),
				 Bind(UserInfo.srcId, StringValue("22222")),
				 Bind(UserInfo.testVersion, IntValue(10)));
	
	si.Execute();

	SQLQuery su(&UserInfo);
    
	su.SetResultFields(UserInfo.createTime, UserInfo.testVersion);
	su.SetCondition(UserInfo.appId == StringValue("11111") || UserInfo.srcId == StringValue("2222"));
	su.Execute();
	su.GetResult();

    return 0;
}

