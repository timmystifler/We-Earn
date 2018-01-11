#include "login_account.h"
#include <string.h>

LOGIN_ACCOUNT LOGIN_ACCOUNT::gs_login_account;

//warning : if now passwd is not ok use old passwd to login
LOGIN_ACCOUNT::LOGIN_ACCOUNT()
{
    strcpy(broker_id, "9999\0");
    strcpy(investor_id, "110238\0");
    strcpy(old_passwd, "lhc997\0");
    strcpy(now_passwd, "lhc199707\0");
}
