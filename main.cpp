#include <stdio.h>
#include <string.h>
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcMdApi.h"
#include "error_def.h"
#include "md_spi.h"

char gs_ip_addr[] = "tcp://218.202.237.33:10012";

int main()
{
	int ret = 0;
	// set true to use udp
	CThostFtdcMdApi *api = CThostFtdcMdApi::CreateFtdcMdApi("./output", false);
	if (!api) 
	{
		printf("failed to create md api\n");
		return -1;
	}
	
	MD_SPI *spi = new MD_SPI;
	api->RegisterSpi(spi);
	spi->RegisterApiHandle(api);
	api->RegisterFront((char *)gs_ip_addr);
	api->Init();

	printf("aaa");

	api->Join();

	return 0;
}

