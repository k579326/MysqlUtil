

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
				 Bind(UserInfo.testVersion, IntValue(10))
				 );
	
	si.Execute();
	t_licKey* lk = (t_licKey*)&t_licKey::GetInstance();
	SQLQuery squery(lk);
	squery.SetResultFields(lk->createTime, lk->endTime);
	squery.Execute();
	RECORDS resList1 = squery.GetResult();
	cout << "createTime:" << resList1[0][0].second->toTimeString() << endl;
	cout << "endTime:" << resList1[0][1].second->toTimeString() << endl;

	SQLQuery su(&UserInfo);
    
	su.SetResultFields(UserInfo.createTime, UserInfo.testVersion);
	su.SetCondition(UserInfo.appId == StringValue("11111") || UserInfo.srcId == StringValue("2222"));
	su.Execute();
	RECORDS resList = su.GetResult();

	cout << "createTime:" << resList[0][0].second->toTimeString() << endl;
	cout << "endTime:" << resList[0][1].second->toInt() << endl;

    return 0;
}