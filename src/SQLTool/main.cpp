

#include "readDB.h"
#include <memory>
#include <iostream>
#include "tinyxml.h"

using namespace std;
using namespace MysqlUtil;


int main()
{
    string host, user, pwd, dbName;
    short port;
    TiXmlDocument doc;

    if (!doc.LoadFile("./config/remote.xml"))
    {
        cout << "load remote.xml failed£¡" << endl;
        return -1;
    }

    TiXmlNode* node = doc.FirstChild("mysqlconfig");
    if (!node)
    {
        cout << "remote.xml is bad!" << endl;
        return -1;
    }

    // host
    TiXmlNode* HostNode = node->FirstChild("HOST");
    if (!HostNode)
    {
        cout << "field HOST is bad!" << endl;
        return -1;
    }
    //HostNode->
    host = HostNode->FirstChild()->Value();

    // user
    TiXmlNode* UserNode = node->FirstChild("USER");
    if (!UserNode)
    {
        cout << "field USER is bad!" << endl;
        return -1;
    }
    user = UserNode->FirstChild()->Value();

    // port
    TiXmlNode* PortNode = node->FirstChild("PORT");
    if (!PortNode)
    {
        cout << "field PORT is bad!" << endl;
        return -1;
    }
    port = (short)strtol(PortNode->FirstChild()->Value(), nullptr, 10);

    // pwd
    TiXmlNode* PwdNode = node->FirstChild("PWD");
    if (!PwdNode)
    {
        cout << "field PWD is bad!" << endl;
        return -1;
    }
    pwd = PwdNode->FirstChild()->Value();

    // db name
    TiXmlNode* DbNameNode = node->FirstChild("DBNAME");
    if (!DbNameNode)
    {
        cout << "field DBNAME is bad!" << endl;
        return -1;
    }
    dbName = DbNameNode->FirstChild()->Value();

    try
    {
        reader r(host, user, pwd, port, dbName);
        r.FetchTableListFromDb();
        r.FetchAllTableInfo();
        if (0 != r.Generate("./config"))
        {
            cout << "gen failed!" << endl;
            return -1;
        }

        cout << "success!" << endl;
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}
