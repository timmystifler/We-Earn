#include "md_spi.h"
#include <stdio.h>
#include <string.h>

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
		printf("user login md success\n");
	}
	else
	{
		printf("user login md failed, ret[%d]\n", ret);
	}
}
