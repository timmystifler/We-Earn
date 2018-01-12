#ifndef _POSITION_MGR_H_
#define _POSITION_MGR_H_


#include <string>
#include <map>


enum E_POSITION_TYPE
{
    E_POSITION_TYPE_BUY    = 0,
    E_POSITION_TYPE_SELL   = 1,
};

struct POSITION_INFO
{

    int  volume;
    int  tick;
    E_POSITION_TYPE type;

    POSITION_INFO (): volume(0), tick(0) {}
};

//use order ref id as key;
typedef std::map<int , POSITION_INFO> POSITION;

class POSITION_MGR
{
    public:
        POSITION _position;

        void add(int order_refer, bool buy, int volume);
        void del(int order_refer, int volume);

        POSITION_MGR() { } 
        static POSITION_MGR _mgr;
        static POSITION_MGR & mgr() { return _mgr; }
};

#endif
