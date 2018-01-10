#include "trade_spi.h"
#include <string.h>
#include <stdio.h>

void TD_SPI::ReqUserLogin()
{
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, "9999");
    strcpy(req.UserID, "110238");
    strcpy(req.Password, "lhc997");
    static int req_id = 0;
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

void TD_SPI::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void TD_SPI::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void TD_SPI::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void TD_SPI::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
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

