#define _GLIBCXX_USE_CXX11_ABI 0

#include <stdio.h>
#include <string.h>
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcMdApi.h"
#include "error_def.h"
#include "md_spi.h"
#include "trade_spi.h"
#include "md_mgr.h"

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>


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

TD_SPI *td_spi;



//
// internal functions declare
//

static int log4cpp_init();

//
// main
//

int main()
{
    int ret = 0;

    
    ret = log4cpp_init();
    if (ret != 0)
    {
        printf("log4cpp init error!\n");
        return ret;
    }
    

    ret = md_mgr_init();
    if (ret != 0)
    {
        return ret;
    }

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
    td_spi = new TD_SPI();
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

//
// internal functions
//


int log4cpp_init()
{
    std::string initFileName = "cfg/log4cpp.properties";
    log4cpp::PropertyConfigurator::configure(initFileName);

    log4cpp::Category& root = log4cpp::Category::getRoot();

    log4cpp::Category& sub1 = 
    	log4cpp::Category::getInstance(std::string("sub1"));

    log4cpp::Category& sub2 = 
    	log4cpp::Category::getInstance(std::string("sub1.sub2"));

    /*
    root.warn("Storm is coming");

    sub1.debug("Received storm warning");
    sub1.info("Closing all hatches");

    sub2.debug("Hiding solar panels");
    sub2.error("Solar panels are blocked");
    sub2.debug("Applying protective shield");
    sub2.warn("Unfolding protective shield");
    sub2.info("Solar panels are shielded");

    sub1.info("All hatches closed");

    root.info("Ready for storm.");
    */

    log4cpp::Category::shutdown();

    return 0;
}



