typedef struct __ngmsg_s_head{
    unsigned char sid;//0~7F
    unsigned char c;
    unsigned char lh;
    unsigned char ll;
} NGMSG_HEAD,*PNGMSG_HEAD;
typedef struct __ngmsg_s{
    NGMSG_HEAD head;
    char *payload;
} NGMSG,*PNGMSG;
extern int test();