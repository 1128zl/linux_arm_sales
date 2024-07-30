//
// Created by 11275 on 2023/12/6.
//
#ifndef SHARE_LCD_H
#define SHARE_LCD_H
#define LCD_WIDTH  1024
#define LCD_HEIGHT 600
#define AD_WIDTH 100
#define AD_HEIGHT 300

#define LCD_INIT_COLOR WHITE
#define WHITE 0xFFFF
#define BLACK 0
#define RED   0xF800
#define GREEN 0x07E0
#define BLUE  0x001F
#define DarkGreen 0x0597

unsigned short *plcd;


#define lcd_draw_point(x,y,color) 	*(unsigned short *)(plcd + (x) + (y)*LCD_WIDTH) = color

void lcd_draw_word(char *ch, int len, int w, int x0, int y0, unsigned short color)
{
    int i, m, x, y;
    //遍历点阵数组中的每一个元素
    for (m = 0; m < len; m++)
    {
        //依次取出第7位~第0位的值
        for (i = 7; i >= 0; i--)
        {
            if (ch[m] & (1<<i))
            {
                x = (7-i)+8*(m%(w/8)) + x0;
                y = m/(w/8) + y0;
                lcd_draw_point(x,y,BLACK);
            }
        }
    }
}


void lcd_draw_number(unsigned number, int x0, int y0, unsigned short color)
{
    int n, i = 0;
    if (number == 0){
        lcd_draw_word(numbers[0],sizeof(numbers[0]),16, x0, y0, color);
        return;
    }
    while (number)
    {
        n = number % 10;
        //显示n
        lcd_draw_word(numbers[n],sizeof(numbers[n]),16, x0-18*i, y0, color);
        i++;
        number /= 10;
    }
}

void lcd_draw_bmp(char *name, int w, int h, int x0, int y0) {
    int fd;
    fd = open(name, O_RDWR);
    if (-1 == fd) {
        perror("open bmp file ");
        return;
    }

    char pixels[w * h * 3];
    unsigned short color;
    int i = 0;

    //因为bmp图片的前面54个字节保存了图片的先关信息比如名字 宽度 高度 位深，需要跳过前面的54个字节
    lseek(fd, 54, SEEK_SET);
    read(fd, pixels, sizeof(pixels)); //将bmp图片中的所有的像素点读取保存到一个数组中

    int x, y;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            //将BMP图片上的一个像素点(占24个bit)转换成LCD上的一个占16个bit的像素点
            color = (pixels[i + 2] >> 3) << 11 | (pixels[i + 1] >> 2) << 5 | pixels[i] >> 3;
            i += 3;
            //在LCD上显示像素点
            //*(unsigned short *)(plcd + (x+x0) + (h-1-y+y0)*1024) = color;
            lcd_draw_point(x + x0, h - 1 - y + y0, color);
        }
    }

    close(fd);
}


#endif //SHARE_LCD_H
