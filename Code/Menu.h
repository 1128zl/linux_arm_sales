//
// Created by 11275 on 2023/12/6.
//

#ifndef SHARE_MENU_H
#define SHARE_MENU_H

double goods_price[8] = {3.50,5.00,5.00,4.00,4.50,5.00,4.00,4.00};

int lcd_init()
{
    //��LCD�豸�ļ�
    int fd;

    fd = open("/dev/fb0", O_RDWR);
    if (-1 == fd) //fd == -1
    {
        perror("open /dev/fb0 ");
        return -1;
    }

    int x, y;
    plcd = mmap(NULL, LCD_WIDTH*LCD_HEIGHT*2, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    // �����ʼ��Ϊ��ɫ
    for (y = 0; y < 600; y++)
    {
        for (x = 0; x < 1024; x++)
        {
            //��BMPͼƬ�ϵ�һ�����ص�(ռ24��bit)ת����LCD�ϵ�һ��ռ16��bit�����ص�
            unsigned short color = 0xFFFF;
            //��LCD����ʾ���ص�
            //*(unsigned short *)(plcd + (x+x0) + (h-1-y+y0)*1024) = color;
            lcd_draw_point(x, y, color);
        }
    }
    return fd;
}

/*
 * ���ϵ��¹���������Ƭ
 * */
void show_boot_animation(){
    int fd;
    fd = open("boot.bmp", O_RDWR);      // boot.bmp�ǿ�����ֽ�ļ�
    if (-1 == fd)
    {
        perror("open bmp file ");
        return ;
    }
    
    char pixels[LCD_WIDTH*LCD_HEIGHT*3]; // boot.bmp�ļ��Ĵ�С�պ�Ϊ 1024 * 600
    unsigned short color;
    int end_line = -1;      //  * end_line,����ͼƬĿǰ��ʾ�����һ�У�
    
    //��ΪbmpͼƬ��ǰ��54���ֽڱ�����ͼƬ���ȹ���Ϣ�������� ��� �߶� λ���Ҫ����ǰ���54���ֽ�
    lseek(fd, 54, SEEK_SET);
    read(fd, pixels, sizeof(pixels)); //��bmpͼƬ�е����е����ص��ȡ���浽һ��������
    int x,y,z;

    //�򿪴������ļ�
    int fd_screen;

    fd_screen = open("/dev/input/touchscreen0", O_RDWR);
    if (fd_screen == -1)
    {
        perror("open /dev/input/touchscreen0 ");
        return ;
    }

    // ͼƬ����
    for (z = 0; z < LCD_HEIGHT;)  // ����ˢ��
    {
        int i = 0;
        for(y = 0;y < z; y++)
        {
            for(x = 0;x < LCD_WIDTH;x++)
            {	
                color = (pixels[i+2]>>3)<<11 | (pixels[i+1]>>2)<<5 | pixels[i]>>3;
                i += 3;
                //��LCD����ʾ���ص�
                *(unsigned short *)(plcd + x + (z - y)*LCD_WIDTH) = color;	
            }
        }
        z+=5;
    }
    usleep(1000000);// һ����˳�
}

void show_time()
{
    //��ʼ����ʾ����������ص�
    int x, y;
    for (y = 30; y < 60; y++) {
        for (x = 700; x < 700+65+38+30+46+22+35+18+30; x++) {
            //��BMPͼƬ�ϵ�һ�����ص�(ռ24��bit)ת����LCD�ϵ�һ��ռ16��bit�����ص�
            unsigned short color =  0xFFFF;
            lcd_draw_point(x,y, color);
        }
    }

    //��ȡ��1970.1.1 00:00 ����������
    time_t t = time(NULL);
    struct tm *tm;
    tm = localtime(&t); //���ڻ�ȡ��ǰʱ�䲢�����ʾΪ����ʱ��

    //2023 12/4 17:21
    lcd_draw_number(tm->tm_year+1900, 700, 30, RED); //��ʾ��

    //��ʾ��
    if(tm->tm_mon+1>9)	// 10-12��
    {
        lcd_draw_number(tm->tm_mon+1, 700+65, 30, RED);
    }
    else	//01-09��
    {
        lcd_draw_word(numbers[0],sizeof(numbers[0]),16,700+65, 30,RED);
        lcd_draw_number(tm->tm_mon+1, 700+65+18, 30, RED);
    }

    //��ʾһ��б��
    lcd_draw_word(sign1, sizeof(sign1), 16,700+65+38, 30, RED);

    //��ʾ��
    if(tm->tm_mday>9)	// 10-31��
    {
        lcd_draw_number(tm->tm_mday, 700+65+38+30, 30, RED);
    }
    else	// 01-09��
    {
        lcd_draw_word(numbers[0],sizeof(numbers[0]),16, 700+65+38+30, 30,RED);
        lcd_draw_number(tm->tm_mday, 700+65+38+30+18, 30, RED);
    }

    //��ʾСʱ
    if(tm->tm_hour>9) // 10-23ʱ
    {
        lcd_draw_number(tm->tm_hour, 700+65+38+30+46, 30, RED);
    }
    else	//0-09ʱ
    {
        lcd_draw_word(numbers[0],sizeof(numbers[0]),16, 700+65+38+30+30, 30,RED);
        lcd_draw_number(tm->tm_hour, 700+65+38+30+46+2, 30, RED);
    }

    //��ʾһ��ð��
    lcd_draw_word(sign2, sizeof(sign2), 32,700+80+38+30+46+5, 30, RED);

    // ��ʾ����
    if(tm->tm_min>9)	// 10-59��
    {
        lcd_draw_number(tm->tm_min, 700+65+38+30+46+22+35, 30, RED);
    }
    else	//01-09��
    {
        lcd_draw_word(numbers[0],sizeof(numbers[0]),16, 700+65+38+30+46+22+35, 30,RED);
        lcd_draw_number(tm->tm_min, 700+65+38+30+46+22+35+18, 30, RED);
    }
}

void timer_handler(int signum)
{
    show_time();//��ʾ��ǰ��ʱ��
}

//����һ��60��Ķ�ʱ
void set_timer()
{
    struct itimerval it, oldit;
    int ret;

    it.it_value.tv_sec = 5;	 //5���Ժ�ͻ������һ���ź�
    it.it_value.tv_usec = 0;
    it.it_interval.tv_sec = 60;	//��ʱ60��
    it.it_interval.tv_usec = 0;

    //����һ��SIGALRM�źŴ������������Ƕ�ʱ��ʱ��һ��ϵͳ���Զ�����һ��SIGALRMһ������źŲ���
    //���Զ��������������õĺ���
    signal(SIGALRM, timer_handler);
    //���ö�ʱ
    setitimer(ITIMER_REAL, &it, &oldit);
}

/*
	@brief 				��ʾ��˾����������
	@param person_name	���������������+��˾���������ֵ�c51��ʽ�µ�ȡģ������
	@param len			����person_name�ĳ���
*/
void show_person_name(char person_name[11][116],int len) {
    //��˾���������ʾ����ʾЧ���������� �������������
    int i;
    //������ʾ������
    for(i=0;i<len;i++)
    {
        //����Ҫ��ʾ�����ִ�����(5,40)��ʼ������ʾ��������֮����8�����ص�
        lcd_draw_word(person_name[i],116,32,5+i*40,40,BLACK);
    }
}

/*
	@brief 				��LCD���Ϸ���ʾ�Զ��ۻ���������˾�Լ�������
	@param company_name ��˾���ֶ�Ӧ��C51��ʽ��Ӧ��ȡģ����ŵ�����
	@param len  		��˾����C51��ʽ��Ӧ��ȡģ������ĳ��ȣ�����˾���ֳ��ȣ�

*/
void show_companyname(char company_name[3][116],int len){
    //��ʾ��˾����--������
    int i;
    for(i=0;i<len;i++){
        lcd_draw_word(company_name[i],116,32,5+i*40,5,DarkGreen);
    }

}

void show_menu_price(double price,int x0,int y0){

    int integerPart = (int)price;
    double decimalPart = price - integerPart;
    decimalPart = (int)(decimalPart * 100);     // ��С�����ֳ���100��ת��Ϊ����
    int decimalPart1 = decimalPart / 10;        // ȡ��������
    int decimalPart2 = decimalPart - (decimalPart1 * 10);   // ȡ��������

    lcd_draw_word(dollar,sizeof(dollar),32,x0 - 22 + 10,y0,BLACK);
    lcd_draw_number(integerPart,x0 + 16 + 10,y0,BLACK);
    lcd_draw_word(dot,sizeof(dot),16,x0 + 32 + 10,y0,BLACK);
    lcd_draw_number(decimalPart1, x0 + 48 + 10, y0,BLACK);
    lcd_draw_number(decimalPart2, x0 + 64 + 10, y0,BLACK);
}

struct Node
{
    //������
    char pic_name[20];
    //ָ����
    struct Node *next;
};
struct Node *head = NULL; //ָ������ͷ����ָ��
struct Node *tail = NULL; //ָ������β����ָ��

//ͨ��β�巨��һ�������в���һ���ڵ�
void insert_node(char *data)
{
    //���ڴ���Ϊ�µĽڵ�����ռ�
    struct Node *new;

    new = (struct Node *)malloc(sizeof(struct Node));
    new->next = NULL;
    memset(new->pic_name, 0, sizeof(new->pic_name));
    strcpy(new->pic_name, data);

    //�ж������Ƿ�Ϊ��
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


// ��������ʵ�ֹ��ѭ����������
void show_ad(){
    //printf("ad ...\n");
    insert_node("ad1.bmp");
    insert_node("ad2.bmp");
    insert_node("ad3.bmp");
    struct Node *tmp = head;
    while (tmp)
    {
        int fd;
        fd = open(tmp->pic_name, O_RDWR); // boot.bmp�ǿ�����ֽ�ļ�
        if (-1 == fd)
        {
            perror("open ad bmp file ");
            return ;
        }
        char pixels[AD_WIDTH*AD_HEIGHT*3]; // boot.bmp�ļ��Ĵ�С�պ�Ϊ 1024 * 600
        unsigned short color;
        int i = 0;
        int end_line = -1;
        /*
         * end_line,�������һ��Ŀǰ��������
         *  ��ʼֵΪ-1��ԭ�����£�
         *      ����Ļ�����һ����������϶��µķ�����Ƭʱ����Ƭδ����ʱ
         *      ��Ƭλ�����x = 0����һλ��Ҳ����x = -1
         */
        //��ΪbmpͼƬ��ǰ��54���ֽڱ�����ͼƬ���ȹ���Ϣ�������� ��� �߶� λ���Ҫ����ǰ���54���ֽ�
        lseek(fd, 54, SEEK_SET);
        read(fd, pixels, sizeof(pixels)); //��bmpͼƬ�е����е����ص��ȡ���浽һ��������

        int x, y;
        for (y = 0; y < AD_HEIGHT; y++)  // ����ˢ��
        {
            if(end_line == -1){ //��Ƭδ�����ʱ��ֻˢ�µ�һ��
                color = (pixels[i+2]>>3)<<11 | (pixels[i+1]>>2)<<5 | pixels[i]>>3;
                i += 3;
                //��LCD����ʾ���ص�
                //*(unsigned short *)(plcd + (x+x0) + (h-1-y+y0)*1024) = color;
                for (x = 0; x < AD_WIDTH; x++)
                    lcd_draw_point(900 + x,150+y,color);
                /*
                 * ���ͣ�Ϊʲô�ǣ�plcd + x + y*1024��
                 * ���������Ȼ��������Ƭ���϶��µط������
                 * �Ƚ�����������Ƭ�ĵײ�������ΪbmpͼƬ�Ķ�ȡ��ʽ�Ǵ�������
                 * ����plcd + x + y*1024��
                 * */

                end_line++;// -1 --> 0
            }else{ //��һ��ˢ�º�
                int index = 0;
                int e;
                for(e = end_line+1;e != 0;e--){ //���ѭ���ǽ��Ѿ�ˢ�µ��������������ƶ�һ��
                    for(index = 0;index<100;index++){
                        // ���Ѿ�ˢ�µ����һ�е���һ�п�ʼ����֮ǰ�����������ƶ�
                        *(unsigned short *)(plcd + index + (e+150)*1024 + 900) = *(unsigned short *)(plcd + index + (e-1 + 150)*1024 + 900);
                    }
                }
                for (x = 0; x < AD_WIDTH; x++){ // �������µ�һ��
                    color = (pixels[i+2]>>3)<<11 | (pixels[i+1]>>2)<<5 | pixels[i]>>3;
                    i += 3;
                    *(unsigned short *)(plcd + x + 900 + 150 * 1024) = color;
                }
                end_line++;
            }
        }
        usleep(1000000); // ��ʱ1s�������
        tmp = tmp->next;
        close(fd);
    }
    return;
}

void *pthread_show_ad(){
    show_ad();
}

void show_menu(){
    // ��Ʒ����۸���ʾ
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

    //���ť��ʾ 
    lcd_draw_bmp("button.bmp",100,100,900,480);

    // ʱ����ʾ
    show_time();
    // ��˾������ʾ
    show_companyname(company_name,3);
    // ��˾��Ա��ʾ
    show_person_name(person_name,11);
}

//���ť��λ
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

    
    if(x > x_button_e && x < x_button_b && y > y_button_e && y < y_button_b)//ȡ����ť
    {
        printf("c\n");
        *flag = 0;      //�������˵����뿪������棬����ѡ����Ʒ��ֹͣ����ѭ����������һ��ѭ����
        lcd_init();
        show_menu();    //��ʾ���˵�
    }
    else if(x > v_x_button_e && x < v_x_button_b && y > v_y_button_e && y < v_y_button_b)
    {
        lcd_init();
        lcd_draw_bmp("vpay.bmp",440,560,292,0);     // ��ʾ΢��֧����
        lcd_draw_bmp("cancle.bmp",200,100,700,420); //ȡ��֧��
        lcd_draw_bmp("success.bmp",200,100,90,420); //֧���ɹ�
        
    }
    else if(x > a_x_button_e && x < a_x_button_b && y > a_y_button_e && y < a_y_button_b)
    {
        lcd_init();
        lcd_draw_bmp("apay.bmp",400,600,292,0);     // ��ʾ֧����֧����
        lcd_draw_bmp("cancle.bmp",200,100,700,420);
        lcd_draw_bmp("success.bmp",200,100,90,420); //֧���ɹ�
    }
    else if(x >= x_button_over_min && x <= x_button_over_max && y >= y_button_over_min && y <= y_button_over_max)
    {
        printf("success\n");
        lcd_init();
        lcd_draw_bmp("over.bmp",1024,600,0,0);  //��ʾ����ɹ�����
        char str[20] = "";
        sprintf(str,"֧���ɹ�%dԪ �ٺ�",price);
        talk(str,sizeof(str));
        talk("��ӭ�´��������ټ�Ӵ",sizeof("��ӭ�´��������ټ�Ӵ"));
        usleep(5000000);    //5���
        lcd_init();
        show_menu();    //��ʾ���˵�
    }
}

void button(int pro,int pro_num,char* message,int len){
    int flag = 1;   
    int x, y,i;
    // �����½��棬��ʼ������-ˢ����Ļ
    lcd_init();
    talk(message,len);
    
    // ����������ʾ
    for(i = 0;i<4;i++)
        lcd_draw_word(fk[i],116,32,50 + (i * 32),100,BLACK);
    show_menu_price(goods_price[pro]*pro_num,180,100);

    // ��������ʾ
    lcd_draw_bmp("vx.bmp",300,200,300,100);
    lcd_draw_bmp("ali.bmp",300,200,300,320);
    lcd_draw_bmp("cancle.bmp",200,100,700,420);

    

    while(flag) 
    {
        get_touchscreen_coordinate(&x, &y);
        button_select(x,y,&flag,(int)goods_price[pro] *pro_num);   //��λ���ť
    }
    usleep(1000000);
}

#endif //SHARE_MENU_H