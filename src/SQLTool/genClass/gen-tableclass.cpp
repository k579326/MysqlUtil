
#include <stdio.h>
#include <iostream>
#include <memory>
#include "gen-tableclass.h"

namespace MysqlUtil
{

    static const std::string cs_tablename_template = "TABLENAME";
    static const std::string cs_label_1 = "Label(1)";
    static const std::string cs_label_2 = "Label(2)";
	static const std::string cs_label_3 = "Label(3)";

    static void _repalce_tablename(std::string& str, const std::string tableName)
    {
        size_t pos = str.find(cs_tablename_template);
        while (pos != std::string::npos)
        {
            str.replace(pos, cs_tablename_template.size(), tableName);
            pos = str.find(cs_tablename_template, pos);
        }

        return;
    }

    // 字段声明
    static void _repalce_label_1(std::string& str, const std::string& declares)
    {
        size_t pos_start = str.find(cs_label_1);
        size_t pos_end = str.find('\n', pos_start);

        str.replace(pos_start, pos_end - pos_start, declares);

        return;
    }

    // 字段初始化
    static void _repalce_label_2(std::string& str, const std::string& initString)
    {
        size_t pos_start = str.find(cs_label_2);
        size_t pos_end = str.find('\n', pos_start);

        str.replace(pos_start, pos_end - pos_start, initString);

        return;
    }

	// 字段初始化
	static void _repalce_label_3(std::string& str, const std::string& pushString)
	{
		size_t pos_start = str.find(cs_label_3);
		size_t pos_end = str.find('\n', pos_start);

		str.replace(pos_start, pos_end - pos_start, pushString);

		return;
	}



    static std::string _make_fields_init(const std::vector<Field>& tableStructure)
    {
        std::string initstring;
		
        for (size_t i = 0; i < tableStructure.size(); i++)
        {
			Field element = tableStructure[i];

            std::string ftn = g_fieldTypeName.find(element.Type())->second;
            std::string fi = "\t\tFIELDNAME(FIELDTYPENAME, \"FIELDNAME\", INDEX, ISUNSIGNED),\n";
            
            size_t pos_start = fi.find("FIELDNAME");
            while (pos_start != std::string::npos)
            {
                fi.replace(pos_start, strlen("FIELDNAME"), element.Name());
                pos_start = fi.find("FIELDNAME");
            }

            pos_start = fi.find("FIELDTYPENAME");
            fi.replace(pos_start, strlen("FIELDTYPENAME"), ftn);

            char numbuf[256];
            sprintf(numbuf, "%d", (int)i);
            pos_start = fi.find("INDEX");
            fi.replace(pos_start, strlen("INDEX"), numbuf);

			std::string strbool = "false";
			pos_start = fi.find("ISUNSIGNED");
			if (element.IsUnsigned())
			{
				strbool = "true";
			}
			fi.replace(pos_start, strlen("ISUNSIGNED"), strbool);

            initstring += fi;
        }
		initstring.erase(initstring.end() - 2); // 去掉最后一个初始化表达式的逗号
        return initstring;
    }

    static std::string _make_fields_declare(const std::vector<Field>& tableStructure)
    {
        std::string declares;

        for (const auto& element : tableStructure)
        {
            std::string fc = "\t const Field " + element.Name() + ";\n";
            declares += fc;
        }

        return declares;
    }

	static std::string _save_fields_to_vector(const std::vector<Field>& tableStructure)
	{
		std::string saveToVect;
		for (const auto& element : tableStructure)
		{
			std::string dopush = "\t\tfieldList_.push_back(" + element.Name() + ");";
			saveToVect += (dopush + "\n");
		}
		
		return saveToVect;
	}


    int GenTableClass(const std::string& path, const std::string& tableName, const std::vector<Field>& tableStructure)
    {
        std::string tableClassString;
        std::shared_ptr<FILE> ptrInFile, ptrOutFile;
        unsigned char buf[4096];
        size_t readlen = 4096;          // 注意template文件内容长度不要超过4096
        FILE* fp = NULL;
        std::string file = path + "/tableclass.template";
        fp = fopen(file.c_str(), "rb");
        if (fp == NULL)
        {
            return -1;
        }

        ptrInFile = std::shared_ptr<FILE>(fp, [](FILE* p)->void { fclose(p); p = NULL; });
        
        readlen = fread(buf, 1, 4096, ptrInFile.get());
        tableClassString.assign((char*)buf, readlen);

        if (tableClassString.empty())
        {
            return -1;
        }

        _repalce_tablename(tableClassString, tableName);
        
        std::string declares = _make_fields_declare(tableStructure);
        _repalce_label_1(tableClassString, declares);

        std::string initfields = _make_fields_init(tableStructure);
        _repalce_label_2(tableClassString, initfields);

		std::string pushString = _save_fields_to_vector(tableStructure);
		_repalce_label_3(tableClassString, pushString);

        std::string classname = path + "/" + tableName + ".hpp";
        fp = fopen(classname.c_str(), "wb");
        ptrOutFile = std::shared_ptr<FILE>(fp, [](FILE* p)->void { fclose(p); p = NULL; });

        fwrite(tableClassString.c_str(), 1, tableClassString.size(), ptrOutFile.get());

        return 0;
    }







}