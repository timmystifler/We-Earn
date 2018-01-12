#include "md_spi.h"
#include <stdio.h>
#include <string.h>
#include "md_mgr.h"
#include "strategy_api.h"
#include "instrument.h"

//front address connected
void MD_SPI::OnFrontConnected()
{
	printf("front connected success, try login\n");
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, "9999");
	strcpy(req.UserID, "110238");
	strcpy(req.Password, "lhc997");

	int login_seq = 0;
	int ret = _api->ReqUserLogin(&req, login_seq);
	if (ret == 0)
	{
		printf("send login md success\n");
	}
	else
	{
		printf("send login md failed, ret[%d]\n", ret);
	}
}

//user login rsp
void MD_SPI::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
			CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo && pRspInfo->ErrorID == 0)
	{
		printf("login md success, req id[%d]\n", nRequestID);
		ReqSubscribeMarketData();
	}
	else
	{
		if (pRspInfo)
		{
			printf("login md faild. ret[%d]", pRspInfo->ErrorID);
		}
	}
}

void MD_SPI::ReqSubscribeMarketData()
{
	_api->SubscribeMarketData(INSTRUMENT_MGR::mgr().instrument_id, INSTRUMENT_MGR::mgr().num);
}

void MD_SPI::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo && pRspInfo->ErrorID == 0)
	{
		printf("sub market data success, req id[%d]\n", nRequestID);
		printf("instrument field:%s\n", pSpecificInstrument->InstrumentID);
	}
	else
	{
		if (pRspInfo) printf("rsp error, ret[%d]\n", pRspInfo->ErrorID);
	}
}

void MD_SPI::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if (pDepthMarketData)
	{
        PrintMarketData(*pDepthMarketData);

		md_mgr_insert_data(*pDepthMarketData);
        run_strategy(pDepthMarketData->InstrumentID);
	}
}

void MD_SPI::PrintMarketData(CThostFtdcDepthMarketDataField &pDepthMarketData)
{
    bool print_on = true;

    if (print_on)
    {
		printf("TradingDay:%s\n", pDepthMarketData.TradingDay);
		printf("InstrumentID:%s\n", pDepthMarketData.InstrumentID);
		printf("LastPrice:%lf\n", pDepthMarketData.LastPrice);
		printf("PreSettlementPrice:%lf\n", pDepthMarketData.PreSettlementPrice);
		printf("PreClosePrice:%lf\n", pDepthMarketData.PreClosePrice);
		printf("PreOpenInterest:%lf\n", pDepthMarketData.PreOpenInterest);
		printf("OpenPrice:%lf\n", pDepthMarketData.OpenPrice);
		printf("HighestPrice:%lf\n", pDepthMarketData.HighestPrice);
		printf("LowestPrice:%lf\n", pDepthMarketData.LowestPrice);
		printf("Volume:%d\n", pDepthMarketData.Volume);
		printf("Turnover:%lf\n", pDepthMarketData.Turnover);
		printf("OpenInterest:%lf\n", pDepthMarketData.OpenInterest);
		//printf("ClosePrice:%lf\n", pDepthMarketData.ClosePrice);
		//printf("SettlementPrice:%lf\n", pDepthMarketData.SettlementPrice);
		printf("BidPrice1:%lf\n", pDepthMarketData.BidPrice1);
		printf("BidVolume1:%d\n", pDepthMarketData.BidVolume1);
		//printf("BidPrice2:%lf\n", pDepthMarketData.BidPrice2);
		//printf("BidVolume2:%lf\n", pDepthMarketData.BidVolume2);
		printf("AskPrice1:%lf\n", pDepthMarketData.AskPrice1);
		printf("AskVolume1:%d\n", pDepthMarketData.AskVolume1);
		//printf("AskPrice2:%lf\n", pDepthMarketData.AskPrice2);
		//printf("AskVolume2:%d\n", pDepthMarketData.AskVolume2);
        printf("-------------------------------------\n");
        printf("-------------------------------------\n");
        printf("-------------------------------------\n");
    }
}
