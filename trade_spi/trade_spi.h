#ifndef _TRADE_SPI_H_
#define _TRADE_SPI_H_

#include <stdlib.h>
#include "ThostFtdcTraderApi.h"

class TD_SPI: public CThostFtdcTraderSpi
{
    public:
        TD_SPI() : _api(NULL), working(false) {}

        void OnFrontConnected();

        void RegisterApiHandle();

        void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
        void OnRspQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID);

        void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnFrontDisconnected(int nReason);

        void OnHeartBeatWarning(int nTimeLapse){}

        void OnRtnOrder(CThostFtdcOrderField *pOrder);

        void OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder);

        void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);

        void OnRtnQuote(CThostFtdcQuoteField *pQuote);

        void OnRtnTrade(CThostFtdcTradeField *pTrade);

        void RegisterApiHandle(CThostFtdcTraderApi *api);
        void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void ReqBuyOrderInsert(const char *instrument_id, char comb_offset, double price, int volume, int order_ref);

        void ReqSellOrderInsert(const char *instrument_id, char comb_offset, double price, int volume, int order_ref);
    private:
        void ReqUserLogin();

        void ReqSettlementConfirmInfo();

        void ReqAccountInfo();

        void ReqPosition();
        void ReqDetailPosition();

        bool IsFlowControl(int ret);
        bool IsMsgOK(CThostFtdcRspInfoField *pRspInfo);

        void HandleAllTraded(CThostFtdcOrderField &order);

        void ReqPasswordUpdate();

        bool IsMyOrder(CThostFtdcOrderField *pOrder);
    
    private:
        CThostFtdcTraderApi *_api;

        bool working;
};




#endif
