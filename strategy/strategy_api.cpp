#include "strategy_api.h"
#include "md_mgr.h"
#include <string>
#include <stdio.h>
#include "trade_spi.h"
#include "login_account.h"

extern TD_SPI *td_spi;

//strategy for open
void _strategy_on_open(std::string instrument_id)
{
    return ;
    //get history md data
    const MD_VEC *vec = md_mgr_find_data(instrument_id);
    if (!vec) 
    {
        printf("no find any md data\n");
        return;
    }
    
    const MD_VEC &cur_vec = *vec;
    // at least 5 tick data to run open strategy
    if (cur_vec.size() < 5) return;

    size_t size = cur_vec.size();
    
    if (cur_vec[size - 1].LastPrice > cur_vec[size - 2].LastPrice
     && cur_vec[size - 1].BidVolume1 > cur_vec[size - 1].AskVolume1)
    {
        printf("buy!!!!\n");
        td_spi->ReqBuyOrderInsert(instrument_id.c_str(), THOST_FTDC_OF_Open, cur_vec[size - 1].AskPrice1, 1, LOGIN_ACCOUNT::account().order_ref_add_and_get());
    }

    if (cur_vec[size - 1].LastPrice < cur_vec[size - 2].LastPrice 
     && cur_vec[size - 1].BidVolume1 < cur_vec[size - 1].AskVolume1)
    {
        printf("sell!!!!\n");
        td_spi->ReqSellOrderInsert(instrument_id.c_str(), THOST_FTDC_OF_Open, cur_vec[size - 1].BidPrice1, 1, LOGIN_ACCOUNT::account().order_ref_add_and_get());
    }
}

//strategy for close
void _strategy_on_close(std::string instrument_id)
{
    //
    //add tick manager
}

void run_strategy(std::string instrument_id)
{
    _strategy_on_open(instrument_id);

    _strategy_on_close(instrument_id);
}
