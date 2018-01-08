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
	private:
		CThostFtdcMdApi *_api;
};

#endif
