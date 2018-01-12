#ifndef _POSITION_MGR_H_
#define _POSITION_MGR_H_

#include <string>
#include <map>

enum E_POSITION_DIRECTION
{
    E_POSITION_DIR_LONG    = 0,
    E_POSITION_DIR_SHORT   = 1,
};

struct POSITION_INFO
{
    int  volume;
    int  tick;
    int  dir;
    std::string instrument_id;
    
    POSITION_INFO (): volume(0), tick(0), dir(0) {}
};

//use order ref id as key;
typedef std::map<int , POSITION_INFO> POSITION;
typedef std::map<int,  POSITION_INFO>::iterator pos_iterator;

class POSITION_MGR
{
    public:
        POSITION _position;

        void add(int order_refer, std::string &instrument_id, int dir, int volume, int tick);
        void del(int order_refer, int volume);
        POSITION_INFO *find(int order_refer);
        POSITION_INFO *find(int order_refer, pos_iterator &it);

        POSITION_MGR() { } 
        static POSITION_MGR _mgr;
        static POSITION_MGR & mgr() { return _mgr; }
};

#endif
