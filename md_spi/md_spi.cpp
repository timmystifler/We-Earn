#include "md_spi.h"
#include <stdio.h>
#include <string.h>

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

char *gs_instrument_id[] = {"CU1205", "CU1206", "CU1803"};
void MD_SPI::ReqSubscribeMarketData()
{
	_api->SubscribeMarketData(gs_instrument_id, sizeof(gs_instrument_id) / sizeof(gs_instrument_id[0]));
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
		printf("TradingDay:%s\n", pDepthMarketData->TradingDay);
		printf("InstrumentID:%s\n", pDepthMarketData->InstrumentID);
		printf("AskVolume1:%d\n", pDepthMarketData->AskVolume1);
		printf("BidPrice1:%lf\n", pDepthMarketData->BidPrice1);

		//todo manuzhang
	}
}
