

#include "t_data.hpp"
#include "t_datakey.hpp"
#include "t_license.hpp"
#include "t_licKey.hpp"
#include "t_userinfo.hpp"
#include "t_userkey.hpp"
#include "easysql/sqlcommand.h"
#include "easysql/sqloperator.h"
//  SGInsertValue(const string& name, field1, field2, field3);
//  SGGetRecord();

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