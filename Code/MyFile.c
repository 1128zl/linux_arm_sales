#include "stdio.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <pthread.h>
#include "Numbers.h"
#include "Character.h"
#include "Serial.h"
#include "Lcd.h"
#include "Touch.h"
#include "Menu.h"
#include "show_goods.h"

pthread_t gif_thread; // ?????????
pthread_t touch_thread; // ???????????????

int stop_animation = 0; // ???��????????????1

// ??????????????
void* touch_thread_func(void* arg) 
{
    // ????????
    int fd_touchscreen = open("/dev/input/touchscreen0", O_RDWR);
    if (fd_touchscreen == -1) {
        perror("open /dev/input/touchscreen0 ");
        pthread_exit(NULL);
    }

    struct input_event ev;
    while (1) {
        if (read(fd_touchscreen, &ev, sizeof(ev)) > 0) {
            if (ev.type == EV_ABS && ev.code == ABS_PRESSURE && ev.value == 0) {
                // ?????????????????��??????????
                stop_animation = 1;
                break;
            }
        }
    }

    close(fd_touchscreen);
    pthread_exit(NULL);
}

// ?????????
void* gif_thread_func(void* arg) {
    char *walk[] = {
        "walk/1.bmp",
        "walk/2.bmp",
        "walk/3.bmp",
        "walk/4.bmp",
        "walk/5.bmp",
        "walk/6.bmp",
        "walk/7.bmp",
        "walk/8.bmp",
        "walk/9.bmp",
        "walk/10.bmp",
        "walk/11.bmp",
        "walk/12.bmp",
        "walk/13.bmp",
        "walk/14.bmp",
        "walk/15.bmp"
    };

    int i = 0;
    while (!stop_animation)     //stop_animation?0????????
    {
        // ?????
        lcd_draw_bmp(walk[i], 800, 600, 50, 0);
        usleep(5000);
        i++;
        if (i == 15) i = 0;
    }
    pthread_exit(NULL);
}

//?????????��???????
void gif_stop() 
{
    // ????????????????
    if (pthread_create(&touch_thread, NULL, touch_thread_func, NULL) != 0) {
        perror("pthread_create touch_thread");
        return -1;
    }
    // ?????????????
    if (pthread_create(&gif_thread, NULL, gif_thread_func, NULL) != 0) {
        perror("pthread_create gif_thread");
        return -1;
    }
    // ??????????????????????????
    pthread_join(touch_thread, NULL);
    // ??????��?????????????
    stop_animation = 1;
    // ????????????????????
    pthread_join(gif_thread, NULL);
    return;
}

int main(){
    int ret;
    int x,y;
    
    int pro = -1;   // ???
    int pro_num=0;  //???????
    
    ret = lcd_init();   //LCD????????
    if(-1 == ret)       //LCD?????????
    {
        perror("Can't init ...");
        return -1;
    }
    gif_stop();             //????????????????????????
    show_boot_animation();  //?????????????????
    show_menu();            //???????????????
    set_timer();            //??????????????
    talk("???????????????",sizeof ("???????????????"));

    //?????????????????
    pthread_t AD_pid;
    pthread_create(&AD_pid,NULL,pthread_show_ad,NULL);

    while (1)
    {
        get_touchscreen_coordinate(&x, &y); 
        pro = select_product(x,y,pro,&pro_num);  //????????????????��?????
        if(pro != -1) {
            talk(product_C[pro],sizeof (product_C[pro]));//???????????
            select_goods(pro,pro_num);  //??????????????????
        }
    }
    
    return 0;
}