#include"my_lrc_play.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"start_mplayer.h"
#include"console.h"
#include"my_add.h"



//读lrc文件到堆区空间并返回堆区指针，以便链表的使用
char*  my_read_lrc_file(char my_lrc_name[])
{
    // printf("%s\n",my_lrc_name);
    // return my_lrc_name;
    //字符数组已成功获取

    //打开文件
    FILE* fp = fopen(my_lrc_name,"r");
    if (NULL == fp)
    {
        perror("fopen");
        exit(-1);
    }
    
    //将文件流定位到指针末尾
    fseek(fp,0,2);
    //获取文件长度
    int len = ftell(fp);
    //复位文件流指针（此处如果写fclose应该需要重新打开）
    rewind(fp);

    //使用calloc全体自动置0，接下来仅仅读入len的长度，方便%s访问
    char* stack = (char*)calloc(1,len+1);

    //此处应该考虑文件大于堆区空间的情况，再议
    //
    fread(stack,len,1,fp);
    fclose(fp);

    return stack;

}

int my_strtok_lrc_data(char* stack,char* cut_of_every_row_arr[])
{
    int i = 0;
    cut_of_every_row_arr[i] = strtok(stack,"\r\n");
    //如果还未指向堆区的\0,说明还没切完继续切
    while (cut_of_every_row_arr[i] != NULL)
    {
        i++;
        cut_of_every_row_arr[i] = strtok(NULL,"\r\n");

    }
    return i;
}

// 切好之后，传入切好的堆区头部指针们，进行格式化输出到屏幕
#if 0
//这获取不到是为什么？？？
void my_analyze_lrc_head(char** cut_of_every_row_arr)
{
    int i = 0;
    while (cut_of_every_row_arr[i][1]<'0'||cut_of_every_row_arr[i][1]>'9')
    {
        
            char msg[128] = "";
            sscanf(&(cut_of_every_row_arr[i][1]),"[%*2c:%[^]]]",msg);
            //光标定位输出******
            printf("%s\n",msg);
            printf("hellp\n");
        i++;
    }
    
}
#endif
//获取头
void my_analyze_lrc_head(char* cut_of_every_row_arr[])
{
    int i =0;
    while (cut_of_every_row_arr[i]!=NULL)
    {
        char* p = cut_of_every_row_arr[i];
        if (p[1]>='a'&&p[1]<='z')
        {
            //char msg[128] = "";
            char* msg = (char*)calloc(1024,1);
            sscanf(p,"[%*2c:%[^]]]",msg);
            //光标定位输出
            cusor_moveto(156,10+i);
            printf("%s\n",msg);
        }
        else if(p[1]>='0' && p[1]<='9')
            break;
        i++;
    }
    
}
//切割头

//获取歌词
NODE* my_analyze_lrc_song(char** cut_of_every_row_arr,NODE* head)
{
    int i = 0;
    while (cut_of_every_row_arr[i] != NULL)
    {
        char* p = cut_of_every_row_arr[i];
        //条件为判断它有时间即是歌词
        if (p[1]>='0' && p[1]<='9')
        {

            //寻找歌词的位置
            char* p2 = p;
            while (*p2 =='[')
            {
                p2+=10;
            }

            //逐个时间分析
            while (*p == '[')
            {
                int m = 0;
                int s = 0;
                sscanf(p,"[%d:%d",&m,&s);
                NODE tmp;
                tmp.time = m*60+s;
                strcpy(tmp.lrc,p2);
                head = insert_link(head,tmp);
                p+=10;

            }
            
        }
        i++;
    }
    return head;
}
//有序插入
NODE* insert_link(NODE* head,NODE tmp)
{
    NODE* pi = (NODE*)calloc(1,sizeof(NODE));

    *pi = tmp;
    pi->next = NULL;

    if (NULL == head)
    {
        head = pi;
        return head;
    }
    else
    {
        NODE* pf = head, *pb = head;
        while (pb->time < pi->time && pb->next != NULL)
        {
            pf = pb;
            pb = pb->next;
        }
        
        if (pb->time>=pi->time)
        {
            if (pb == head)
            {
                pi->next = head;
                head = pi;
                return head;
            }
            else
            {
                pf->next = pi;
                pi->next = pb;
                return head;
            }
        
        }
         else
        {
            pb->next = pi;
            return head;
        }
    }
    return head;
}

void print_link(NODE* head)
{
    if(NULL == head)
    {
        printf("link not exist\n");
        return;
    }
    else
    {
        NODE* pb = head;
        while (pb != NULL)
        {
            printf("%d %s\n",pb->time,pb->lrc);
            pb = pb->next;
        }
        
    }
    
}

#if 0
NODE* search_link(NODE* head,int time)
{
    if (NULL == head)
    {
        printf("link not exist\n");
        return NULL;
    }
    else
    {
        NODE* pb = head;
        while (strcmp(pb->time,time)!=0&&pb->next!=NULL)
        {
            pb = pb->next;
        }
        if (strcmp(pb->time,time) == 0)
        {
            return pb;
        }
        else
        {
            printf("not found!\n");
            return NULL;
        }
        
    }
    
}
#endif
#if 0
//在屏幕左下角打印时钟
// void my_clock()
// {
//     int time = 0;
//     //定位光标
//     cusor_moveto(50,0);
//     while (/*<=时间最大值*/)
//     {
//         printf("\r%02d:%02d",mytime/60,mytime%60);
//         fflush(stdout);
//         sleep(1);
//         time++;
//     }
    
