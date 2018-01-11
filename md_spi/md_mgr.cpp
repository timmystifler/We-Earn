#include <stdio.h>
#include "md_mgr.h"

static MD_MGR gs_md_mgr;

static MD_VEC* _md_mgr_get_md_vec_by_id(std::string id);
static int* _md_mgr_get_tick_by_id(std::string id);

int md_mgr_init()
{
    printf("[INFO] md mgr init success!\n");
    
    return 0;
}

void md_mgr_insert_data(CThostFtdcDepthMarketDataField& data)
{
    std::string id = data.InstrumentID;

    MD_VEC* vec = _md_mgr_get_md_vec_by_id(id);
    if (vec == NULL)
    {
        printf("[ERROR] md mgr find vec error, id %s\n", id.c_str());
        return;
    }

    vec->push_back(data);

    int *tick = _md_mgr_get_tick_by_id(id);
    if (!tick) printf("tick not find\n");

    ++(*tick);

    return;    
}


MD_VEC* _md_mgr_get_md_vec_by_id(std::string id)
{

    MD_MAP::iterator iter;

    iter = gs_md_mgr._map.find(id);

    if (iter != gs_md_mgr._map.end())
    {
        return &iter->second;
    }
    else
    {
        MD_VEC _vec;
        gs_md_mgr._map.insert(std::pair<std::string, MD_VEC>(id, _vec));
        
        iter = gs_md_mgr._map.find(id);
        if (iter != gs_md_mgr._map.end())
        {
            return &iter->second;
        }
    }

    return NULL;
}

int* _md_mgr_get_tick_by_id(std::string id)
{
    TICK_MAP::iterator iter;

    iter = gs_md_mgr._tick.find(id);

    if (iter != gs_md_mgr._tick.end()) return &iter->second;
    else
    {
        int tick = 0;
        gs_md_mgr._tick[id] = tick;

        iter = gs_md_mgr._tick.find(id);
        return &iter->second;
    }

    return NULL;
}

const MD_VEC *md_mgr_find_data(std::string instrument_id)
{
    return _md_mgr_get_md_vec_by_id(instrument_id);
}

int md_mgr_find_tick(std::string instrument_id)
{
    int *tick =  _md_mgr_get_tick_by_id(instrument_id);
    if (!tick) return 0;
    return *tick;
}
