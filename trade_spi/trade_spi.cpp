#include "trade_spi.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include "account_mgr.h"
#include "login_account.h"
#include "instrument.h"

static int req_id = 0;

static ACCOUNT_MGR *gs_td_account = NULL;
static LOGIN_ACCOUNT *gs_login = NULL;

static char *itoa(int num)
{
    static char buf[100];
    sprintf(buf, "%d", num);
    return buf;
}

void TD_SPI::ReqUserLogin()
{
    gs_login = &LOGIN_ACCOUNT::account();

    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, gs_login->broker_id);
    strcpy(req.UserID, gs_login->investor_id);
    strcpy(req.Password, gs_login->now_passwd);

    int ret = _api->ReqUserLogin(&req, ++req_id);
    if (ret != 0)
    {
        printf("send req for login success\n");
    }
}

void TD_SPI::OnFrontConnected()
{
    ReqUserLogin();
}

void TD_SPI::RegisterApiHandle(CThostFtdcTraderApi *api)
{
    _api = api;
}

void TD_SPI::ReqPasswordUpdate()
{
    CThostFtdcUserPasswordUpdateField req;
    strcpy(req.BrokerID, gs_login->broker_id);
    strcpy(req.UserID, gs_login->investor_id);
    strcpy(req.OldPassword, gs_login->old_passwd);
    strcpy(req.NewPassword, gs_login->now_passwd);
    
    int ret = _api->ReqUserPasswordUpdate(&req, ++req_id);
    if (ret == 0)
    {
        printf("send update passwd success\n");
    }
}

void TD_SPI::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pRspInfo && pRspInfo->ErrorID == 0)
    {
        printf("modify passwd success\n");
    }
}


void TD_SPI::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pRspInfo && pRspInfo->ErrorID == 0)
    {
        printf("login trade success, req id = %d\n", nRequestID);
        int order_ref = atoi(pRspUserLogin->MaxOrderRef);
        gs_login->set_front_id(pRspUserLogin->FrontID);
        gs_login->set_session_id(pRspUserLogin->SessionID);
        gs_login->set_order_ref(order_ref);
        ReqSettlementConfirmInfo();
        ReqAccountInfo();
    }
    else
    {
        if (pRspInfo)
        {
            printf("login trade failed, ret[%d]\n", pRspInfo->ErrorID);
            std::cout << pRspInfo->ErrorMsg << std::endl;
            ReqPasswordUpdate();
        }
    }
}

void TD_SPI::ReqAccountInfo()
{
    CThostFtdcQryTradingAccountField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, gs_login->broker_id);
    strcpy(req.InvestorID, gs_login->investor_id);

    int ret = 0;
    while(true)
    {
        ret = _api->ReqQryTradingAccount(&req, ++req_id);
        if (!IsFlowControl(ret)) 
        {
            if (ret == 0) printf("send req account success\n");
            else
            {
                printf("send req account failed, ret = %d\n", ret);
            }
            break;
        }
        else
        {
            printf("flow control wait\n");
            sleep(1);
        }

    }
}

bool TD_SPI::IsFlowControl(int ret)
{
    return ret == -2 || ret == -3;
}

void TD_SPI::ReqSettlementConfirmInfo()
{
    CThostFtdcSettlementInfoConfirmField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, gs_login->broker_id);
    strcpy(req.InvestorID, gs_login->investor_id);
    int ret = _api->ReqSettlementInfoConfirm(&req, ++req_id);
    if (ret != 0)
    {
        printf("send req settlement error\n");
    }
    else
    {
        printf("send req settlement success\n");
    }
}

void TD_SPI::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pRspInfo && pRspInfo->ErrorID == 0)
    {
        printf("recv settlement success\n");
    }
    else
    {
        printf("recv settlement confirm failed\n");
    }
}

void TD_SPI::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

//imba pRspInfo always equal to zero in this func
void TD_SPI::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (!pTradingAccount) return;
    std::cout << pTradingAccount->Available << std::endl;

    gs_td_account = ACCOUNT_MGR::get_mgr();
    if (!gs_td_account || !pTradingAccount) 
    {
        printf("rsp for trading account error\n");
    }
    else
    {
        //trade module init finish prepare for work
        gs_td_account->init(pTradingAccount);
        ReqPosition();
        working = true;
    }
}

