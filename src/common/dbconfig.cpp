
#include <iostream>
#include "dbconfig.h"
#include "tinyxml.h"


int load_config(db_config_t& config)
{
	
	string host, user, pwd, dbName;
    short port;
    TiXmlDocument doc;

    if (!doc.LoadFile("./config/remote.xml"))
    {
        cout << "load remote.xml failed!" << endl;
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
	
	
	config.host = host;
	config.user = user; 
	config.pwd = pwd;
	config.dbName = dbName;
    config.port = port;
	
	return 0;
}



