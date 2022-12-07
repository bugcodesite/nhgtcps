typedef struct __ngmsg_s_head{
    __int8_t sid;//0~7F
    __int8_t c;
    __int16_t l;
} NGMSG_HEAD,*PNGMSG_HEAD;
typedef struct __ngmsg_s{
    NGMSG_HEAD head;
    char *payload;
} NGMSG,*PNGMSG;
extern int test();