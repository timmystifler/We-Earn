#include "account_mgr.h"

ACCOUNT_MGR ACCOUNT_MGR::gs_account_mgr;

TRADE_ACCOUNT::TRADE_ACCOUNT() : CurrMargin(0), Available(0){}

void ACCOUNT_MGR::init(CThostFtdcTradingAccountField *account_info)
{
    if (!account_info) return;
    account.CurrMargin = account_info->CurrMargin;
    account.Available = account_info->Available;
}

void ACCOUNT_MGR::on_change()
{
}

void ACCOUNT_MGR::dump_account_info()
{
}

ACCOUNT_MGR::ACCOUNT_MGR()
{
}
