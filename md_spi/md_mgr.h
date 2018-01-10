#ifndef __MD_MGR_H__
#define __MD_MGR_H__

#include "ThostFtdcUserApiStruct.h"
#include <map>
#include <string>
#include <vector>

typedef std::vector<CThostFtdcDepthMarketDataField> MD_VEC;

typedef std::map<std::string, MD_VEC> MD_MAP;

struct MD_MGR
{
	MD_MAP _map;
};

int md_mgr_init();

void md_mgr_insert_data(CThostFtdcDepthMarketDataField& data);



#endif