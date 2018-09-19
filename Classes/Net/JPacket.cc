#include "ZJH.h"

void xorfunc(std::string &nString)
{
	const int KEY = 13;
	int strLen = (nString.length());
	char *cString = (char*)(nString.c_str());
	
	for (int i = 0; i < strLen; i++)
	{
		*(cString + i) = (*(cString + i) ^ KEY);
	}
}

JPacket::JPacket()
{
}

JPacket::~JPacket()
{
}

std::string& JPacket::tostring()
{
    return str;
}

Json::Value& JPacket::tojson()
{
    return val;
}

void JPacket::end()
{
    //header[0] = 0xFA;
    //header[1] = 0xF1;
	string out = val.toStyledString().c_str();
	xorfunc(out);
    header.length = out.length();
	
    str.clear();
    str.append((const char *)&header, sizeof(struct Header_Packet));
    
    str.append(out);
}
std::string JPacket::codeStr(const std::string& str)
{
    string out =str;
    xorfunc(out);
     struct Header_Packet d;
    d.length =out.length();
    
    std::string ret;
    ret.clear();
    ret.append((const char *)&d, sizeof(struct Header_Packet));
    ret.append(out);
    return ret;
}
std::string JPacket::decodeStr(const std::string& str)
{
    string out =str;
    xorfunc(out);
    return out;
}

int JPacket::parse(std::string &str)
{
	xorfunc(str);
    //CCLOG("recvData: [%s]\n", str.c_str());
    if (!reader.parse(str, val))
    {
        CCLOG("recvDataStyled: error [%s]\n", str.c_str());
        return -1;
    }
	
	CCLOG("recvDataStyled: [%s]\n", val.toStyledString().c_str());
    return 0;
}

int JPacket::sefe_check()
{
    if (!val["cmd"].isNumeric()) return -2;

    int cmd = val["cmd"].asInt();
	
    switch (cmd)
    {
        
    }

    return cmd;
}

