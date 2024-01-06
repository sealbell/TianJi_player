/*

        滚屏

*/

//歌词输出
#include <stdio.h>
#include <stdlib.h>
#include "console.h"
#include "my_lrc_play.h"

void my_roll_lrc(int mytime, NODE *head, NODE **parr, int flag, NODE **pxx, NODE **pxx2)
{

        if (parr[0] != NULL && mytime == parr[0]->time)
        {

                int i = 0;
                for (i = 0; i < 3; i++)
                {
                        cusor_moveto(150, 19 + i * 2);
                        // if (i == 0)
                        // {
                        //         set_fg_color(COLOR_CYAN);
                        // }

                        if (parr[0] != NULL)
                        {
                                set_fg_color(COLOR_CYAN);
                                cusor_moveto(150, 19);
                                printf("%s                                 \n", parr[0]->lrc);
                                cusor_set_pos();
                                fflush(stdout);
                                if (parr[1]->next != NULL)
                                {
                                        set_fg_color(COLOR_WHITE);
                                        cusor_moveto(150, 21);
                                        printf("%s                                 \n", parr[1]->lrc);
                                        cusor_set_pos();
                                        fflush(stdout);
                                        if (parr[2]->next != NULL)
                                        {
                                                set_fg_color(COLOR_WHITE);
                                                cusor_moveto(150, 23);
                                                printf("%s                                 \n", parr[2]->lrc);
                                                cusor_set_pos();
                                                fflush(stdout);
                                        }
                                        else if (parr[2]->next == NULL)
                                        {
                                                cusor_moveto(150, 23);
                                                printf("                                              ");
                                        }
                                }
                                else if (parr[1]->next == NULL)
                                {
                                        cusor_moveto(150, 21);
                                        printf("                                              ");
                                }
                        }
                }
                if (parr[0] == pxx[0]->next)
                {
                        cusor_moveto(150, 17);
                        printf("%s                             \n", pxx[0]->lrc);
                        cusor_set_pos();
                        fflush(stdout);
                }
                if (pxx[0] == pxx2[0]->next)
                {
                        cusor_moveto(150, 15);
                        printf("%s                             \n", pxx2[0]->lrc);
                        cusor_set_pos();
                        fflush(stdout);
                }

                for (i = 0; i < 3; i++)
                {
                        pxx2[i] = pxx[i];
                        pxx[i] = parr[i];
                        //pxx2[i] = parr[i]->next;
                        //步进条件，匹配上了我们才步进
                        if (parr[i]->next != NULL)
                        {
                                parr[i] = parr[i]->next;
                        }
                }
        }
}

