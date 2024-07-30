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

//初始化串口
//file: 串口所对应的文件名
//baudrate：波特率 串口1秒钟能够传输的最大bit数
//返回值 串口的文件描述符
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
    //清空结构体
    memset(&myserial, 0, sizeof (myserial));
    //O_RDWR
    myserial.c_cflag |= (CLOCAL | CREAD);
    //设置控制模式状态，本地连接，接受使能
    //设置 数据位
    myserial.c_cflag &= ~CSIZE;   //清空数据位
    myserial.c_cflag &= ~CRTSCTS; //无硬件流控制
    myserial.c_cflag |= CS8;      //数据位:8

    myserial.c_cflag &= ~CSTOPB;//   //1位停止位
    myserial.c_cflag &= ~PARENB;  //不要校验
    //myserial.c_iflag |= IGNPAR;   //不要校验
    //myserial.c_oflag = 0;  //输入模式
    //myserial.c_lflag = 0;  //不激活终端模式

    switch (baudrate)
    {
        case 9600:
            cfsetospeed(&myserial, B9600);  //设置波特率
            cfsetispeed(&myserial, B9600);
            break;
        case 115200:
            cfsetospeed(&myserial, B115200);  //设置波特率
            cfsetispeed(&myserial, B115200);
            break;
        case 19200:
            cfsetospeed(&myserial, B19200);  //设置波特率
            cfsetispeed(&myserial, B19200);
            break;
    }

    /* 刷新输出队列,清除正接受的数据 */
    tcflush(fd, TCIFLUSH);

    /* 改变配置 */
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
        //通过COM2将命令发送出去
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
            sprintf(message,"重%d\n",value);
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
