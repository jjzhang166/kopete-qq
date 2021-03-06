//#include <imgstore.h>
#include <string.h>
#include <assert.h>
//#include <smiley.h>
//#include <accountopt.h>
#include "translate.h"
#include "trex.h"
#include "qq_types.h"
#include "msg.h"
#include <ktemporaryfile.h>
#include <QString>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <fcntl.h>
#include <stdio.h>
#ifndef INST_PREFIX
#define INST_PREFIX "/usr"
#endif

#define FACE_DIR INST_PREFIX"/share/pixmaps/pidgin/emotes/qq/"
//static GHashTable* smily_hash;
static TRex* _regex;
static TRex* hs_regex;
struct smile_entry{
    int id;
    const char* smile[6];
};
static struct smile_entry smile_tables[] = {
    {14,    {   ":)",   ":-)",      "/微笑",    "/wx",      0}},
    {1,     {   ":~",   "/撇嘴",    "/pz",      0}},
	{2,     {   ":B",   "/色",      "/se",      0}},
    {3,     {   ":|",   "/发呆",    "/fd",      0}},
    {4,     {   "8-)",  "/得意",    "/dy",      0}},
    {5,     {   ":[",   "/流泪",    "/ll",      0}},
    {6,     {   ":$",   "/含羞",    "/hx",      0}},
    {7,     {   ":X",	"/闭嘴",	"/bz",      0}},
    {8,     {   ":Z",	"/睡", 	    "/shui",    0}},
    {9,     {   ":'(",  ":'-(",	    "/大哭",	"/dk",      0}},
    {10,    {   ":-|",	"/尴尬",	"/gg",      0}},
    {11,    {   ":@",	"/发怒",	"/fn",      0}},
    {12,    {   ";-)",  ";)",		":P",		"/调皮",	"/tp",      0}},
    {13,    {   ":D",   "/呲牙",	"/cy",      0}},
    {0,     {   ":O",	"/惊讶",	"/jy",      0}},
    {50,    {   ":-(",  ":(",		"/难过",	"/ng",      0}},
    {51,    {   "8-)",	":+",		"/酷",		"/kuk",     0}},
    {96,    {   "--b",	"/冷汗",	"/lengh",   0}},
    {53,    {   ":Q",	"/抓狂",	"/zk",      0}},
    {54,    {   ":T",	"/吐",		"/tuu",     0}},
    {73,    {   ":p",	"/偷笑",	"/tx",      0}},
    {74,    {   ":-D",	"/可爱",	"/ka",      0}},
    {75,    {   ":d",	"/白眼",	"/baiy",    0}},
    {76,    {   ":o",	"/傲慢",	"/am",      0}},
    {77,    {   ":g",	"/饥饿",	"/jie",     0}},
    {78,    {   "|-)",	"/困",		"/kun",     0}},
    {55,    {   ":!",	"/惊恐",	"/jk",      0}},
    {56,    {   ":L",	"/流汗",	"/lh",      0}},
    {57,    {   ":}",	"/憨笑",	"/hx",      0}},
    {58,    {   ":;",	"/大兵",	"/dab",     0}},
    {79,    {   ":f",	"/奋斗",	"/fendou",  0}},
    {80,    {   ":-S",	"/咒骂",	"/zhm",     0}},
    {81,    {   "???",	"/yiw",	    "/yiw",     0}},
    {82,    {   ";X",	"/嘘",		"/xu",      0}},
    {83,    {   ";@",	"/晕",		"/yun",     0}},
    {84,    {   ":8",	"/折磨",	"/zhem",    0}},
    {85,    {   ":b",	"/哀",		"/shuai",   0}},
    {86,    {   "/骷髅",	"/kl",      0}},
    {87,    {   "/xx",	"/敲打",	"/qiao",    0}},
    {88,    {   "/bye",	"/再见",	"/zj",      0}},
    {97,    {   "/wipe","/擦汗",	"/ch",      0}},
    {98,    {   "/dig",	"/抠鼻",	"/kb",      0}},
    {99,    {   "/handclap",        "/鼓掌",	"/gz",      0}},
    {100,   {	"&-(",	"/糗大了",	"/qd",      0}},
    {101,   {	"B-)",	"/坏笑",	"/huaix",   0}},
    {102,   {	"[@",	"/左哼哼",	"/zhh",     0}},
    {103,   {	"@]",	"/右哼哼",	"/yhh",     0}},
    {104,   {	":-O",	"/哈欠",	"/hq",      0}},
    {105,   {	"]-|",	"/鄙视",	"/bs",      0}},
    {106,   {	"P-(",	"/委屈",	"/wq",      0}},
    {107,   {	":'|",	"/快哭了",	"/kk",      0}},
    {108,   {	"X-)",	"/阴险",	"/yx",      0}},
    {109,   {	":*",	"/亲亲",	"/qq",      0}},
    {110,   {	"@x",	"/吓",		"/xia",     0}},
    {111,   {	"/8*",	"/可怜",	"/kel",     0}},
    {112,   {	"/pd",	"/菜刀",	"/cd",      0}},
    {32,    {	"/[w]",	"/西瓜",	"/xig",     0}},
    {113,   {	"/beer","/啤酒",	"/pj",      0}},
    {114,   {	"/basketb",		    "/篮球",    "/lq",      0}},
    {115,   {	"/oo",	"/乒乓",	"/pp",      0}},
    {63,    {	"/coffee",			"/咖啡",	"/kf",      0}},
    {59,    {	"/pig",	"/猪头",	"/zt",      0}},
    {33,    {	"/rose","/玫瑰",	"/mg",      0}},
    {34,    {	"/fade","/凋谢",	"/dx",      0}},
    {116,   {	":-*",	"/示爱",	"/sa",      0}},
    {36,    {	"/heart",           "/爱心", 	"/xin",     0}},
    {37,    {	"/break",           "/心碎", 	"/xs",      0}},
    {38,    {	"/cake","/蛋糕",	"/dg",      0}},
    {91,    {	"/li",	"/闪电",	"/shd",     0}},
    {92,    {	"/bomb","/炸弹",	"/zhd",     0}},
    {93,    {	"/kn",	"/刀",		"/dao",     0}},
    {29,    {	"/footb",	        "/足球",	"/zq",      0}},
    {117,   {	"/瓢虫",            "/pch",     0}},
    {72,    {	"/ww",	"/便便",	"/bb",      0}},
    {45,    {	"/moon","/月亮",	"/yl",      0}},
    {42,    {	"/sun",	"/太阳",	"/ty",      0}},
    {39,    {	"/gift","/礼物",	"/lw",      0}},
    {62,    {	"/hug",	"/拥抱",	"/yb",      0}},
    {46,    {	"/强",	"/qiang",   0}},
    {47,    {	"/weak","/弱",		"/ruo",     0}},
    {71,    {	"/share",	        "/握手",	"/ws",      0}},
    {95,    {	"/v",	"/胜利",	"/shl",     0}},
    {118,   {	"@)",	"/抱拳",	"/bq",      0}},
    {119,   {	"/jj",	"/勾引",	"/gy",      0}},
    {120,   {	"@@",	"/拳头",	"/qt",      0}},
    {121,   {	"/bad",	"/差劲",	"/cj",      0}},
    {122,   {	"/loveu",	        "/爱你",	"/aini",    0}},
    {123,   {	"/NO",	"/no",		"/bm",      0}},
    {124,   {	"/OK",	"/ok",		"/hd",      0}},
    {27,    {	"/love","/爱情",	"/aiq",     0}},
    {21,    {	"/[L]",	"/飞吻",	"/fw",      0}},
    {23,    {	"/jump","/跳跳",	"/tiao",    0}},
    {25,    {	"/shake",           "/发抖",	"/fad",     0}},
    {26,    {	"/[o]",	"/怄火",	"/oh",      0}},
    {125,   {	"/转圈","/zhq",     0}},
    {126,   {	"/磕头","/kt",      0}},
    {127,   {	"/回头","/ht",      0}},
    {128,   {	"/跳绳","/tsh",     0}},
    {129,   {	"/oY",	"/挥手",	"/hsh",     0}},
    {130,   {	"#-O",	"/激动",	"/jd",      0}},
    {131,   {	"/街舞","/jw",	    0}},
    {132,   {	"/kiss","/献吻",	"/xw",      0}},
    {133,   {	"/<&",	"/左太极",	"/ztj",     0}},
    {134,   {	"/&>",	"/右太极",	"/ytj",     0}},
    {-1,    {   0   }}
};
const char* HTML_SYMBOL = "<[^>]+>|&amp;|&quot;|&gt;|&lt;";
#define sizemap(num) (6+2*num)
#define sizeunmap(px) ((px-6)/2)
static char* build_smiley_exp()
{
    char* exp = (char*)s_malloc0(2048);
    char* spec_char = "()[]*$\\|+";
    //<IMG ID=''> is belongs to <.*?>
    //first html label .then smiley
    strcpy(exp,"<[^>]+>|\\[FACE_\\d+\\]|\\[TOGGLEFACE\\]|/\\S+");
    struct smile_entry* entry = &smile_tables[0];
    const char *smiley,*beg,*end;
    const char** ptr;
    while(entry->id !=-1){
        ptr = entry->smile;
        while(*ptr){
            smiley = *ptr;
            if(smiley[0]=='/'){
                ptr++;
                continue;
            }
            strcat(exp,"|");
            beg = smiley;
            do{
                end=strpbrk(beg,spec_char);
                if(end==NULL) strcat(exp,beg);
                else {
                    strncat(exp, beg, end-beg);
                    strcat(exp,"\\");
                    strncat(exp,end,1);
                    beg = end+1;
                }
            }while(end);
            ptr++;
        }
        entry++;
    }
    return exp;
}
static LwqqMsgContent* build_string_content(const char* from,const char* to,LwqqMsgMessage* msg)
{
    LwqqMsgContent* c;
    LwqqMsgContent* last;
    const char *begin,*end,*read;
    char* write;
    if(to==NULL)to = from+strlen(from)+1;
    last = TAILQ_LAST(&msg->content,LwqqMsgContentHead);
    if(last && last->type == LWQQ_CONTENT_STRING){
        c = NULL;//return NULL
        size_t sz = strlen(last->data.str);
        last->data.str = (char*)s_realloc(last->data.str,sz+to-from+3);
        read = write = last->data.str+sz;
    }else{
        c = (LwqqMsgContent*)s_malloc0(sizeof(*c));
        c->type = LWQQ_CONTENT_STRING;
        c->data.str = (char*)s_malloc0(to-from+3);
        read = write = c->data.str;
    }
    //initial value
    strncpy(write,from,to-from);
    write[to-from]='\0';
    const char *ptr;
    //fprintf(stderr, "bengin while\n");
    while(*read!='\0'){
        if(!trex_search(hs_regex,read,&begin,&end)){
            ptr = strchr(read,'\0');
            ptr++;
            memmove(write,read,ptr-read);
            write+=ptr-read;
            break;
        }
        if(begin>read){
            memmove(write,read,begin-read);
            write+=(begin-read);
        }
        if(strncmp(begin,"<br>",strlen("<br>"))==0){
            strcpy(write,"\n");
            write++;
        }else if(strncmp(begin,"&amp;",strlen("&amp;"))==0){
            strcpy(write,"&");
            write++;
        }else if(strncmp(begin,"&quot;",strlen("&quot;"))==0){
            strcpy(write,"\"");
            write++;
        }else if(strncmp(begin,"&lt;",strlen("&lt;"))==0){
            strcpy(write,"<");
            write++;
        }else if(strncmp(begin,"&gt;",strlen("&gt;"))==0){
            strcpy(write,">");
            write++;
        }else if(begin[0]=='<'){
            if(begin[1]=='/'){
            }else{
                if(begin[1]=='b')lwqq_bit_set(msg->f_style,LWQQ_FONT_BOLD,1);
                else if(begin[1]=='i')lwqq_bit_set(msg->f_style,LWQQ_FONT_ITALIC,1);
                else if(begin[1]=='u')lwqq_bit_set(msg->f_style,LWQQ_FONT_UNDERLINE,1);
                else if(strncmp(begin+1,"font",4)==0){
                    const char *key = begin+6;
                    const char *value = strchr(begin,'=')+2;
                    const char *end = strchr(value,'"');
                    if(strncmp(key,"size",4)==0){
                        int size = atoi(value);
                        //msg->f_size = sizemap(size);
                        msg->f_size = size;
                    }else if(strncmp(key,"color",5)==0){
                        strncpy(msg->f_color,value+1,6);
                        msg->f_color[6] = '\0';
                    }else if(strncmp(key,"face",4)==0){
                        s_free(msg->f_name);
                        msg->f_name =(char*) s_malloc0(end-value+1);
                        strncpy(msg->f_name,value,end-value);
                        msg->f_name[end-value]='\0';
                    }
                }
            }
        }
        read = end;
    }
    *write = '\0';
    return c;
}
#if 0
static LwqqMsgContent* build_face_content(const char* face,int len)
{
    char buf[20];
    char* ptr = buf;
    if(len>=20){
        ptr = s_malloc(len+2);
    }
    memcpy(ptr,face,len);
    ptr[len]= '\0';
    LwqqMsgContent* c;
    if(smily_hash==NULL) translate_global_init();
    int num = (long)g_hash_table_lookup(smily_hash,ptr);
    if(num==0) return NULL;
    //unshift face because when build it we shift it.
    num--;
    c = s_malloc0(sizeof(*c));
    c->type = LWQQ_CONTENT_FACE;
    c->data.face = num;
    if(len>=20)
        s_free(ptr);
    return c;
}
#endif

