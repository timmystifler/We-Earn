#include "instrument.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

INSTRUMENT_MGR INSTRUMENT_MGR::_mgr;

void INSTRUMENT_MGR::add(const char *id)
{
    instrument_id[num] = (char *)malloc(strlen(id));
    if (!instrument_id[num]) 
    {
        printf("alloc for instrument id failed\n");
        return;
    }
    
    strcpy(instrument_id[num], id);
    ++num;
}

INSTRUMENT_MGR::INSTRUMENT_MGR()
{
    memset(&instrument_id, 0x0, sizeof(instrument_id));
    num = 0;
    
    add("ru1805\0");
    add("cu1803\0");
}


