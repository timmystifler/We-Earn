#include "trade_spi.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

static int req_id = 0;

const char broker_id[] = "9999";
const char user_id[] = "110238";
const char passwd[]  = "lhc199707";

void TD_SPI::ReqUserLogin()
{
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, broker_id);
    strcpy(req.UserID, user_id);
    strcpy(req.Password, passwd);
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
    strcpy(req.BrokerID, broker_id);
    strcpy(req.UserID, user_id);
    strcpy(req.OldPassword, "lhc997");
    strcpy(req.NewPassword, "lhc199707");
    
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
    strcpy(req.BrokerID, broker_id);
    strcpy(req.InvestorID, user_id);

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
            sleep(5);
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
    strcpy(req.BrokerID, broker_id);
    strcpy(req.InvestorID, user_id);
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
    std::cout << pTradingAccount->Available << std::endl;
}

void TD_SPI::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void TD_SPI::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
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

void TD_SPI::OnRtnOrder(CThostFtdcOrderField *pOrder)
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