static LwqqMsgContent* build_face_direct(int num)
{
    LwqqMsgContent* c;
    c = (LwqqMsgContent*)s_malloc0(sizeof(*c));
    c->type = LWQQ_CONTENT_FACE;
    c->data.face = num;
    return c;
}

static char *img_fileName(const char *file)
{
    //fprintf(stderr, "fileName:%s\n", basename(file));
    return basename(file);
}

static size_t img_filesize(const char *file)
{
    struct stat buf;
    if(stat(file, &buf)<0)
    {
        return 0;
    }
    return (size_t)buf.st_size;
}

static size_t img_filebuffer(const char *file, char *buffer)
{
    int fd;
    if((fd = open(file, O_RDONLY)))
    {

        int readSize = 0;
        int size = 0;
        char *buf[1024];
        bzero(buf, sizeof(buf));
        while ((size = read(fd, buf, sizeof(buf))) > 0) {

            memcpy(buffer+readSize, buf, size);
            readSize += size;
            bzero(buf, sizeof(buf));
        }
        //fprintf(stderr, "readsize:%d, totle:%d\n", readSize, img_filesize(file));
        close(fd);
        return readSize;
    }else
    {
        //fprintf(stderr,"open file failed\n");
        return 0;
    }
}



int translate_message_to_struct(LwqqClient* lc,const char* to,const char* what,LwqqMsg* msg,int using_cface)
{
    const char* ptr = what;
    static int img_id = 1;
    int face_id;
    LwqqMsgContent *c;
    const char* begin,*end;
    TRexMatch m;
    if(_regex==NULL) translate_global_init();
    TRex* x = _regex;
    LwqqMsgMessage* mmsg = (LwqqMsgMessage*)msg;
    int translate_face=0;
     
    while(*ptr!='\0'){
        c = NULL;
        if(!trex_search(x,ptr,&begin,&end)){
            ///last part.
            c = build_string_content(ptr,NULL,mmsg);
            if(c) TAILQ_INSERT_TAIL(&mmsg->content,c,entries);
            //this is finished yet.
            break;
        }
        if(begin>ptr){
            //note you can not pass c directly. 
            //because insert_tail is a macro.
            c = build_string_content(ptr,begin,mmsg);
            //you should insert twice now. like |inserted|begin|
            if(c) TAILQ_INSERT_TAIL(&mmsg->content,c,entries);
        }
        trex_getsubexp(x,0,&m);
#if 1
        char fileName[512] = {0};
        if(strstr(begin,"<img")==begin){
            //process ing img.
            //sscanf(begin,"<img src=\"%s\"/>",fileName);
            sscanf(begin, "%*[^\"]\"%[^\"]", fileName);
            char *imgBuffer = (char *)s_malloc(img_filesize(fileName));
            img_filebuffer(fileName, imgBuffer);
            if(using_cface||msg->type == LWQQ_MS_GROUP_MSG){
                c =( LwqqMsgContent*)s_malloc0(sizeof(*c));
                c->type = LWQQ_CONTENT_CFACE;
                c->data.cface.name = s_strdup(img_fileName(fileName));
                c->data.cface.data = (char*)s_malloc(img_filesize(fileName));
                memcpy(c->data.cface.data, imgBuffer,img_filesize(fileName));
                c->data.cface.size = img_filesize(fileName);
                char buf[12];
                snprintf(buf,sizeof(buf),"%d",img_id++);
                //record extra information to file_id
                c->data.cface.file_id = s_strdup(buf);
            }else{
                c = ( LwqqMsgContent*)s_malloc0(sizeof(*c));
                c->type = LWQQ_CONTENT_OFFPIC;
                c->data.img.name = s_strdup(img_fileName(fileName));
                c->data.img.data = (char*)s_malloc(img_filesize(fileName));
                memcpy(c->data.img.data,imgBuffer,img_filesize(fileName));
                c->data.img.size = img_filesize(fileName);
            }
       }else if(*begin==':'&&*(end-1)==':'){
            if(strstr(begin,":face")==begin){
                sscanf(begin,":face%d:",&face_id);
                c = build_face_direct(face_id);
            }else if(strstr(begin,":-face:")==begin){
                translate_face=!translate_face;
            }else{
                //other :faces:
                //c = translate_face?build_face_content(m.begin, m.len):NULL;
                 c = NULL;
                if(c==NULL) c = build_string_content(begin, end, mmsg);
            }
        }else if(begin[0]==':'){
            if(strstr(begin,":face")==begin){
                sscanf(begin,":face%d:",&face_id);
                c = build_face_direct(face_id);
                end =strstr(begin+1, ":") + 1;
            }else if(strstr(begin,":-face:")==begin){
                translate_face=!translate_face;
            }else{
                //other :faces:
                //c = translate_face?build_face_content(m.begin, m.len):NULL;
                 c = NULL;
                if(c==NULL) c = build_string_content(begin, end, mmsg);
            }
//            //other :)
//            c = translate_face?build_face_content(m.begin, m.len):NULL;
//            if(c==NULL)
//            {
//                c = s_malloc0(sizeof(*c));
//                c->type = LWQQ_CONTENT_STRING;
//                c->data.str = s_strdup(what);
//                end = what[strlen(what)+1];
//                //fprintf(stderr, "malloc\n");
//                if(end == '\0')
//                    //fprintf(stderr, "0000\n");
//            }

       }else if(begin[0]=='&'){
        }else{
             //fprintf(stderr, "else :\n");
//            //other face with no fix style
            //c = translate_face?build_face_content(m.begin,m.len):NULL;
             c = NULL;
            if(c==NULL) c = build_string_content(begin, end, mmsg);
        }
#endif
        ptr = end;
        if(c!=NULL)
            lwqq_msg_content_append(mmsg, c);
        //fprintf(stderr, "lwqq_msg_content_append\n");
    }
    return 0;
}

