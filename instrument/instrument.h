#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_


#define MAX_INSTRUMENT_NUM 1024

class INSTRUMENT_MGR
{
    public:
        char *instrument_id[MAX_INSTRUMENT_NUM];
        int num;

        INSTRUMENT_MGR();
        void add(const char *);

        static INSTRUMENT_MGR _mgr;
        static INSTRUMENT_MGR &mgr() { return _mgr;}
};




#endif
