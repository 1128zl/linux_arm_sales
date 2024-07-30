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
    //�������������е�ÿһ��Ԫ��
    for (m = 0; m < len; m++)
    {
        //����ȡ����7λ~��0λ��ֵ
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
        //��ʾn
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

    //��ΪbmpͼƬ��ǰ��54���ֽڱ�����ͼƬ���ȹ���Ϣ�������� ��� �߶� λ���Ҫ����ǰ���54���ֽ�
    lseek(fd, 54, SEEK_SET);
    read(fd, pixels, sizeof(pixels)); //��bmpͼƬ�е����е����ص��ȡ���浽һ��������

    int x, y;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            //��BMPͼƬ�ϵ�һ�����ص�(ռ24��bit)ת����LCD�ϵ�һ��ռ16��bit�����ص�
            color = (pixels[i + 2] >> 3) << 11 | (pixels[i + 1] >> 2) << 5 | pixels[i] >> 3;
            i += 3;
            //��LCD����ʾ���ص�
            //*(unsigned short *)(plcd + (x+x0) + (h-1-y+y0)*1024) = color;
            lcd_draw_point(x + x0, h - 1 - y + y0, color);
        }
    }

    close(fd);
}


#endif //SHARE_LCD_H
