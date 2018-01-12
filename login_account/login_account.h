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

        int front_id;
        int session_id;
        int order_ref;

        LOGIN_ACCOUNT();
        static LOGIN_ACCOUNT gs_login_account;
        static LOGIN_ACCOUNT &account() { return gs_login_account;}

        void set_front_id(int f_id) {front_id = f_id;}
        void set_session_id(int s_id) {session_id = s_id;}
        void set_order_ref(int ref) {order_ref = ref;}
        int order_ref_add_and_get() {++order_ref; return order_ref;}
};

#endif
