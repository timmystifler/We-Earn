#include <stdio.h>
#include <string.h>
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcMdApi.h"
#include "error_def.h"
#include "md_spi.h"
#include "trade_spi.h"

const char *gs_md_front_addr[] = {"tcp://180.168.146.187:10013" //16-9
                          , "tcp://180.168.146.187:10011" //real time
                          , "tcp://180.168.146.187:10010"
                          , "tcp://180.168.146.187:10012"
};

const char *gs_td_front_addr[] = {"tcp://180.168.146.187:10030"
                          , "tcp://180.168.146.187:10001"
                          , "tcp://180.168.146.187:10000"
                          , "tcp://218.202.237.33:10002"
};

int main()
{
	int ret = 0;
	// set true to use udp
	CThostFtdcMdApi *md_api = CThostFtdcMdApi::CreateFtdcMdApi("./output", false);
	if (!md_api) 
	{
		printf("failed to create md api\n");
		return -1;
	}
	int server_num = 0;
	MD_SPI *md_spi = new MD_SPI;
	md_api->RegisterSpi(md_spi);
	md_spi->RegisterApiHandle(md_api);
	md_api->RegisterFront((char *)gs_md_front_addr[server_num]);
	md_api->Init();

    CThostFtdcTraderApi *td_api = CThostFtdcTraderApi::CreateFtdcTraderApi("./output");
    if (!td_api)
    {
        printf("failed to create td api\n");
        return -1;
    }
    TD_SPI *td_spi = new TD_SPI();
    td_api->RegisterSpi(td_spi);
    td_spi->RegisterApiHandle(td_api);
    td_api->RegisterFront((char *)gs_td_front_addr[server_num]);
    td_api->SubscribePrivateTopic(THOST_TERT_QUICK);
    td_api->SubscribePublicTopic(THOST_TERT_QUICK);
    td_api->Init();

	md_api->Join();
    td_api->Join();

	return 0;
}

