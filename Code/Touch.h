#ifndef SHARE_TOUCH_H
#define SHARE_TOUCH_H
char product_C[8][20]={"可口可乐","椰树椰汁","东方树叶","日本生可乐","阿萨姆奶茶","红牛","冰红茶","绿茶"};

// 根据触摸点坐标判断选择的商品存在选择的商品时，点击付款按钮进入付款界面，取消支付后返回主菜单
int select_product(int x,int y,int Pro_select,int* pro_num)      //Pro_select上次选择后的商品编号
{

    printf("SELECT PRODUCE ...\n");
    int pro=0;      //pro本次触屏商品编号
    int x_min[8]={650,510,350,165,650,510,350,165};
    int x_max[8]={780,615,415,240,780,615,415,240};
    int y_min[8]={260,260,260,260,80,80,80,80};
    int y_max[8]={415,415,415,415,240,240,240,240};
    

    int x_button_b = 110;
    int x_button_e = 5;
    int y_button_b = 100;
    int y_button_e = 0;

    // 判断本次选择的商品
    while(pro<8)
    {
        if(x>=x_min[pro] && x<=x_max[pro] && y>=y_min[pro] && y<=y_max[pro]){
            if(pro!=Pro_select) *pro_num=1;
            else    (*pro_num)++;
            return pro;
        }
        pro++;
    }


    //判断是否点击付款按钮
    if(x > x_button_e && x < x_button_b && y > y_button_e && y < y_button_b){

        if(Pro_select != -1){    
            char str[100] = "";
            sprintf(str,"%s %d瓶",product_C[Pro_select],*pro_num);
            talk(str,sizeof(str));  //在上次选择了商品的前提下，进入付款界面
            button(Pro_select,*pro_num); 
        }
            *pro_num=0;
    }

    return -1;
}

//获取触摸点的坐标
int get_touchscreen_coordinate(int *x, int *y)
{
    //打开触摸屏文件
    int fd;

    fd = open("/dev/input/touchscreen0", O_RDWR);
    if (fd == -1)
    {
        perror("open /dev/input/touchscreen0 ");
        return 0;
    }

    //获取触摸点的坐标(其实就是读取触摸屏)
    struct input_event ev;
    while(1)
    {
        read(fd, &ev, sizeof(ev)); //当我们按下触摸屏的时候，坐标信息都保存在了ev里边
        //注意: x和y坐标需要通过两次读取才能够读取完成
        if (ev.type == EV_ABS && ev.code == ABS_X)
        {
            *x = ev.value;
            printf("x: %d\n", *x);
        }
        else if (ev.type == EV_ABS && ev.code == ABS_Y)
        {
            *y = ev.value;
            printf("y: %d\n", *y);
        }
        //判断手是否松开
        if (ev.type == EV_ABS && ev.code == ABS_PRESSURE && ev.value == 0)
        {
            return 0;
        }
    }
}


#endif //SHARE_TOUCH_H
