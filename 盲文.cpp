// 盲文chars.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//打开mang.bmp，输出out.txt

#include <iostream>
#include<io.h>
#include<fcntl.h>
#include<wchar.h>
#include"../../../header/bio_graphics.h"




 

/// <summary>
/// the bits are like
/// 0 3 \n
/// 1 4 \n
/// 2 5 \n
/// 6 7 \n
/// 
/// of number 0b76543210
/// 
/// //that really does not make sence, but it is encoded like this 
/// </summary>
/// <returns>3 bytes utf-8 blind char</returns>
void blind_char_utf8(uint8_t num, uint8_t ret[3])
{//e.g ⡈==0b01001000
    ret[0] = 0xe2;
    ret[1] = 0xa0+(num>>6);
    ret[2] = 0x80+(num&0b111111);
    return;
};

void blind_char_renderer(bmp* pic, FILE* utf8fp,int wid_gap,int hei_gap)
{
    uint8_t rn[] = { "\r\n" };
    fwrite(rn, 1, 2, utf8fp);
    int charwid = pic->width / (2+ wid_gap);
    int charhei = pic->height / (4 + hei_gap);
    uint8_t bchar = 0;
    uint8_t dots[8] = { 0 };
    int picx = 0, picy = 0;
    for (int hei = 0; hei < charhei; hei++) {
        for (int wid = 0; wid < charwid; wid++)
        {
            pic->x = wid * (2 + wid_gap);
            pic->y = hei * (4 + hei_gap);
            pic->pointread_r();
            dots[0] = (pic->rgb[0] > 128) ? 0 : 1;
            pic->y++;
            pic->pointread_r();
            dots[1] = (pic->rgb[0] > 128) ? 0 : 1;
            pic->y++;
            pic->pointread_r();
            dots[2] = (pic->rgb[0] > 128) ? 0 : 1;
            pic->y++;
            pic->pointread_r();
            dots[6] = (pic->rgb[0] > 128) ? 0 : 1;

            pic->y -= 3;
            pic->x++;

            pic->pointread_r();
            dots[3] = (pic->rgb[0] > 128) ? 0 : 1;
            pic->y++;
            pic->pointread_r();
            dots[4] = (pic->rgb[0] > 128) ? 0 : 1;
            pic->y++;
            pic->pointread_r();
            dots[5] = (pic->rgb[0] > 128) ? 0 : 1;
            pic->y++;
            pic->pointread_r();
            dots[7] = (pic->rgb[0] > 128) ? 0 : 1;

            bchar = dots[7];
            for (int bc = 6; bc >=0; bc--)
            {
                bchar <<= 1;
                bchar += dots[bc];
            }
            uint8_t input[3] = { 0 };
            blind_char_utf8(bchar, input);
            fwrite(input, 3, 1, utf8fp);
        }
        fwrite(rn, 1, 2, utf8fp);
    }
}
int main()
{
    FILE* fout;
    bmp* pic = new bmp("mang.bmp");
    fopen_s(&fout, "out.txt", "wb+");
    uint8_t bchar = 0,ret[3]={0};
    blind_char_renderer(pic, fout, 1,1);

    _fcloseall();
    system("out.txt");
}

