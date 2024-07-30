//=====显示选择的商品名称(及数量)=====

#ifndef SHARE_SHOW_GOODS_H
#define SHARE_SHOW_GOODS_H



// 显示商品及数量的屏幕区域初始化（刷白）
void show_init()
{
    int x,y;
    for (y = 550; y < 580; y++) 
    {
        for (x = 50; x < 280; x++) 
        {
            unsigned short color = 0xFFFF;
            lcd_draw_point(x , y , color);
        }
    }
}

// 显示商品名称
void show_coke(int pro_num)
{
    show_init();
    int i;
    for(i=0;i<2;i++)
        lcd_draw_word(coke[i],116,32,50 + (i * 35),550,BLACK);    
    lcd_draw_number(pro_num, 230, 550, BLACK);
    lcd_draw_word(bottle,116,32,250,550,BLACK); 
        
}

void show_yeshu(int pro_num){
    show_init();
    int i;
    for(i=0;i<4;i++)
        lcd_draw_word(ys[i],116,32,50 + (i * 35),550,BLACK);
    lcd_draw_number(pro_num, 230, 550, BLACK);
    lcd_draw_word(bottle,116,32,250,550,BLACK); 
}

void show_dfsy(int pro_num){
    show_init();
    int i;
    for(i=0;i<4;i++)
        lcd_draw_word(dfsy[i],116,32,50 + (i * 35),550,BLACK);
    lcd_draw_number(pro_num, 230, 550, BLACK);
    lcd_draw_word(bottle,116,32,250,550,BLACK); 
}

void show_rbsk(int pro_num){
    show_init();
    int i;
    for(i=0;i<3;i++)
        lcd_draw_word(rbsk[i],116,32,50 + (i * 35),550,BLACK);
    lcd_draw_number(pro_num, 230, 550, BLACK);
    lcd_draw_word(bottle,116,32,250,550,BLACK); 
}

void show_asm(int pro_num){
    show_init();
    int i;
    for(i=0;i<5;i++)
        lcd_draw_word(asmnc[i],116,32,50 + (i * 35),550,BLACK);
    lcd_draw_number(pro_num, 230, 550, BLACK);
    lcd_draw_word(bottle,116,32,250,550,BLACK); 
}

void show_hn(int pro_num){
    show_init();
    int i;
    for(i=0;i<2;i++)
        lcd_draw_word(hn[i],116,32,50 + (i * 35),550,BLACK);
    lcd_draw_number(pro_num, 230, 550, BLACK);
    lcd_draw_word(bottle,116,32,250,550,BLACK); 
}

void show_bhc(int pro_num){
    show_init();
    int i;
    for(i=0;i<3;i++)
        lcd_draw_word(bhc[i],116,32,50 + (i * 35),550,BLACK);
    lcd_draw_number(pro_num, 230, 550, BLACK);
    lcd_draw_word(bottle,116,32,250,550,BLACK); 
}

void show_lc(int pro_num){
    show_init();
    int i;
    for(i=0;i<2;i++)
        lcd_draw_word(lc[i],116,32,50 + (i * 35),550,BLACK);
    lcd_draw_number(pro_num, 230, 550, BLACK);
    lcd_draw_word(bottle,116,32,250,550,BLACK); 
}

void select_goods(int pro,int pro_num){

    switch (pro) {
        case 0:{
            show_coke(pro_num);
            break;
        }
        case 1:{
            show_yeshu(pro_num);
            break;
        }
        case 2:{
            show_dfsy(pro_num);
            break;
        }
        case 3:{
            show_rbsk(pro_num);
            break;
        }
        case 4:{
            show_asm(pro_num);
            break;
        }
        case 5:{
            show_hn(pro_num);
            break;
        }
        case 6:{
            show_bhc(pro_num);
            break;
        }
        case 7:{
            show_lc(pro_num);
            break;
        }
    }}

#endif //SHARE_SHOW_GOODS_H
