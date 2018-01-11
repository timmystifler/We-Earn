#ifndef __MD_MGR_H__
#define __MD_MGR_H__

#include "ThostFtdcUserApiStruct.h"
#include <map>
#include <string>
#include <vector>

typedef std::vector<CThostFtdcDepthMarketDataField> MD_VEC;

typedef std::map<std::string, MD_VEC> MD_MAP;
typedef std::map<std::string, int> TICK_MAP;

struct MD_MGR
{
	MD_MAP   _map;
    TICK_MAP _tick;
};

int md_mgr_init();

void md_mgr_insert_data(CThostFtdcDepthMarketDataField& data);
int md_mgr_find_tick(std::string instrument_id);
const MD_VEC *md_mgr_find_data(std::string instrument_id);



#endif
