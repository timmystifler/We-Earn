#ifndef _ACCOUNT_MGR_H_
#define _ACCOUNT_MGR_H_

#include "ThostFtdcTraderApi.h"

class TRADE_ACCOUNT
{
    public:
        TRADE_ACCOUNT();
        double CurrMargin;
        double Available;
};

// store account info for trade and strategy
class ACCOUNT_MGR
{
    public:
        ACCOUNT_MGR ();
        void init(CThostFtdcTradingAccountField *account_info);
        static ACCOUNT_MGR * get_mgr() {return &gs_account_mgr;}

        void on_change();
        void dump_account_info();
    private:
        TRADE_ACCOUNT account;

        static ACCOUNT_MGR gs_account_mgr;
};

#endif
