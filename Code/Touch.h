#ifndef SHARE_TOUCH_H
#define SHARE_TOUCH_H
char product_C[8][20]={"�ɿڿ���","Ҭ��Ҭ֭","������Ҷ","�ձ�������","����ķ�̲�","��ţ","�����","�̲�"};

// ���ݴ����������ж�ѡ�����Ʒ����ѡ�����Ʒʱ��������ť���븶����棬ȡ��֧���󷵻����˵�
int select_product(int x,int y,int Pro_select,int* pro_num)      //Pro_select�ϴ�ѡ������Ʒ���
{

    printf("SELECT PRODUCE ...\n");
    int pro=0;      //pro���δ�����Ʒ���
    int x_min[8]={650,510,350,165,650,510,350,165};
    int x_max[8]={780,615,415,240,780,615,415,240};
    int y_min[8]={260,260,260,260,80,80,80,80};
    int y_max[8]={415,415,415,415,240,240,240,240};
    

    int x_button_b = 110;
    int x_button_e = 5;
    int y_button_b = 100;
    int y_button_e = 0;

    // �жϱ���ѡ�����Ʒ
    while(pro<8)
    {
        if(x>=x_min[pro] && x<=x_max[pro] && y>=y_min[pro] && y<=y_max[pro]){
            if(pro!=Pro_select) *pro_num=1;
            else    (*pro_num)++;
            return pro;
        }
        pro++;
    }


    //�ж��Ƿ������ť
    if(x > x_button_e && x < x_button_b && y > y_button_e && y < y_button_b){

        if(Pro_select != -1){    
            char str[100] = "";
            sprintf(str,"%s %dƿ",product_C[Pro_select],*pro_num);
            talk(str,sizeof(str));  //���ϴ�ѡ������Ʒ��ǰ���£����븶�����
            button(Pro_select,*pro_num); 
        }
            *pro_num=0;
    }

    return -1;
}

//��ȡ�����������
int get_touchscreen_coordinate(int *x, int *y)
{
    //�򿪴������ļ�
    int fd;

    fd = open("/dev/input/touchscreen0", O_RDWR);
    if (fd == -1)
    {
        perror("open /dev/input/touchscreen0 ");
        return 0;
    }

    //��ȡ�����������(��ʵ���Ƕ�ȡ������)
    struct input_event ev;
    while(1)
    {
        read(fd, &ev, sizeof(ev)); //�����ǰ��´�������ʱ��������Ϣ����������ev���
        //ע��: x��y������Ҫͨ�����ζ�ȡ���ܹ���ȡ���
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
        //�ж����Ƿ��ɿ�
        if (ev.type == EV_ABS && ev.code == ABS_PRESSURE && ev.value == 0)
        {
            return 0;
        }
    }
}


#endif //SHARE_TOUCH_H
