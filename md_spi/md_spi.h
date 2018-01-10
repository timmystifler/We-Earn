#ifndef _MD_SPI_
#define _MD_SPI_

#include "ThostFtdcMdApi.h"


class MD_SPI : public CThostFtdcMdSpi
{
	public:
		void OnFrontConnected();
		void OnFrontDisconnected(int nReason) {}

		// may never call this function
		void OnHeartBeatWarning(int nTimeLapse) {}
		void RegisterApiHandle(CThostFtdcMdApi *api) { _api = api; }
		void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
						CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		void ReqUserLogout();
		void ReqSubscribeMarketData();

		void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, 
				CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		// so far no need this.
		void ReqSubscribeQuoteRsp();
        void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
	private:
		CThostFtdcMdApi *_api;
};

#endif
