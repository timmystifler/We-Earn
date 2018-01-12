#include "position_mgr.h"
#include <stdio.h>
#include <string>

POSITION_MGR POSITION_MGR::_mgr;

POSITION_INFO *POSITION_MGR::find(int order_refer)
{
    std::map<int, POSITION_INFO>::iterator it = _position.begin();

    it = _position.find(order_refer);

    if (it == _position.end()) return NULL;

    return &it->second;
}

POSITION_INFO *POSITION_MGR::find(int order_refer, pos_iterator & it_out)
{
    it_out = _position.find(order_refer);

    if (it_out == _position.end()) return NULL;


    return &it_out->second;
}

void POSITION_MGR::add(int order_refer, std::string &instrument_id, int dir, int volume, int tick)
{
    POSITION_INFO *position = find(order_refer);

    if (!position) 
    {
        POSITION_INFO new_position;
        new_position.tick = tick;
        new_position.dir  = dir;
        new_position.volume = volume;
        new_position.instrument_id = instrument_id;
        _position[order_refer] = new_position;
    }
    else
    {
        position->volume += volume;
    }
}

void POSITION_MGR::del(int order_refer, int volume)
{
    pos_iterator it;
    POSITION_INFO *position = find(order_refer, it);
    if (!position)
    {
        printf("fatal not find any position in mgr\n");
        return;
    }
    position->volume -= volume;

    if (position->volume == 0)
    {
        _position.erase(it);
    }
}
