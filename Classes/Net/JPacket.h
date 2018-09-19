#ifndef __JPACKET_H__
#define __JPACKET_H__

#include "json.h"

struct Header_Packet {
    //unsigned char   sig[2];
	unsigned int    length;
	//unsigned int    info;
};

class JPacket
{
public:
    JPacket();
    virtual ~JPacket();

    std::string& tostring();
    Json::Value& tojson();

    void end();
    int parse(std::string&);
    int sefe_check();
    
    static std::string codeStr(const std::string& d);
    static std::string decodeStr(const std::string& d);
private:
    struct Header_Packet    header;

public:
    std::string             str;
    Json::Value             val;
    Json::Reader            reader;
    int                     len;
};

#endif
