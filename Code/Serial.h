//
// Created by 11275 on 2023/12/6.
//

#ifndef SHARE_SERIAL_H
#define SHARE_SERIAL_H

#define COM2 "/dev/ttymxc1"
#define COM3 "/dev/ttymxc2"


#define dbgOut(arg...) \
do{ \
	char b__[1024]; \
	sprintf(b__, arg); \
	fprintf(stdout, "[%s,%s,%d] %s", __FILE__, __func__, __LINE__, b__); \
}while(0)

//��ʼ������
//file: ��������Ӧ���ļ���
//baudrate�������� ����1�����ܹ���������bit��
//����ֵ ���ڵ��ļ�������
int init_serial(const char *file, int baudrate)
{
    int fd;

    fd = open(file, O_RDWR);
    if (fd == -1)
    {
        perror("open device error:");
        return -1;
    }

    struct termios myserial;
    //��սṹ��
    memset(&myserial, 0, sizeof (myserial));
    //O_RDWR
    myserial.c_cflag |= (CLOCAL | CREAD);
    //���ÿ���ģʽ״̬���������ӣ�����ʹ��
    //���� ����λ
    myserial.c_cflag &= ~CSIZE;   //�������λ
    myserial.c_cflag &= ~CRTSCTS; //��Ӳ��������
    myserial.c_cflag |= CS8;      //����λ:8

    myserial.c_cflag &= ~CSTOPB;//   //1λֹͣλ
    myserial.c_cflag &= ~PARENB;  //��ҪУ��
    //myserial.c_iflag |= IGNPAR;   //��ҪУ��
    //myserial.c_oflag = 0;  //����ģʽ
    //myserial.c_lflag = 0;  //�������ն�ģʽ

    switch (baudrate)
    {
        case 9600:
            cfsetospeed(&myserial, B9600);  //���ò�����
            cfsetispeed(&myserial, B9600);
            break;
        case 115200:
            cfsetospeed(&myserial, B115200);  //���ò�����
            cfsetispeed(&myserial, B115200);
            break;
        case 19200:
            cfsetospeed(&myserial, B19200);  //���ò�����
            cfsetispeed(&myserial, B19200);
            break;
    }

    /* ˢ���������,��������ܵ����� */
    tcflush(fd, TCIFLUSH);

    /* �ı����� */
    tcsetattr(fd, TCSANOW, &myserial);

    return fd;
}

void talk(char *message,int len){
    int com3_fd;
    com3_fd = init_serial(COM3,9600);
    write(com3_fd,message, len);
    //usleep(100000);
}

void Pressure(){
    int com2_fd;
    com2_fd = init_serial(COM2,9600);
    char cmd[5] = {0xA3,0,0xA2,0xA4,0xA5};
    while (1)
    {
        //ͨ��COM2������ͳ�ȥ
        write(com2_fd, cmd, sizeof(cmd));
        char buf[10];
        read(com2_fd, buf, sizeof(buf));
        unsigned int value;
        value = buf[4]<<16 | buf[5] << 8 | buf[6];

        for (int k = 0; k < 10; k++)
            printf("%x ", buf[k]);
        printf("\n");
        dbgOut("value: %d\n", value);

        if(value != 0){
            char message[30] = "";
            sprintf(message,"��%d\n",value);
            //printf(message);
            talk(message,sizeof (message));
            //usleep(5000000);
        }
        usleep(1000000);
    }
}

void *Pressure_pthread(){
    Pressure();
}

#endif //SHARE_SERIAL_H
