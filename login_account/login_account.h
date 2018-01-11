#ifndef _LOGIN_ACCOUNT_H_
#define _LOGIN_ACCOUNT_H_

#define ACCOUNT_ID_LEN 50
#define PASSWORD_LEN 100

class LOGIN_ACCOUNT
{
    public:
        char broker_id[ACCOUNT_ID_LEN];
        char investor_id[ACCOUNT_ID_LEN];
        char old_passwd[PASSWORD_LEN];
        char now_passwd[PASSWORD_LEN];

        LOGIN_ACCOUNT();
        static LOGIN_ACCOUNT gs_login_account;
        static LOGIN_ACCOUNT *get_login_account() { return &gs_login_account;}
};

#endif
