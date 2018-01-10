#include "md_mgr.h"

static MD_MGR gs_md_mgr;

//
// 内部函数声明
//

MD_VEC* _md_mgr_get_md_vec_by_id(std::string id);


//
// 接口实现
//

int md_mgr_init()
{
    printf("[INFO] md mgr init success!");
    
	return 0;
}

void md_mgr_insert_data(CThostFtdcDepthMarketDataField& data)
{
	std::string id = data.InstrumentID;

    MD_VEC* vec = _md_mgr_get_md_vec_by_id(id);
    if (vec == NULL)
    {
        printf("[ERROR] md mgr find vec error, id %s", id.c_str());
        return;
    }

    vec->push_back(data);

    return;    
}


//
// 内部函数实现
//

MD_VEC* _md_mgr_get_md_vec_by_id(std::string id)
{

	MD_MAP::iterator iter;

	iter = gs_md_mgr._map.find(id);

    if (iter != gs_md_mgr._map.end())
    {
        return &iter->second;
    }

    return NULL;
}


