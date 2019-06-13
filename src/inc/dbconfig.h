

#pragma once


#include <string>
using namespace std;


typedef struct
{
	string host;
	string user; 
	string pwd;
	string dbName;
    short port;
}db_config_t;


int load_config(db_config_t& config);