//
// Created by 11275 on 2023/12/6.
//

#ifndef SHARE_MENU_H
#define SHARE_MENU_H

double goods_price[8] = {3.50,5.00,5.00,4.00,4.50,5.00,4.00,4.00};

int lcd_init()
{
    //打开LCD设备文件
    int fd;

    fd = open("/dev/fb0", O_RDWR);
    if (-1 == fd) //fd == -1
    {
        perror("open /dev/fb0 ");
        return -1;
    }

    int x, y;
    plcd = mmap(NULL, LCD_WIDTH*LCD_HEIGHT*2, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    // 界面初始化为白色
    for (y = 0; y < 600; y++)
    {
        for (x = 0; x < 1024; x++)
        {
            //将BMP图片上的一个像素点(占24个bit)转换成LCD上的一个占16个bit的像素点
            unsigned short color = 0xFFFF;
            //在LCD上显示像素点
            //*(unsigned short *)(plcd + (x+x0) + (h-1-y+y0)*1024) = color;
            lcd_draw_point(x, y, color);
        }
    }
    return fd;
}

/*
 * 从上到下滚动播放照片
 * */
void show_boot_animation(){
    int fd;
    fd = open("boot.bmp", O_RDWR);      // boot.bmp是开机壁纸文件
    if (-1 == fd)
    {
        perror("open bmp file ");
        return ;
    }
    
    char pixels[LCD_WIDTH*LCD_HEIGHT*3]; // boot.bmp文件的大小刚好为 1024 * 600
    unsigned short color;
    int end_line = -1;      //  * end_line,代表图片目前显示的最后一行；
    
    //因为bmp图片的前面54个字节保存了图片的先关信息比如名字 宽度 高度 位深，需要跳过前面的54个字节
    lseek(fd, 54, SEEK_SET);
    read(fd, pixels, sizeof(pixels)); //将bmp图片中的所有的像素点读取保存到一个数组中
    int x,y,z;

    //打开触摸屏文件
    int fd_screen;

    fd_screen = open("/dev/input/touchscreen0", O_RDWR);
    if (fd_screen == -1)
    {
        perror("open /dev/input/touchscreen0 ");
        return ;
    }

    // 图片滚动
    for (z = 0; z < LCD_HEIGHT;)  // 逐行刷新
    {
        int i = 0;
        for(y = 0;y < z; y++)
        {
            for(x = 0;x < LCD_WIDTH;x++)
            {	
                color = (pixels[i+2]>>3)<<11 | (pixels[i+1]>>2)<<5 | pixels[i]>>3;
                i += 3;
                //在LCD上显示像素点
                *(unsigned short *)(plcd + x + (z - y)*LCD_WIDTH) = color;	
            }
        }
        z+=5;
    }
    usleep(1000000);// 一秒后退出
}

void show_time()
{
    //初始化显示屏，否则会重叠
    int x, y;
    for (y = 30; y < 60; y++) {
        for (x = 700; x < 700+65+38+30+46+22+35+18+30; x++) {
            //将BMP图片上的一个像素点(占24个bit)转换成LCD上的一个占16个bit的像素点
            unsigned short color =  0xFFFF;
            lcd_draw_point(x,y, color);
        }
    }

    //获取从1970.1.1 00:00 经过的秒数
    time_t t = time(NULL);
    struct tm *tm;
    tm = localtime(&t); //用于获取当前时间并将其表示为本地时间

    //2023 12/4 17:21
    lcd_draw_number(tm->tm_year+1900, 700, 30, RED); //显示年

    //显示月
    if(tm->tm_mon+1>9)	// 10-12月
    {
        lcd_draw_number(tm->tm_mon+1, 700+65, 30, RED);
    }
    else	//01-09月
    {
        lcd_draw_word(numbers[0],sizeof(numbers[0]),16,700+65, 30,RED);
        lcd_draw_number(tm->tm_mon+1, 700+65+18, 30, RED);
    }

    //显示一个斜杠
    lcd_draw_word(sign1, sizeof(sign1), 16,700+65+38, 30, RED);

    //显示日
    if(tm->tm_mday>9)	// 10-31日
    {
        lcd_draw_number(tm->tm_mday, 700+65+38+30, 30, RED);
    }
    else	// 01-09日
    {
        lcd_draw_word(numbers[0],sizeof(numbers[0]),16, 700+65+38+30, 30,RED);
        lcd_draw_number(tm->tm_mday, 700+65+38+30+18, 30, RED);
    }

    //显示小时
    if(tm->tm_hour>9) // 10-23时
    {
        lcd_draw_number(tm->tm_hour, 700+65+38+30+46, 30, RED);
    }
    else	//0-09时
    {
        lcd_draw_word(numbers[0],sizeof(numbers[0]),16, 700+65+38+30+30, 30,RED);
        lcd_draw_number(tm->tm_hour, 700+65+38+30+46+2, 30, RED);
    }

    //显示一个冒号
    lcd_draw_word(sign2, sizeof(sign2), 32,700+80+38+30+46+5, 30, RED);

    // 显示分钟
    if(tm->tm_min>9)	// 10-59分
    {
        lcd_draw_number(tm->tm_min, 700+65+38+30+46+22+35, 30, RED);
    }
    else	//01-09分
    {
        lcd_draw_word(numbers[0],sizeof(numbers[0]),16, 700+65+38+30+46+22+35, 30,RED);
        lcd_draw_number(tm->tm_min, 700+65+38+30+46+22+35+18, 30, RED);
    }
}

void timer_handler(int signum)
{
    show_time();//显示当前的时间
}

//设置一个60秒的定时
void set_timer()
{
    struct itimerval it, oldit;
    int ret;

    it.it_value.tv_sec = 5;	 //5秒以后就会产生第一个信号
    it.it_value.tv_usec = 0;
    it.it_interval.tv_sec = 60;	//定时60秒
    it.it_interval.tv_usec = 0;

    //设置一个SIGALRM信号处理函数，当我们定时的时间一到系统会自动产生一个SIGALRM一旦这个信号产生
    //会自动调用我们所设置的函数
    signal(SIGALRM, timer_handler);
    //设置定时
    setitimer(ITIMER_REAL, &it, &oldit);
}

/*
	@brief 				显示公司代表人名字
	@param person_name	保存代表人三个字+公司代表人名字的c51格式下的取模的数组
	@param len			数组person_name的长度
*/
void show_person_name(char person_name[11][116],int len) {
    //公司代表介绍显示，显示效果：代表人 张湘睿，左莉，刘杰
    int i;
    //依次显示代表人
    for(i=0;i<len;i++)
    {
        //将需要显示的文字从坐标(5,40)开始依次显示，字与字之间间隔8个像素点
        lcd_draw_word(person_name[i],116,32,5+i*40,40,BLACK);
    }
}

/*
	@brief 				在LCD的上方显示自动售货机所属公司以及代表人
	@param company_name 公司名字对应的C51格式对应的取模所存放的数组
	@param len  		公司名字C51格式对应的取模的数组的长度（即公司名字长度）

*/
void show_companyname(char company_name[3][116],int len){
    //显示公司名字--清饮堂
    int i;
    for(i=0;i<len;i++){
        lcd_draw_word(company_name[i],116,32,5+i*40,5,DarkGreen);
    }

}

void show_menu_price(double price,int x0,int y0){

    int integerPart = (int)price;
    double decimalPart = price - integerPart;
    decimalPart = (int)(decimalPart * 100);     // 将小数部分乘以100，转换为整数
    int decimalPart1 = decimalPart / 10;        // 取整数部分
    int decimalPart2 = decimalPart - (decimalPart1 * 10);   // 取余数部分

    lcd_draw_word(dollar,sizeof(dollar),32,x0 - 22 + 10,y0,BLACK);
    lcd_draw_number(integerPart,x0 + 16 + 10,y0,BLACK);
    lcd_draw_word(dot,sizeof(dot),16,x0 + 32 + 10,y0,BLACK);
    lcd_draw_number(decimalPart1, x0 + 48 + 10, y0,BLACK);
    lcd_draw_number(decimalPart2, x0 + 64 + 10, y0,BLACK);
}

struct Node
{
    //数据域
    char pic_name[20];
    //指针域
    struct Node *next;
};
struct Node *head = NULL; //指向链表头部的指针
struct Node *tail = NULL; //指向链表尾部的指针

//通过尾插法向一个链表中插入一个节点
void insert_node(char *data)
{
    //在内存上为新的节点申请空间
    struct Node *new;

    new = (struct Node *)malloc(sizeof(struct Node));
    new->next = NULL;
    memset(new->pic_name, 0, sizeof(new->pic_name));
    strcpy(new->pic_name, data);

    //判断链表是否为空
    if (NULL == head)
    {
        head = new;
        tail = new;
        return ;
    }
    tail->next = new;
    tail = new;
    tail->next = head;
}


// 利用链表实现广告循环滚动播放
void show_ad(){
    //printf("ad ...\n");
    insert_node("ad1.bmp");
    insert_node("ad2.bmp");
    insert_node("ad3.bmp");
    struct Node *tmp = head;
    while (tmp)
    {
        int fd;
        fd = open(tmp->pic_name, O_RDWR); // boot.bmp是开机壁纸文件
        if (-1 == fd)
        {
            perror("open ad bmp file ");
            return ;
        }
        char pixels[AD_WIDTH*AD_HEIGHT*3]; // boot.bmp文件的大小刚好为 1024 * 600
        unsigned short color;
        int i = 0;
        int end_line = -1;
        /*
         * end_line,代表最后一行目前的行数；
         *  初始值为-1，原因如下：
         *      将屏幕抽象成一个相框，在自上而下的放入照片时，照片未放入时
         *      照片位于相框x = 0的上一位，也就是x = -1
         */
        //因为bmp图片的前面54个字节保存了图片的先关信息比如名字 宽度 高度 位深，需要跳过前面的54个字节
        lseek(fd, 54, SEEK_SET);
        read(fd, pixels, sizeof(pixels)); //将bmp图片中的所有的像素点读取保存到一个数组中

        int x, y;
        for (y = 0; y < AD_HEIGHT; y++)  // 逐行刷新
        {
            if(end_line == -1){ //照片未放入的时候，只刷新第一行
                color = (pixels[i+2]>>3)<<11 | (pixels[i+1]>>2)<<5 | pixels[i]>>3;
                i += 3;
                //在LCD上显示像素点
                //*(unsigned short *)(plcd + (x+x0) + (h-1-y+y0)*1024) = color;
                for (x = 0; x < AD_WIDTH; x++)
                    lcd_draw_point(900 + x,150+y,color);
                /*
                 * 解释：为什么是：plcd + x + y*1024？
                 * 这个过程依然理解成是相片自上而下地放入相框
                 * 先进入相框的是相片的底部，又因为bmp图片的读取方式是从下自上
                 * 所以plcd + x + y*1024；
                 * */

                end_line++;// -1 --> 0
            }else{ //第一行刷新后
                int index = 0;
                int e;
                for(e = end_line+1;e != 0;e--){ //这个循环是将已经刷新的区域整体向下移动一行
                    for(index = 0;index<100;index++){
                        // 从已经刷新的最后一行的下一行开始，将之前的行数向下移动
                        *(unsigned short *)(plcd + index + (e+150)*1024 + 900) = *(unsigned short *)(plcd + index + (e-1 + 150)*1024 + 900);
                    }
                }
                for (x = 0; x < AD_WIDTH; x++){ // 载入最新的一行
                    color = (pixels[i+2]>>3)<<11 | (pixels[i+1]>>2)<<5 | pixels[i]>>3;
                    i += 3;
                    *(unsigned short *)(plcd + x + 900 + 150 * 1024) = color;
                }
                end_line++;
            }
        }
        usleep(1000000); // 延时1s进入界面
        tmp = tmp->next;
        close(fd);
    }
    return;
}

void *pthread_show_ad(){
    show_ad();
}

void show_menu(){
    // 商品及其价格显示
    lcd_draw_bmp("backgroud.bmp",1024,600,0,0);
    lcd_draw_bmp("coca_coke.bmp",100,165,50,100);
    show_menu_price(3.50,50,270);
    lcd_draw_bmp("cocoo.bmp",100,165,50 + ((165 + 50) * 1),100);
    show_menu_price(5.00,50 + ((165+50) * 1),270);
    lcd_draw_bmp("dfsy.bmp",100,165,50 + ((165 + 50) * 2),100);
    show_menu_price(5.00,50 + ((165 + 50) * 2),270);
    lcd_draw_bmp("pepsi.bmp",100,165,50 + ((165 + 50) * 3),100);
    show_menu_price(4.00,50 + ((165+50) * 3),270);
    lcd_draw_bmp("asm.bmp",100,165,50,300);
    show_menu_price(4.50,50,470);
    lcd_draw_bmp("redbull.bmp",100,165,50 + ((165 + 50) * 1),300);
    show_menu_price(5.00,50 + ((165+50) * 1),470);
    lcd_draw_bmp("guojiao.bmp",100,165,50 + ((165 + 50) * 2),300);
    show_menu_price(4.00,50 + ((165+50) * 2),470);
    lcd_draw_bmp("Tea.bmp",100,165,50 + ((165 + 50) * 3),300);
    show_menu_price(4.00,50 + ((165+50) * 3),470);

    //付款按钮显示 
    lcd_draw_bmp("button.bmp",100,100,900,480);

    // 时间显示
    show_time();
    // 公司名称显示
    show_companyname(company_name,3);
    // 公司成员显示
    show_person_name(person_name,11);
}

//付款按钮定位
void button_select(int x,int y,int *flag,int price){
    int x_button_b = 255;
    int x_button_e = 16;
    int y_button_b = 135;
    int y_button_e = 60;

    int v_x_button_b = 530;
    int v_x_button_e = 360;
    int v_y_button_b = 360;
    int v_y_button_e = 240;

    int a_x_button_b = 550;
    int a_x_button_e = 224;
    int a_y_button_b = 360;
    int a_y_button_e = 50;

    int x_button_over_min = 560;
    int x_button_over_max = 740;
    int y_button_over_min = 40;
    int y_button_over_max = 130;

    
    if(x > x_button_e && x < x_button_b && y > y_button_e && y < y_button_b)//取消按钮
    {
        printf("c\n");
        *flag = 0;      //返回主菜单后，离开付款界面，继续选择商品（停止本次循环，继续下一次循环）
        lcd_init();
        show_menu();    //显示主菜单
    }
    else if(x > v_x_button_e && x < v_x_button_b && y > v_y_button_e && y < v_y_button_b)
    {
        lcd_init();
        lcd_draw_bmp("vpay.bmp",440,560,292,0);     // 显示微信支付码
        lcd_draw_bmp("cancle.bmp",200,100,700,420); //取消支付
        lcd_draw_bmp("success.bmp",200,100,90,420); //支付成功
        
    }
    else if(x > a_x_button_e && x < a_x_button_b && y > a_y_button_e && y < a_y_button_b)
    {
        lcd_init();
        lcd_draw_bmp("apay.bmp",400,600,292,0);     // 显示支付宝支付码
        lcd_draw_bmp("cancle.bmp",200,100,700,420);
        lcd_draw_bmp("success.bmp",200,100,90,420); //支付成功
    }
    else if(x >= x_button_over_min && x <= x_button_over_max && y >= y_button_over_min && y <= y_button_over_max)
    {
        printf("success\n");
        lcd_init();
        lcd_draw_bmp("over.bmp",1024,600,0,0);  //显示付款成功界面
        char str[20] = "";
        sprintf(str,"支付成功%d元 嘿嘿",price);
        talk(str,sizeof(str));
        talk("欢迎下次再来，再见哟",sizeof("欢迎下次再来，再见哟"));
        usleep(5000000);    //5秒后
        lcd_init();
        show_menu();    //显示主菜单
    }
}

void button(int pro,int pro_num,char* message,int len){
    int flag = 1;   
    int x, y,i;
    // 进入新界面，初始化动作-刷白屏幕
    lcd_init();
    talk(message,len);
    
    // 待付款金额显示
    for(i = 0;i<4;i++)
        lcd_draw_word(fk[i],116,32,50 + (i * 32),100,BLACK);
    show_menu_price(goods_price[pro]*pro_num,180,100);

    // 付款码显示
    lcd_draw_bmp("vx.bmp",300,200,300,100);
    lcd_draw_bmp("ali.bmp",300,200,300,320);
    lcd_draw_bmp("cancle.bmp",200,100,700,420);

    

    while(flag) 
    {
        get_touchscreen_coordinate(&x, &y);
        button_select(x,y,&flag,(int)goods_price[pro] *pro_num);   //定位付款按钮
    }
    usleep(1000000);
}

#endif //SHARE_MENU_H