// }
// //获取时间最大值
// void time_max()
// {

// }
#endif
void my_matching(NODE* head,char* cut_of_every_row_arr[])
{
    //打印头
    
    my_analyze_lrc_head(cut_of_every_row_arr);


    //打印总条
    cusor_moveto(35,59);
    int i = 0;
    int n = 100;
    
    for ( i = 0; i < n; i++)
    {
       printf("-");
    }

    #if 0
    cusor_moveto(150,17);
    printf("helloworldhhhhhhhhhhh");
    cusor_set_pos();
    fflush(stdout);

    cusor_moveto(150,19);
    printf("helloworldhhhhhhhhhhh");
    cusor_set_pos();
    fflush(stdout);

    cusor_moveto(150,21);
    printf("helloworldhhhhhhhhhhh");
    cusor_set_pos();
    fflush(stdout);

    cusor_moveto(150,23);
    printf("helloworldhhhhhhhhhhh");
    cusor_set_pos();
    fflush(stdout);

    #endif

    mplayer_play("简单爱.mp3");
    NODE* pm = head;
    int mytime = 0;
    sleep(1);

    //此tmp用于初始化进度容器
    float tmp = 0;
    //进度条动态光标位置初始化
    int j = 0;

    //最大时间输出
    cusor_moveto(25,60);
    int maxtime = print_song_max_time(head);
    cusor_set_pos();

    //初始化三个指针
    NODE *parr[3] = {NULL};
    NODE *phead = head;
    for (i = 0; i < 3; i++)
    {
        parr[i] = phead;
        phead = phead->next;
    }
    NODE *pxx[3] = {NULL};
    for ( i = 0; i < 3; i++)
    {
        pxx[i] = head;
        
    }
    NODE *pxx2[3] = {NULL};
    for ( i = 0; i < 3; i++)
    {
        pxx2[i] = head;
        
    }

    while (mytime<=maxtime)
    {
        
        
        //时钟输出
        cusor_moveto(20,60);
        printf("%02d:%02d",mytime/60,mytime%60);

        //歌词输出
        

        //滚屏
        int flag = 0;
        my_roll_lrc(mytime,head,parr,flag,pxx,pxx2);

        // cusor_moveto(150,15);
        // if (mytime == pm->time)
        // {
        //     printf("%s\n",pm->lrc);
        //     pm = pm->next;
        // }
        // fflush(stdout);



        //打印目前进度
        
        tmp += (float)100/(float)maxtime;
        if ((tmp-1)>=0)
        {
            tmp -= 1;
            cusor_moveto(35+j,59);

            set_fg_color(COLOR_CYAN);
            printf("➤");
            set_fg_color(COLOR_WHITE);

            j++;
            fflush(stdout);
        }

        //步进
        sleep(1);
        mytime++;

    }
    sleep(5);
}

int print_song_max_time(NODE* head)
{
    while (head->next!=NULL)
    {
        head =head->next;
    }
    printf("/%02d:%02d",head->time/60,head->time%60);
    return head->time;
}



//打印进度条
void my_print_line()
{

    //打印总条
    cusor_moveto(35,59);
    int i = 0;
    int n = 100;
    
    for ( i = 0; i < n; i++)
    {
        printf("-");
    }
    
}
//项目的流程
void my_lrc_play()
{
    //生成一个图形界面
    //仔细一想不行，需要先获取lrc中的全部数据之后，再对页面进行初始化
    system("clear screen");
    //从lrc中一次性读取到堆空间
    printf("请输入歌词名\n");
    char my_lrc_name[128];
    scanf("%s",my_lrc_name); 

    //此函数在内部申请了这个歌词文件那么大的堆区空间 并赋给了stack
    char* stack = my_read_lrc_file(my_lrc_name);

    #if 0
    //对堆区空间进行按“\r\n”切割
    //切割函数，返回值为int，获取了切割次数，以便给链表赋值
    //两个参数分别为：堆区空间入口，切好的字符串数组名
    //字符串数组即指针数组
    //由于不知道，到底需要切割多少行，需要借助strtok的返回值，申请变长数组，此处暂时用一个大数组代替
    #endif

    //这是一个指针数组，指针指向的类型是字符
    char* cut_of_every_row_arr[1024] =  {NULL};

    //对堆区空间进行切割，并将切割的头放入，cut_of_every_arr[]
    int num_of_node = my_strtok_lrc_data(stack,cut_of_every_row_arr);
    
    #if 0
    //验证了切割成功
    int i = 0;
    while (cut_of_every_row_arr[i])
    {
        printf("%s\n",cut_of_every_row_arr[i]);
        i++;
    }
    #endif

    //切割成功后，应格式化获取信息，先获取头信息
    //my_analyze_lrc_head(cut_of_every_row_arr);

    //分析歌词以及对应时间
    NODE* head = NULL;
    head = my_analyze_lrc_song(cut_of_every_row_arr,head);

    //print_link(head);

    //歌词匹配
    my_matching(head,cut_of_every_row_arr);

    //打印歌曲的时间
    //print_song_max_time(head);
    
    //清屏结束
    system("clear screen");
    printf("播放完毕\n");
    
}