static void paste_content_string(const char* from,char* to)
{
    const char* read = from;
    const char* ptr = read;
    char* write = to;
    size_t n = 0;
    while((ptr = strpbrk(read,"<>&\""))){
        if(ptr>read){
            n = ptr-read;
            strncpy(write,read,n);
            write += n;
        }
        switch(*ptr){
            case '<' : strcpy(write,"&lt;");break;
            case '>' : strcpy(write,"&gt;");break;
            case '&' : strcpy(write,"&amp;");break;
            case '"' : strcpy(write,"&quot;");break;
        }
        read = ptr+1;
        write+=strlen(write);
    }
    if(*read != '\0'){
        strcpy(write,read);
        write+=strlen(read);
    }
    *write = '\0';
}
char* translate_to_html_symbol(const char* s)
{
    char buf[2048] = {0};
    paste_content_string(s, buf);
    return s_strdup(buf);
}

void translate_struct_to_message(qq_account* ac, LwqqMsgMessage* msg, char* buf)
{
    LwqqMsgContent* c;
    char piece[48] = {0};
    if(lwqq_bit_get(msg->f_style,LWQQ_FONT_BOLD)) strcat(buf,"<b>");
    if(lwqq_bit_get(msg->f_style,LWQQ_FONT_ITALIC)) strcat(buf,"<i>");
    if(lwqq_bit_get(msg->f_style,LWQQ_FONT_UNDERLINE)) strcat(buf,"<u>");
    snprintf(buf+strlen(buf),300,"<font ");
    if(ac->flag&DARK_THEME_ADAPT){
        int c = strtoul(msg->f_color, NULL, 16);
        int t = (c==0)?0xffffff:(c%256)/2+128+((c/256%256)/2+128)*256+((c/256/256%256)/2+128)*256*256;
        snprintf(buf+strlen(buf),300,"color=\"#%x\" ",t);
    }else
        snprintf(buf+strlen(buf),300,"color=\"#%s\" ",msg->f_color);
    if(!(ac->flag&IGNORE_FONT_FACE)&&msg->f_name)
        snprintf(buf+strlen(buf),300,"face=\"%s\" ",msg->f_name);
    if(!(ac->flag&IGNORE_FONT_SIZE))
        snprintf(buf+strlen(buf),300,"size=\"%d\" ",msg->f_size);
    strcat(buf,">");
    
    TAILQ_FOREACH(c, &msg->content, entries) {
        switch(c->type){
            case LWQQ_CONTENT_STRING:
                paste_content_string(c->data.str,buf+strlen(buf));
                break;
            case LWQQ_CONTENT_FACE:
                strcat(buf,translate_smile(c->data.face));
                break;
#if 1
            case LWQQ_CONTENT_OFFPIC:
                //fprintf(stderr,"pic size:%d, file path:%s\n", c->data.img.size, c->data.img.file_path);
                if(c->data.img.size>0){
                    KTemporaryFile *inkImage = new KTemporaryFile();
                    inkImage->setPrefix("chatgif-");
                    inkImage->setSuffix(".gif");
                    inkImage->open();
                    inkImage->write(c->data.img.data, c->data.img.size);
                    inkImage->close();
                    c->data.img.data = NULL;
                    //make it room to change num if necessary.
                    snprintf(piece,sizeof(piece),"<img src=\"%s\" />",inkImage->fileName().toUtf8().constData());
                    //fprintf(stderr, "url:%s\n", inkImage->fileName().toUtf8().constData());
                    strcat(buf,piece);
                }else{
                    if((msg->super.super.type==LWQQ_MS_GROUP_MSG&&ac->flag&NOT_DOWNLOAD_GROUP_PIC)){
                        strcat(buf,_("【DISABLE PIC】"));
                    }
                    else if(c->data.img.url){
                        format_append(buf, "<a href=\"%s\">%s</a>",
                                c->data.img.url,
                                _("【PIC】")
                                );
                    }
                    else{
                        strcat(buf,_("【PIC NOT FOUND】"));
                    }
                }
                break;
        case LWQQ_CONTENT_CFACE:
            if(c->data.cface.size>0){
                KTemporaryFile *inkImage = new KTemporaryFile();
                inkImage->setPrefix("chatgif-");
                inkImage->setSuffix(".gif");
                inkImage->open();
                inkImage->write(c->data.img.data, c->data.img.size);
                inkImage->close();
                c->data.cface.data = NULL;
                snprintf(piece,sizeof(piece),"<img src=\"%s\"/>",inkImage->fileName().toUtf8().constData());
                strcat(buf,piece);
            }else{
                if((msg->super.super.type==LWQQ_MS_GROUP_MSG&&ac->flag&NOT_DOWNLOAD_GROUP_PIC)){
                    strcat(buf,_("【DISABLE PIC】"));
                }
                else if(c->data.cface.url){
                    format_append(buf, "<a href=\"%s\">%s</a>",
                                  c->data.cface.url,
                                  _("【PIC】")
                                  );
                }
                else{
                    strcat(buf,_("【PIC NOT FOUND】"));
                }
            }
                break;
#endif
        }
    }
    strcat(buf,"</font>");
    if(lwqq_bit_get(msg->f_style,LWQQ_FONT_BOLD)) strcat(buf,"</u>");
    if(lwqq_bit_get(msg->f_style,LWQQ_FONT_ITALIC)) strcat(buf,"</i>");
    if(lwqq_bit_get(msg->f_style,LWQQ_FONT_UNDERLINE)) strcat(buf,"</b>");
}

void translate_global_init()
{
    if(_regex==NULL){
        const char* err = NULL;
        char* smiley_exp = build_smiley_exp();
        _regex = trex_compile(_TREXC(smiley_exp),&err);
        free(smiley_exp);
        if(err) {lwqq_puts(err);assert(0);}
        assert(_regex!=NULL);
    }
    if(hs_regex==NULL){
        const char* err = NULL;
        hs_regex = trex_compile(_TREXC(HTML_SYMBOL),&err);
        if(err){lwqq_puts(err);assert(0);}
        assert(_regex!=NULL);
    }
}

void translate_global_free()
{
    if(_regex){
        trex_free(_regex);
        _regex = NULL;
    }
    if(hs_regex){
        trex_free(hs_regex);
        hs_regex = NULL;
    }
}
const char* translate_smile(int face)
{
    static char buf[64];
    snprintf(buf, sizeof(buf), ":face%d:",face);
    return buf;
}