void TD_SPI::ReqBuyOrderInsert(const char *instrument_id, char comb_offset, double price, int volume, int order_ref)
{
    if (!working) return;

    CThostFtdcInputOrderField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, gs_login->broker_id);
    strcpy(req.InvestorID, gs_login->investor_id);
    strcpy(req.InstrumentID, instrument_id);
    strcpy(req.UserID, gs_login->investor_id);
    strcpy(req.OrderRef, itoa(order_ref));
    req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    req.Direction = THOST_FTDC_D_Buy;
    req.CombOffsetFlag[0] = comb_offset;
    req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    req.LimitPrice = price;
    req.VolumeTotalOriginal = 1;
    req.TimeCondition = THOST_FTDC_TC_GFD;
    req.VolumeCondition = THOST_FTDC_VC_AV;
    req.MinVolume = 1;
    req.ContingentCondition = THOST_FTDC_CC_Immediately;
    req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    req.IsAutoSuspend = 0;
    req.UserForceClose = 0;

    int ret = _api->ReqOrderInsert(&req, ++req_id);
    if (ret != 0)
    {
        printf("failed to send order req\n");
    }
    else
    {
        printf("send order success\n");
    }
}

void TD_SPI::ReqSellOrderInsert(const char *instrument_id, char comb_offset, double price, int volume, int order_ref)
{
    if (!working) return;

    CThostFtdcInputOrderField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, gs_login->broker_id);
    strcpy(req.InvestorID, gs_login->investor_id);
    strcpy(req.InstrumentID, instrument_id);
    strcpy(req.OrderRef, itoa(order_ref));
    req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    req.Direction = THOST_FTDC_D_Buy;
    req.CombOffsetFlag[0] = comb_offset;
    req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    req.LimitPrice = price;
    req.VolumeTotalOriginal = 1;
    req.TimeCondition = THOST_FTDC_TC_GFD;
    req.VolumeCondition = THOST_FTDC_VC_AV;
    req.MinVolume = 1;
    req.ContingentCondition = THOST_FTDC_CC_Immediately;
    req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    req.IsAutoSuspend = 0;
    req.UserForceClose = 0;

    int ret = _api->ReqOrderInsert(&req, ++req_id);
    if (ret != 0)
    {
        printf("failed to send order req\n");
    }

}

void TD_SPI::ReqPosition()
{
    int ret = 0;
    for (int i = 0; i < INSTRUMENT_MGR::mgr().num; ++i)
    {
        CThostFtdcQryInvestorPositionField req;
        strcpy(req.BrokerID, gs_login->broker_id);
        strcpy(req.InvestorID, gs_login->investor_id);
        strcpy(req.InstrumentID, INSTRUMENT_MGR::mgr().instrument_id[i]);
        while(true)
        {
            ret = _api->ReqQryInvestorPosition(&req, ++req_id);
            if (IsFlowControl(ret)) 
            {
                usleep(500);
                continue;
            }

            if (ret != 0)
            {
                printf("send req position failed, ret = %d\n", ret);
            }
            break;
        }
    }

    printf("send req position success\n");
}

void TD_SPI::ReqDetailPosition()
{
    for (int i = 0; i < INSTRUMENT_MGR::mgr().num; ++i)
    {
        CThostFtdcQryInvestorPositionDetailField req;
        strcpy(req.BrokerID, gs_login->broker_id);
        strcpy(req.InvestorID, gs_login->investor_id);
        strcpy(req.InstrumentID, INSTRUMENT_MGR::mgr().instrument_id[i]);
        _api->ReqQryInvestorPositionDetail(&req, ++req_id);
    }
    printf("send query detail position\n");
}

void TD_SPI::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    printf("recv investor position\n");
    if (!pInvestorPosition) return;
    if (pRspInfo && pRspInfo->ErrorID == 0)
    {
        printf("");
    }

}

void TD_SPI::OnRspQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pInvestorPosition, int nRequestID)
{
    printf("recv investor position\n");
    if (!pInvestorPosition) return;


}

void TD_SPI::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pRspInfo && pRspInfo->ErrorID == 0)
    {
        printf("send order insert success\n");
    }
    else
    {
        if (pRspInfo) printf("send order insert failed, ret[%d]", pRspInfo->ErrorID);
    }
}

bool TD_SPI::IsMyOrder(CThostFtdcOrderField *pOrder)
{
    if (!pOrder) return false;

    if (pOrder->SessionID != gs_login->session_id
     || pOrder->FrontID != gs_login->front_id) return false;

    return true;
}
void TD_SPI::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
    if (!pOrder) return;
    if (!IsMyOrder(pOrder)) return;

    printf("order status:%c\n", pOrder->OrderStatus);
    printf("order status:%s\n", pOrder->StatusMsg);
}


void TD_SPI::OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void TD_SPI::OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void TD_SPI::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void TD_SPI::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void TD_SPI::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void TD_SPI::OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void TD_SPI::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void TD_SPI::OnFrontDisconnected(int nReason)
{
}

void TD_SPI::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder)
{
}

void TD_SPI::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
}

void TD_SPI::OnRtnQuote(CThostFtdcQuoteField *pQuote)
{
}

void TD_SPI::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
}

