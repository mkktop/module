#include "oled.h"

/**
 * OLED�Դ�����
 * ���е���ʾ��������ֻ�ǶԴ��Դ�������ж�д
 * ������OLED_Update������OLED_UpdateArea����
 * �ŻὫ�Դ���������ݷ��͵�OLEDӲ����������ʾ
 */
uint8_t OLED_DisplayBuf[8][128];

/**
 * ��    ����OLEDд����
 * ��    ����Data Ҫд�����ݵ���ʼ��ַ
 * ��    ����Count Ҫд�����ݵ�����
 * �� �� ֵ����
 */
static void OLED_WriteData(uint8_t bytes[], uint8_t length)
{
    HAL_I2C_Mem_Write(&I2C, DEV_ADDR, DATA_MODE, I2C_MEMADD_SIZE_8BIT, bytes, length, 100);
}

/**
 * ��    ����OLEDд����
 * ��    ����Command Ҫд�������ֵ����Χ��0x00 ~ 0xFF
 * �� �� ֵ����
 */
static void OLED_WriteCmd(uint8_t bytes[], uint8_t length)
{
    HAL_I2C_Mem_Write(&I2C, DEV_ADDR, CMD_MODE, I2C_MEMADD_SIZE_8BIT, bytes, length, 100);
}

/**
 * ��    ����OLED��ʼ��
 * ��    ������
 * �� �� ֵ����
 * ˵    ����ʹ��ǰ����Ҫ���ô˳�ʼ������
 */
void OLED_Init(void)
{
    uint8_t cmd_bytes[23];
    // �ر���Ļ
    cmd_bytes[0] = 0xAE;
    // ������ʾʱ�ӷ�Ƶ��/����Ƶ��
    cmd_bytes[1] = 0xD5;
    // 0x00 ~ 0xFF
    cmd_bytes[2] = 0x80;
    // ���ö�·������
    cmd_bytes[3] = 0xA8;
    // 0x0E ~ 0x3F
    cmd_bytes[4] = 0x3F;
    // ������ʾƫ��
    cmd_bytes[5] = 0xD3;
    // 0x00 ~ 0x7F
    cmd_bytes[6] = 0x00;
    // ������ʾ��ʼ�У�0x40 ~ 0x7F
    cmd_bytes[7] = 0x40;
    // �������ҷ���0xA1������0xA0���ҷ���
    cmd_bytes[8] = 0xA1;
    // �������·���0xC8������0xC0���·���
    cmd_bytes[9] = 0xC8;
    // ����COM����Ӳ������
    cmd_bytes[10] = 0xDA;
    // 0x02 ~ 0x12
    cmd_bytes[11] = 0x12;
    // ���öԱȶ�
    cmd_bytes[12] = 0x81;
    // 0x00 ~ 0xFF
    cmd_bytes[13] = 0xCF;
    // ����Ԥ�������
    cmd_bytes[14] = 0xD9;
    // 0xF1 ~ 0xFF
    cmd_bytes[15] = 0xF1;
    // ����VCOMHȡ��ѡ�񼶱�
    cmd_bytes[16] = 0xDB;
    // 0x00 ~ 0x40
    cmd_bytes[17] = 0x40;
    // ����������ʾ��/�ر�
    cmd_bytes[18] = 0xA4;
    // ��������/��ɫ��ʾ��0xA6������0xA7��ɫ
    cmd_bytes[19] = 0xA6;
    // ���ó���
    cmd_bytes[20] = 0x8D;
    // 0x10 ~ 0x14
    cmd_bytes[21] = 0x14;
    // ������ʾ
    cmd_bytes[22] = 0xAF;
    // ��������
    OLED_WriteCmd(cmd_bytes, 23);
    // ����
    OLED_Clear();
    OLED_Update();
}

/**
 * ��    ����OLED������ʾָ��λ��
 * ��    ����Page ָ��������ڵ�ҳ����Χ��0 ~ 7
 * ��    ����column ָ��������ڵ������꣬��Χ��0 ~ 127
 * �� �� ֵ����
 * ˵    ����OLEDĬ�ϵ�X�ᣬֻ��8��BitΪһ��д�룬��1ҳ����8��X������
 */
void OLED_SetPointer(uint8_t Page, uint8_t column)
{
    uint8_t cmd_bytes[3];
    // ����ҳ��ַ
    cmd_bytes[0] = 0xB0 | Page;
    // �����е�ַ
    cmd_bytes[1] = 0x0F & column;
    cmd_bytes[2] = 0x10 | (column >> 4);
    OLED_WriteCmd(cmd_bytes, 3);
}

/**
 * ��    ������OLED�Դ�������µ�OLED��Ļ
 * ��    ������
 * �� �� ֵ����
 * ˵    �������е���ʾ��������ֻ�Ƕ�OLED�Դ�������ж�д
 *           ������OLED_Update������OLED_UpdateArea����
 *           �ŻὫ�Դ���������ݷ��͵�OLEDӲ����������ʾ
 *           �ʵ�����ʾ������Ҫ�������س�������Ļ�ϣ�������ø��º���
 */
void OLED_Update(void)
{
    uint8_t i;
    /*����ÿһҳ*/
    for (i = 0; i < 8; i++) {
        /*���ù��λ��Ϊÿһҳ�ĵ�һ��*/
        OLED_SetPointer(i, 0);
        /*����д��128�����ݣ����Դ����������д�뵽OLEDӲ��*/
        OLED_WriteData(OLED_DisplayBuf[i], 128);
    }
}

/**
 * ��    ������OLED�Դ����鲿�ָ��µ�OLED��Ļ
 * ��    ����X ָ���������Ͻǵĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
 * ��    ����Y ָ���������Ͻǵ������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
 * ��    ����Width ָ������Ŀ�ȣ���Χ��0 ~ 128
 * ��    ����Height ָ������ĸ߶ȣ���Χ��0 ~ 64
 * �� �� ֵ����
 * ˵    �����˺��������ٸ��²���ָ��������
 *           �����������Y��ֻ��������ҳ����ͬһҳ��ʣ�ಿ�ֻ����һ�����
 * ˵    �������е���ʾ��������ֻ�Ƕ�OLED�Դ�������ж�д
 *           ������OLED_Update������OLED_UpdateArea����
 *           �ŻὫ�Դ���������ݷ��͵�OLEDӲ����������ʾ
 *           �ʵ�����ʾ������Ҫ�������س�������Ļ�ϣ�������ø��º���
 */
void OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
    int16_t j;
    int16_t Page, Page1;

    /*���������ڼ���ҳ��ַʱ��Ҫ��һ��ƫ��*/
    /*(Y + Height - 1) / 8 + 1��Ŀ����(Y + Height) / 8������ȡ��*/
    Page  = Y / 8;
    Page1 = (Y + Height - 1) / 8 + 1;
    if (Y < 0) {
        Page -= 1;
        Page1 -= 1;
    }

    /*����ָ�������漰�����ҳ*/
    for (j = Page; j < Page1; j++) {
        if (X >= 0 && X <= 127 && j >= 0 && j <= 7) // ������Ļ�����ݲ���ʾ
        {
            /*���ù��λ��Ϊ���ҳ��ָ����*/
            OLED_SetPointer(j, X);
            /*����д��Width�����ݣ����Դ����������д�뵽OLEDӲ��*/
            OLED_WriteData(&OLED_DisplayBuf[j][X], Width);
        }
    }
}

/**
 * ��    ������OLED����
 * ��    ������
 * �� �� ֵ����
 * ˵    �������ô˺�����OLED����
 */
void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++) // ����8ҳ
    {
        for (i = 0; i < 128; i++) // ����128��
        {
            OLED_DisplayBuf[j][i] = 0x00; // ���Դ���������ȫ������
        }
    }
    // OLED_Update();
}

/**
 * ��    ������OLED�Դ����鲿������
 * ��    ����X ָ���������Ͻǵĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
 * ��    ����Y ָ���������Ͻǵ������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
 * ��    ����Width ָ������Ŀ�ȣ���Χ��0 ~ 128
 * ��    ����Height ָ������ĸ߶ȣ���Χ��0 ~ 64
 * �� �� ֵ����
 * ˵    �������ô˺�����OLED��������
 */
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
    int16_t i, j;

    for (j = Y; j < Y + Height; j++) // ����ָ��ҳ
    {
        for (i = X; i < X + Width; i++) // ����ָ����
        {
            if (i >= 0 && i <= 127 && j >= 0 && j <= 63) // ������Ļ�����ݲ���ʾ
            {
                OLED_DisplayBuf[j / 8][i] &=  ~ (0x01 << (j % 8)); // ���Դ�����ָ����������
            }
        }
    }
}

/**
 * ��    ������OLEDȫ������
 * ��    ������
 * �� �� ֵ����
 * ˵    �������ô˺�����OLEDȫ������
 */
void OLED_On(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++) // ����8ҳ
    {
        for (i = 0; i < 128; i++) // ����128��
        {
            OLED_DisplayBuf[j][i] = 0xff; // ���Դ���������ȫ������
        }
    }
    OLED_Update();
}

/**
 * ��    ��������OLED��ʾ
 * ��    ������
 * �� �� ֵ����
 * ˵    �������ô˺����󣬿���OLED��ʾ
 */
void OLED_Dislay_On(void)
{
    uint8_t cmd_bytes[3];
    cmd_bytes[0] = 0x8D;
    cmd_bytes[1] = 0x14;
    cmd_bytes[2] = 0xAF;
    OLED_WriteCmd(cmd_bytes, 3);
}

/**
 * ��    �����ر�OLED��ʾ
 * ��    ������
 * �� �� ֵ����
 * ˵    �������ô˺����󣬹ر�OLED��ʾ
 */
void OLED_Dislay_Off(void)
{
    uint8_t cmd_bytes[3];
    cmd_bytes[0] = 0x8D;
    cmd_bytes[1] = 0x10;
    cmd_bytes[2] = 0xAE;
    OLED_WriteCmd(cmd_bytes, 3);
}

/**
 * ��    ������OLED�Դ�����ȫ��ȡ��
 * ��    ������
 * �� �� ֵ����
 * ˵    �������ô˺�����OLED�Դ�����ȫ��ȡ��
 */
void OLED_Reverse(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++) // ����8ҳ
    {
        for (i = 0; i < 128; i++) // ����128��
        {
            OLED_DisplayBuf[j][i] ^= 0xFF; // ���Դ���������ȫ��ȡ��
        }
    }
}

/**
 * ��    ������OLED�Դ����鲿��ȡ��
 * ��    ����X ָ���������Ͻǵĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
 * ��    ����Y ָ���������Ͻǵ������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
 * ��    ����Width ָ������Ŀ�ȣ���Χ��0 ~ 128
 * ��    ����Height ָ������ĸ߶ȣ���Χ��0 ~ 64
 * �� �� ֵ����
 * ˵    �������ô˺����󣬽�OLED�Դ����鲿��ȡ��
 */
void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
    int16_t i, j;

    for (j = Y; j < Y + Height; j++) // ����ָ��ҳ
    {
        for (i = X; i < X + Width; i++) // ����ָ����
        {
            if (i >= 0 && i <= 127 && j >= 0 && j <= 63) // ������Ļ�����ݲ���ʾ
            {
                OLED_DisplayBuf[j / 8][i] ^= 0x01 << (j % 8); // ���Դ�����ָ������ȡ��
            }
        }
    }
}

/**
 * ��    ����ֹͣ����
 * ��    ������
 * �� �� ֵ����
 * ˵    �������ô˺�����ֹͣ����
 */
void OLED_Scroll_Stop(void)
{
    uint8_t cmd_bytes[1];
    // ֹͣ����
    cmd_bytes[0] = 0x2e;
    OLED_WriteCmd(cmd_bytes, 1);
}

/**
 * ��    ��������ȫ������ˮƽ����
 * ��    ����dir �����ķ��� ˮƽ�ҹ�����0x26 ˮƽ�������0x27
 * �� �� ֵ����
 * ˵    �������ô˺�����OLEDȫ������ˮƽ����
 */
void OLED_Horizontal_Scroll(uint8_t dir)
{
    uint8_t cmd_bytes[9];
    // ֹͣ����
    cmd_bytes[0] = 0x2e;
    // ���ù�������
    cmd_bytes[1] = dir;
    // ���������ֽ�
    cmd_bytes[2] = 0x00;
    // ���ÿ�ʼҳ��ַ
    cmd_bytes[3] = 0x00;
    // ����֡�ٶ�
    cmd_bytes[4] = 0x07;
    // ���ý���ҳ
    cmd_bytes[5] = 0x07;
    // �����ֽ�����
    cmd_bytes[6] = 0x00;
    cmd_bytes[7] = 0xff;
    // ��������
    cmd_bytes[8] = 0x2f;
    OLED_WriteCmd(cmd_bytes, 9);
}

/**
 * ��    �������ò�������ˮƽ����
 * ��    ����dir �����ķ��� 0x26 ˮƽ�������0x27
 * ��    ����start ��ʼҳ��ַ
 * ��    ����end ����ҳ��ַ
 * �� �� ֵ����
 * ˵    �������ô˺�����OLED��������ˮƽ����
 */
void OLED_Horizontal_Scroll_Area(uint8_t dir, uint8_t start, uint8_t end)
{
    uint8_t cmd_bytes[9];
    // ֹͣ����
    cmd_bytes[0] = 0x2e;
    // ���ù�������
    cmd_bytes[1] = dir;
    // ���������ֽ�
    cmd_bytes[2] = 0x00;
    // ���ÿ�ʼҳ��ַ
    cmd_bytes[3] = start;
    // ����֡�ٶ�
    cmd_bytes[4] = 0x07;
    // ���ý���ҳ
    cmd_bytes[5] = end;
    // �����ֽ�����
    cmd_bytes[6] = 0x00;
    cmd_bytes[7] = 0xff;
    // ��������
    cmd_bytes[8] = 0x2f;
    OLED_WriteCmd(cmd_bytes, 9);
}

/**
 * ��    ��������ˮƽ�봹ֱ��������
 * ��    ����dir �����ķ��� ��ֱ��ˮƽ�ҹ�����0x29 ��ֱ��ˮƽ�������0x2A��ֱ��ˮƽ�ҹ���
 * ��    ����start ��ʼҳ��ַ
 * ��    ����end ����ҳ��ַ
 * �� �� ֵ����
 * ˵    �������ô˺�����OLED��������ˮƽ����
 */
void OLED_Horizontal_And_Vertical_Scroll(uint8_t dir)
{
    uint8_t cmd_bytes[10];
    // ֹͣ����
    cmd_bytes[0] = 0x2e;
    // ���ù�������
    cmd_bytes[1] = dir;
    // ���������ֽ�
    cmd_bytes[2] = 0x01;
    // ���ÿ�ʼҳ��ַ
    cmd_bytes[3] = 0x00;
    // ����֡�ٶ�
    cmd_bytes[4] = 0x07;
    // ���ý���ҳ
    cmd_bytes[5] = 0x07;
    // ��ֱ����ƫ����
    cmd_bytes[6] = 0x01;
    // �����ֽ�����
    cmd_bytes[7] = 0x00;
    cmd_bytes[8] = 0xff;
    // ��������
    cmd_bytes[9] = 0x2f;
    OLED_WriteCmd(cmd_bytes, 10);
}

/**
 * ��    ����������ʾ����
 * ��    ����Bright 0x00 ~ 0xff ��ʾ����Ĭ������ֵͨ���� 0x7F
 * �� �� ֵ����
 * ˵    ������ʾ����Ĭ������ֵͨ���� 0x7F
 */
void OLED_Bright_Set(uint8_t Bright)
{
    uint8_t cmd_bytes[2];
    cmd_bytes[0] = 0x81;
    cmd_bytes[1] = Bright;
    OLED_WriteCmd(cmd_bytes, 2);
}

/**
 * ��    ����OLED��ʾͼ��
 * ��    ����X ָ��ͼ�����Ͻǵĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
 * ��    ����Y ָ��ͼ�����Ͻǵ������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
 * ��    ����Width ָ��ͼ��Ŀ�ȣ���Χ��0 ~ 128
 * ��    ����Height ָ��ͼ��ĸ߶ȣ���Χ��0 ~ 64
 * ��    ����Image ָ��Ҫ��ʾ��ͼ��
 * �� �� ֵ����
 * ˵    ����
 */
void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
    uint8_t i = 0, j = 0;
    int16_t Page, Shift;

    /*��ͼ�������������*/
    OLED_ClearArea(X, Y, Width, Height);

    /*����ָ��ͼ���漰�����ҳ*/
    /*(Height - 1) / 8 + 1��Ŀ����Height / 8������ȡ��*/
    for (j = 0; j < (Height - 1) / 8 + 1; j++) {
        /*����ָ��ͼ���漰�������*/
        for (i = 0; i < Width; i++) {
            if (X + i >= 0 && X + i <= 127) // ������Ļ�����ݲ���ʾ
            {
                /*���������ڼ���ҳ��ַ����λʱ��Ҫ��һ��ƫ��*/
                Page  = Y / 8;
                Shift = Y % 8;
                if (Y < 0) {
                    Page -= 1;
                    Shift += 8;
                }

                if (Page + j >= 0 && Page + j <= 7) // ������Ļ�����ݲ���ʾ
                {
                    /*��ʾͼ���ڵ�ǰҳ������*/
                    OLED_DisplayBuf[Page + j][X + i] |= Image[j * Width + i] << (Shift);
                }

                if (Page + j + 1 >= 0 && Page + j + 1 <= 7) // ������Ļ�����ݲ���ʾ
                {
                    /*��ʾͼ������һҳ������*/
                    OLED_DisplayBuf[Page + j + 1][X + i] |= Image[j * Width + i] >> (8 - Shift);
                }
            }
        }
    }
}

/**
 * ��    ����OLED��ʾһ���ַ�
 * ��    ����X ָ���ַ����Ͻǵĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
 * ��    ����Y ָ���ַ����Ͻǵ������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
 * ��    ����Char ָ��Ҫ��ʾ���ַ�����Χ��ASCII��ɼ��ַ�
 * ��    ����FontSize ָ�������С
 *           ��Χ��OLED_8X16		��8���أ���16����
 *                 OLED_6X8		��6���أ���8����
 * �� �� ֵ����
 * ˵    ����
 */
void OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize)
{
    if (FontSize == OLED_8X16) // ����Ϊ��8���أ���16����
    {
        /*��ASCII��ģ��OLED_F8x16��ָ��������8*16��ͼ���ʽ��ʾ*/
        OLED_ShowImage(X, Y, 8, 16, OLED_F8x16[Char - ' ']);
    } else if (FontSize == OLED_6X8) // ����Ϊ��6���أ���8����
    {
        /*��ASCII��ģ��OLED_F6x8��ָ��������6*8��ͼ���ʽ��ʾ*/
        OLED_ShowImage(X, Y, 6, 8, OLED_F6x8[Char - ' ']);
    }
}

void OLED_ShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize)
{
    uint16_t i = 0;
    char SingleChar[5];
    uint8_t CharLength = 0;
    uint16_t XOffset   = 0;
    uint16_t pIndex;

    while (String[i] != '\0') // �����ַ���
    {

#ifdef OLED_CHARSET_UTF8 // �����ַ���ΪUTF8
        /*�˶δ����Ŀ���ǣ���ȡUTF8�ַ����е�һ���ַ���ת�浽SingleChar���ַ�����*/
        /*�ж�UTF8�����һ���ֽڵı�־λ*/
        if ((String[i] & 0x80) == 0x00) // ��һ���ֽ�Ϊ0xxxxxxx
        {
            CharLength    = 1;                 // �ַ�Ϊ1�ֽ�
            SingleChar[0] = String[i++];       // ����һ���ֽ�д��SingleChar��0��λ�ã����iָ����һ���ֽ�
            SingleChar[1] = '\0';              // ΪSingleChar����ַ���������־λ
        } else if ((String[i] & 0xE0) == 0xC0) // ��һ���ֽ�Ϊ110xxxxx
        {
            CharLength    = 2;                 // �ַ�Ϊ2�ֽ�
            SingleChar[0] = String[i++];       // ����һ���ֽ�д��SingleChar��0��λ�ã����iָ����һ���ֽ�
            if (String[i] == '\0') { break; }  // �������������ѭ����������ʾ
            SingleChar[1] = String[i++];       // ���ڶ����ֽ�д��SingleChar��1��λ�ã����iָ����һ���ֽ�
            SingleChar[2] = '\0';              // ΪSingleChar����ַ���������־λ
        } else if ((String[i] & 0xF0) == 0xE0) // ��һ���ֽ�Ϊ1110xxxx
        {
            CharLength    = 3; // �ַ�Ϊ3�ֽ�
            SingleChar[0] = String[i++];
            if (String[i] == '\0') { break; }
            SingleChar[1] = String[i++];
            if (String[i] == '\0') { break; }
            SingleChar[2] = String[i++];
            SingleChar[3] = '\0';
        } else if ((String[i] & 0xF8) == 0xF0) // ��һ���ֽ�Ϊ11110xxx
        {
            CharLength    = 4; // �ַ�Ϊ4�ֽ�
            SingleChar[0] = String[i++];
            if (String[i] == '\0') { break; }
            SingleChar[1] = String[i++];
            if (String[i] == '\0') { break; }
            SingleChar[2] = String[i++];
            if (String[i] == '\0') { break; }
            SingleChar[3] = String[i++];
            SingleChar[4] = '\0';
        } else {
            i++; // ���������iָ����һ���ֽڣ����Դ��ֽڣ������ж���һ���ֽ�
            continue;
        }
#endif

#ifdef OLED_CHARSET_GB2312 // �����ַ���ΪGB2312
        /*�˶δ����Ŀ���ǣ���ȡGB2312�ַ����е�һ���ַ���ת�浽SingleChar���ַ�����*/
        /*�ж�GB2312�ֽڵ����λ��־λ*/
        if ((String[i] & 0x80) == 0x00) // ���λΪ0
        {
            CharLength    = 1;           // �ַ�Ϊ1�ֽ�
            SingleChar[0] = String[i++]; // ����һ���ֽ�д��SingleChar��0��λ�ã����iָ����һ���ֽ�
            SingleChar[1] = '\0';        // ΪSingleChar����ַ���������־λ
        } else                           // ���λΪ1
        {
            CharLength    = 2;                // �ַ�Ϊ2�ֽ�
            SingleChar[0] = String[i++];      // ����һ���ֽ�д��SingleChar��0��λ�ã����iָ����һ���ֽ�
            if (String[i] == '\0') { break; } // �������������ѭ����������ʾ
            SingleChar[1] = String[i++];      // ���ڶ����ֽ�д��SingleChar��1��λ�ã����iָ����һ���ֽ�
            SingleChar[2] = '\0';             // ΪSingleChar����ַ���������־λ
        }
#endif

        /*��ʾ����������ȡ����SingleChar*/
        if (CharLength == 1) // ����ǵ��ֽ��ַ�
        {
            /*ʹ��OLED_ShowChar��ʾ���ַ�*/
            OLED_ShowChar(X + XOffset, Y, SingleChar[0], FontSize);
            XOffset += FontSize;
        } else // ���򣬼����ֽ��ַ�
        {
            /*����������ģ�⣬����ģ����Ѱ�Ҵ��ַ�������*/
            /*����ҵ����һ���ַ�������Ϊ���ַ����������ʾ�ַ�δ����ģ�ⶨ�壬ֹͣѰ��*/
            for (pIndex = 0; strcmp(OLED_CF16x16[pIndex].Index, "") != 0; pIndex++) {
                /*�ҵ�ƥ����ַ�*/
                if (strcmp(OLED_CF16x16[pIndex].Index, SingleChar) == 0) {
                    break; // ����ѭ������ʱpIndex��ֵΪָ���ַ�������
                }
            }
            if (FontSize == OLED_8X16) // ��������Ϊ8*16����
            {
                /*����ģ��OLED_CF16x16��ָ��������16*16��ͼ���ʽ��ʾ*/
                OLED_ShowImage(X + XOffset, Y, 16, 16, OLED_CF16x16[pIndex].Data);
                XOffset += 16;
            } else if (FontSize == OLED_6X8) // ��������Ϊ6*8����
            {
                /*�ռ䲻�㣬��λ����ʾ'?'*/
                OLED_ShowChar(X + XOffset, Y, '?', OLED_6X8);
                XOffset += OLED_6X8;
            }
        }
    }
}

/**
  * ��    �����η�����
  * ��    ����X ����
  * ��    ����Y ָ��
  * �� �� ֵ������X��Y�η�
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;	//���Ĭ��Ϊ1
	while (Y --)			//�۳�Y��
	{
		Result *= X;		//ÿ�ΰ�X�۳˵������
	}
	return Result;
}

/**
 * ��    ����OLED��ʾ���֣�ʮ���ƣ���������
 * ��    ����X ָ���������Ͻǵĺ����꣬��Ļ����0 ~ 127
 * ��    ����Y ָ���������Ͻǵ������꣬��Ļ����0 ~ 63
 * ��    ����Number ָ��Ҫ��ʾ�����֣���Χ��0 ~ 4294967295
 * ��    ����Length ָ�����ֵĳ��ȣ���Χ��0 ~ 10
 * ��    ����FontSize ָ�������С
 *           ��Χ��OLED_8X16		��8���أ���16����
 *                 OLED_6X8		��6���أ���8����
 * �� �� ֵ����
 * ˵    ����
 */
void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
    uint8_t i;
    for (i = 0; i < Length; i++) // �������ֵ�ÿһλ
    {
        /*����OLED_ShowChar������������ʾÿ������*/
        /*Number / OLED_Pow(10, Length - i - 1) % 10 ����ʮ������ȡ���ֵ�ÿһλ*/
        /*+ '0' �ɽ�����ת��Ϊ�ַ���ʽ*/
        OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
    }
}

/**
  * ��    ����OLED��ʾ�з������֣�ʮ���ƣ�������
  * ��    ����X ָ���������Ͻǵĺ����꣬��Ļ����0 ~ 127
  * ��    ����Y ָ���������Ͻǵ������꣬��Ļ����0 ~ 63
  * ��    ����Number ָ��Ҫ��ʾ�����֣���Χ��-2147483648 ~ 2147483647
  * ��    ����Length ָ�����ֵĳ��ȣ���Χ��0 ~ 10
  * ��    ����FontSize ָ�������С
  *           ��Χ��OLED_8X16		��8���أ���16����
  *                 OLED_6X8		��6���أ���8����
  * �� �� ֵ����
  */

void OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize)
{
    	uint8_t i;
	uint32_t Number1;
	
	if (Number >= 0)						//���ִ��ڵ���0
	{
		OLED_ShowChar(X, Y, '+', FontSize);	//��ʾ+��
		Number1 = Number;					//Number1ֱ�ӵ���Number
	}
	else									//����С��0
	{
		OLED_ShowChar(X, Y, '-', FontSize);	//��ʾ-��
		Number1 = -Number;					//Number1����Numberȡ��
	}
	
	for (i = 0; i < Length; i++)			//�������ֵ�ÿһλ								
	{
		/*����OLED_ShowChar������������ʾÿ������*/
		/*Number1 / OLED_Pow(10, Length - i - 1) % 10 ����ʮ������ȡ���ֵ�ÿһλ*/
		/*+ '0' �ɽ�����ת��Ϊ�ַ���ʽ*/
		OLED_ShowChar(X + (i + 1) * FontSize, Y, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
	}
}

/**
  * ��    ����OLED��ʾʮ���������֣�ʮ�����ƣ���������
  * ��    ����X ָ���������Ͻǵĺ����꣬��Ļ����0 ~ 127
  * ��    ����Y ָ���������Ͻǵ������꣬��Ļ����0 ~ 63
  * ��    ����Number ָ��Ҫ��ʾ�����֣���Χ��0x00000000 ~ 0xFFFFFFFF
  * ��    ����Length ָ�����ֵĳ��ȣ���Χ��0 ~ 8
  * ��    ����FontSize ָ�������С
  *           ��Χ��OLED_8X16		��8���أ���16����
  *                 OLED_6X8		��6���أ���8����
  * �� �� ֵ����
  */
void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
    uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)		//�������ֵ�ÿһλ
	{
		/*��ʮ��������ȡ���ֵ�ÿһλ*/
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		
		if (SingleNumber < 10)			//��������С��10
		{
			/*����OLED_ShowChar��������ʾ������*/
			/*+ '0' �ɽ�����ת��Ϊ�ַ���ʽ*/
			OLED_ShowChar(X + i * FontSize, Y, SingleNumber + '0', FontSize);
		}
		else							//�������ִ���10
		{
			/*����OLED_ShowChar��������ʾ������*/
			/*+ 'A' �ɽ�����ת��Ϊ��A��ʼ��ʮ�������ַ�*/
			OLED_ShowChar(X + i * FontSize, Y, SingleNumber - 10 + 'A', FontSize);
		}
	}
}

/**
  * ��    ����OLED��ʾ���������֣������ƣ���������
  * ��    ����X ָ���������Ͻǵĺ����꣬��Ļ����0 ~ 127
  * ��    ����Y ָ���������Ͻǵ������꣬����Ļ����0 ~ 63
  * ��    ����Number ָ��Ҫ��ʾ�����֣���Χ��0x00000000 ~ 0xFFFFFFFF
  * ��    ����Length ָ�����ֵĳ��ȣ���Χ��0 ~ 16
  * ��    ����FontSize ָ�������С
  *           ��Χ��OLED_8X16		��8���أ���16����
  *                 OLED_6X8		��6���أ���8����
  * �� �� ֵ����
  */
void OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
    uint8_t i;
	for (i = 0; i < Length; i++)		//�������ֵ�ÿһλ	
	{
		/*����OLED_ShowChar������������ʾÿ������*/
		/*Number / OLED_Pow(2, Length - i - 1) % 2 ���Զ�������ȡ���ֵ�ÿһλ*/
		/*+ '0' �ɽ�����ת��Ϊ�ַ���ʽ*/
		OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(2, Length - i - 1) % 2 + '0', FontSize);
	}
}

/**
  * ��    ����OLED��ʾ�������֣�ʮ���ƣ�С����
  * ��    ����X ָ���������Ͻǵĺ����꣬����Ļ����0 ~ 127
  * ��    ����Y ָ���������Ͻǵ������꣬��Ļ����0 ~ 63
  * ��    ����Number ָ��Ҫ��ʾ�����֣���Χ��-4294967295.0 ~ 4294967295.0
  * ��    ����IntLength ָ�����ֵ�����λ���ȣ���Χ��0 ~ 10
  * ��    ����FraLength ָ�����ֵ�С��λ���ȣ���Χ��0 ~ 9��С����������������ʾ
  * ��    ����FontSize ָ�������С
  *           ��Χ��OLED_8X16		��8���أ���16����
  *                 OLED_6X8		��6���أ���8����
  * �� �� ֵ����
  */
void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize)
{
    uint32_t PowNum, IntNum, FraNum;
	
	if (Number >= 0)						//���ִ��ڵ���0
	{
		OLED_ShowChar(X, Y, '+', FontSize);	//��ʾ+��
	}
	else									//����С��0
	{
		OLED_ShowChar(X, Y, '-', FontSize);	//��ʾ-��
		Number = -Number;					//Numberȡ��
	}
	
	/*��ȡ�������ֺ�С������*/
	IntNum = Number;						//ֱ�Ӹ�ֵ�����ͱ�������ȡ����
	Number -= IntNum;						//��Number��������������ֹ֮��С���˵�����ʱ����������ɴ���
	PowNum = OLED_Pow(10, FraLength);		//����ָ��С����λ����ȷ������
	FraNum = round(Number * PowNum);		//��С���˵�������ͬʱ�������룬������ʾ���
	IntNum += FraNum / PowNum;				//��������������˽�λ������Ҫ�ټӸ�����
	
	/*��ʾ��������*/
	OLED_ShowNum(X + FontSize, Y, IntNum, IntLength, FontSize);
	
	/*��ʾС����*/
	OLED_ShowChar(X + (IntLength + 1) * FontSize, Y, '.', FontSize);
	
	/*��ʾС������*/
	OLED_ShowNum(X + (IntLength + 2) * FontSize, Y, FraNum, FraLength, FontSize);

}

/**
  * ��    ����OLEDʹ��printf������ӡ��ʽ���ַ�����֧��ASCII������Ļ��д�룩
  * ��    ����X ָ����ʽ���ַ������Ͻǵĺ����꣬��Ļ����0 ~ 127
  * ��    ����Y ָ����ʽ���ַ������Ͻǵ������꣬��Ļ����0 ~ 63
  * ��    ����FontSize ָ�������С
  *           ��Χ��OLED_8X16		��8���أ���16����
  *                 OLED_6X8		��6���أ���8����
  * ��    ����format ָ��Ҫ��ʾ�ĸ�ʽ���ַ�������Χ��ASCII��ɼ��ַ��������ַ���ɵ��ַ���
  * ��    ����... ��ʽ���ַ��������б�
  * �� �� ֵ����
  * ˵    ������ʾ�������ַ���Ҫ��OLED_Data.c���OLED_CF16x16���鶨��
  *           δ�ҵ�ָ�������ַ�ʱ������ʾĬ��ͼ�Σ�һ�������ڲ�һ���ʺţ�
  *           �������СΪOLED_8X16ʱ�������ַ���16*16����������ʾ
  *           �������СΪOLED_6X8ʱ�������ַ���6*8������ʾ'?'
  */
void OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...)
{
    char String[256];						//�����ַ�����
	va_list arg;							//����ɱ�����б��������͵ı���arg
	va_start(arg, format);					//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);			//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);							//��������arg
	OLED_ShowString(X, Y, String, FontSize);//OLED��ʾ�ַ����飨�ַ�����
}


#ifdef OLED_Extend
/**
  * ��    �����ж�ָ�����Ƿ���ָ��������ڲ�
  * ��    ����nvert ����εĶ�����
  * ��    ����vertx verty ��������ζ����x��y���������
  * ��    ����testx testy ���Ե��X��y����
  * �� �� ֵ��ָ�����Ƿ���ָ��������ڲ���1�����ڲ���0�������ڲ�
  */
uint8_t OLED_pnpoly(uint8_t nvert, int16_t *vertx, int16_t *verty, int16_t testx, int16_t testy)
{
	int16_t i, j, c = 0;
	for (i = 0, j = nvert - 1; i < nvert; j = i++)
	{
		if (((verty[i] > testy) != (verty[j] > testy)) &&
			(testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
		{
			c = !c;
		}
	}
	return c;
}

/**
  * ��    �����ж�ָ�����Ƿ���ָ���Ƕ��ڲ�
  * ��    ����X Y ָ���������
  * ��    ����StartAngle EndAngle ��ʼ�ǶȺ���ֹ�Ƕȣ���Χ��-180~180
  *           ˮƽ����Ϊ0�ȣ�ˮƽ����Ϊ180�Ȼ�-180�ȣ��·�Ϊ�������Ϸ�Ϊ������˳ʱ����ת
  * �� �� ֵ��ָ�����Ƿ���ָ���Ƕ��ڲ���1�����ڲ���0�������ڲ�
  */
uint8_t OLED_IsInAngle(int16_t X, int16_t Y, int16_t StartAngle, int16_t EndAngle)
{
	int16_t PointAngle;
	PointAngle = atan2(Y, X) / 3.14 * 180;	//����ָ����Ļ��ȣ���ת��Ϊ�Ƕȱ�ʾ
	if (StartAngle < EndAngle)	//��ʼ�Ƕ�С����ֹ�Ƕȵ����
	{
		/*���ָ���Ƕ�����ʼ��ֹ�Ƕ�֮�䣬���ж�ָ������ָ���Ƕ�*/
		if (PointAngle >= StartAngle && PointAngle <= EndAngle)
		{
			return 1;
		}
	}
	else			//��ʼ�Ƕȴ�������ֹ�Ƕȵ����
	{
		/*���ָ���Ƕȴ�����ʼ�ǶȻ���С����ֹ�Ƕȣ����ж�ָ������ָ���Ƕ�*/
		if (PointAngle >= StartAngle || PointAngle <= EndAngle)
		{
			return 1;
		}
	}
	return 0;		//�������������������ж��ж�ָ���㲻��ָ���Ƕ�
}
/**
  * ��    ����OLED��ָ��λ�û�һ����
  * ��    ����X ָ����ĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
  * ��    ����Y ָ����������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
  * �� �� ֵ����
  */
void OLED_DrawPoint(int16_t X, int16_t Y)
{
	if (X >= 0 && X <= 127 && Y >=0 && Y <= 63)		//������Ļ�����ݲ���ʾ
	{
		/*���Դ�����ָ��λ�õ�һ��Bit������1*/
		OLED_DisplayBuf[Y / 8][X] |= 0x01 << (Y % 8);
	}
}

/**
  * ��    ����OLED��ȡָ��λ�õ��ֵ
  * ��    ����X ָ����ĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
  * ��    ����Y ָ����������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
  * �� �� ֵ��ָ��λ�õ��Ƿ��ڵ���״̬��1��������0��Ϩ��
  */
uint8_t OLED_GetPoint(int16_t X, int16_t Y)
{
	if (X >= 0 && X <= 127 && Y >=0 && Y <= 63)		//������Ļ�����ݲ���ȡ
	{
		/*�ж�ָ��λ�õ�����*/
		if (OLED_DisplayBuf[Y / 8][X] & 0x01 << (Y % 8))
		{
			return 1;	//Ϊ1������1
		}
	}
	
	return 0;		//���򣬷���0
}

/**
  * ��    ����OLED����
  * ��    ����X0 ָ��һ���˵�ĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
  * ��    ����Y0 ָ��һ���˵�������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
  * ��    ����X1 ָ����һ���˵�ĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
  * ��    ����Y1 ָ����һ���˵�������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
  * �� �� ֵ����
  */
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1)
{
	int16_t x, y, dx, dy, d, incrE, incrNE, temp;
	int16_t x0 = X0, y0 = Y0, x1 = X1, y1 = Y1;
	uint8_t yflag = 0, xyflag = 0;
	
	if (y0 == y1)		//���ߵ�������
	{
		/*0�ŵ�X�������1�ŵ�X���꣬�򽻻�����X����*/
		if (x0 > x1) {temp = x0; x0 = x1; x1 = temp;}
		
		/*����X����*/
		for (x = x0; x <= x1; x ++)
		{
			OLED_DrawPoint(x, y0);	//���λ���
		}
	}
	else if (x0 == x1)	//���ߵ�������
	{
		/*0�ŵ�Y�������1�ŵ�Y���꣬�򽻻�����Y����*/
		if (y0 > y1) {temp = y0; y0 = y1; y1 = temp;}
		
		/*����Y����*/
		for (y = y0; y <= y1; y ++)
		{
			OLED_DrawPoint(x0, y);	//���λ���
		}
	}
	else				//б��
	{
		
		if (x0 > x1)	//0�ŵ�X�������1�ŵ�X����
		{
			/*������������*/
			/*������Ӱ�컭�ߣ����ǻ��߷����ɵ�һ���������������ޱ�Ϊ��һ��������*/
			temp = x0; x0 = x1; x1 = temp;
			temp = y0; y0 = y1; y1 = temp;
		}
		
		if (y0 > y1)	//0�ŵ�Y�������1�ŵ�Y����
		{
			/*��Y����ȡ��*/
			/*ȡ����Ӱ�컭�ߣ����ǻ��߷����ɵ�һ�������ޱ�Ϊ��һ����*/
			y0 = -y0;
			y1 = -y1;
			
			/*�ñ�־λyflag����ס��ǰ�任���ں���ʵ�ʻ���ʱ���ٽ����껻����*/
			yflag = 1;
		}
		
		if (y1 - y0 > x1 - x0)	//����б�ʴ���1
		{
			/*��X������Y���껥��*/
			/*������Ӱ�컭�ߣ����ǻ��߷����ɵ�һ����0 ~ 90�ȷ�Χ��Ϊ��һ����0 ~ 45�ȷ�Χ*/
			temp = x0; x0 = y0; y0 = temp;
			temp = x1; x1 = y1; y1 = temp;
			
			/*�ñ�־λxyflag����ס��ǰ�任���ں���ʵ�ʻ���ʱ���ٽ����껻����*/
			xyflag = 1;
		}
		
		/*����ΪBresenham�㷨��ֱ��*/
		/*�㷨Ҫ�󣬻��߷������Ϊ��һ����0 ~ 45�ȷ�Χ*/
		dx = x1 - x0;
		dy = y1 - y0;
		incrE = 2 * dy;
		incrNE = 2 * (dy - dx);
		d = 2 * dy - dx;
		x = x0;
		y = y0;
		
		/*����ʼ�㣬ͬʱ�жϱ�־λ�������껻����*/
		if (yflag && xyflag){OLED_DrawPoint(y, -x);}
		else if (yflag)		{OLED_DrawPoint(x, -y);}
		else if (xyflag)	{OLED_DrawPoint(y, x);}
		else				{OLED_DrawPoint(x, y);}
		
		while (x < x1)		//����X���ÿ����
		{
			x ++;
			if (d < 0)		//��һ�����ڵ�ǰ�㶫��
			{
				d += incrE;
			}
			else			//��һ�����ڵ�ǰ�㶫����
			{
				y ++;
				d += incrNE;
			}
			
			/*��ÿһ���㣬ͬʱ�жϱ�־λ�������껻����*/
			if (yflag && xyflag){OLED_DrawPoint(y, -x);}
			else if (yflag)		{OLED_DrawPoint(x, -y);}
			else if (xyflag)	{OLED_DrawPoint(y, x);}
			else				{OLED_DrawPoint(x, y);}
		}	
	}
}

/**
  * ��    ����OLED����
  * ��    ����X ָ���������Ͻǵĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
  * ��    ����Y ָ���������Ͻǵ������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
  * ��    ����Width ָ�����εĿ�ȣ���Χ��0 ~ 128
  * ��    ����Height ָ�����εĸ߶ȣ���Χ��0 ~ 64
  * ��    ����IsFilled ָ�������Ƿ����
  *           ��Χ��OLED_UNFILLED		�����
  *                 OLED_FILLED			���
  * �� �� ֵ����
  */
void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled)
{
	int16_t i, j;
	if (!IsFilled)		//ָ�����β����
	{
		/*��������X���꣬����������������*/
		for (i = X; i < X + Width; i ++)
		{
			OLED_DrawPoint(i, Y);
			OLED_DrawPoint(i, Y + Height - 1);
		}
		/*��������Y���꣬����������������*/
		for (i = Y; i < Y + Height; i ++)
		{
			OLED_DrawPoint(X, i);
			OLED_DrawPoint(X + Width - 1, i);
		}
	}
	else				//ָ���������
	{
		/*����X����*/
		for (i = X; i < X + Width; i ++)
		{
			/*����Y����*/
			for (j = Y; j < Y + Height; j ++)
			{
				/*��ָ�����򻭵㣬���������*/
				OLED_DrawPoint(i, j);
			}
		}
	}
}

/**
  * ��    ����OLED������
  * ��    ����X0 ָ����һ���˵�ĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
  * ��    ����Y0 ָ����һ���˵�������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
  * ��    ����X1 ָ���ڶ����˵�ĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
  * ��    ����Y1 ָ���ڶ����˵�������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
  * ��    ����X2 ָ���������˵�ĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
  * ��    ����Y2 ָ���������˵�������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
  * ��    ����IsFilled ָ���������Ƿ����
  *           ��Χ��OLED_UNFILLED		�����
  *                 OLED_FILLED			���
  * �� �� ֵ����
  */
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled)
{
	int16_t minx = X0, miny = Y0, maxx = X0, maxy = Y0;
	int16_t i, j;
	int16_t vx[] = {X0, X1, X2};
	int16_t vy[] = {Y0, Y1, Y2};
	
	if (!IsFilled)			//ָ�������β����
	{
		/*���û��ߺ���������������ֱ������*/
		OLED_DrawLine(X0, Y0, X1, Y1);
		OLED_DrawLine(X0, Y0, X2, Y2);
		OLED_DrawLine(X1, Y1, X2, Y2);
	}
	else					//ָ�����������
	{
		/*�ҵ���������С��X��Y����*/
		if (X1 < minx) {minx = X1;}
		if (X2 < minx) {minx = X2;}
		if (Y1 < miny) {miny = Y1;}
		if (Y2 < miny) {miny = Y2;}
		
		/*�ҵ�����������X��Y����*/
		if (X1 > maxx) {maxx = X1;}
		if (X2 > maxx) {maxx = X2;}
		if (Y1 > maxy) {maxy = Y1;}
		if (Y2 > maxy) {maxy = Y2;}
		
		/*��С�������֮��ľ���Ϊ������Ҫ��������*/
		/*���������������еĵ�*/
		/*����X����*/		
		for (i = minx; i <= maxx; i ++)
		{
			/*����Y����*/	
			for (j = miny; j <= maxy; j ++)
			{
				/*����OLED_pnpoly���ж�ָ�����Ƿ���ָ��������֮��*/
				/*����ڣ��򻭵㣬������ڣ���������*/
				if (OLED_pnpoly(3, vx, vy, i, j)) {OLED_DrawPoint(i, j);}
			}
		}
	}
}

/**
  * ��    ����OLED��Բ
  * ��    ����X ָ��Բ��Բ�ĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
  * ��    ����Y ָ��Բ��Բ�������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
  * ��    ����Radius ָ��Բ�İ뾶����Χ��0 ~ 255
  * ��    ����IsFilled ָ��Բ�Ƿ����
  *           ��Χ��OLED_UNFILLED		�����
  *                 OLED_FILLED			���
  * �� �� ֵ����
  */
void OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled)
{
	int16_t x, y, d, j;
	
	
	d = 1 - Radius;
	x = 0;
	y = Radius;
	
	/*��ÿ���˷�֮һԲ������ʼ��*/
	OLED_DrawPoint(X + x, Y + y);
	OLED_DrawPoint(X - x, Y - y);
	OLED_DrawPoint(X + y, Y + x);
	OLED_DrawPoint(X - y, Y - x);
	
	if (IsFilled)		//ָ��Բ���
	{
		/*������ʼ��Y����*/
		for (j = -y; j < y; j ++)
		{
			/*��ָ�����򻭵㣬��䲿��Բ*/
			OLED_DrawPoint(X, Y + j);
		}
	}
	
	while (x < y)		//����X���ÿ����
	{
		x ++;
		if (d < 0)		//��һ�����ڵ�ǰ�㶫��
		{
			d += 2 * x + 1;
		}
		else			//��һ�����ڵ�ǰ�㶫�Ϸ�
		{
			y --;
			d += 2 * (x - y) + 1;
		}
		
		/*��ÿ���˷�֮һԲ���ĵ�*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X + y, Y + x);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - y, Y - x);
		OLED_DrawPoint(X + x, Y - y);
		OLED_DrawPoint(X + y, Y - x);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X - y, Y + x);
		
		if (IsFilled)	//ָ��Բ���
		{
			/*�����м䲿��*/
			for (j = -y; j < y; j ++)
			{
				/*��ָ�����򻭵㣬��䲿��Բ*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
			
			/*�������ಿ��*/
			for (j = -x; j < x; j ++)
			{
				/*��ָ�����򻭵㣬��䲿��Բ*/
				OLED_DrawPoint(X - y, Y + j);
				OLED_DrawPoint(X + y, Y + j);
			}
		}
	}
}

/**
  * ��    ����OLED����Բ
  * ��    ����X ָ����Բ��Բ�ĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
  * ��    ����Y ָ����Բ��Բ�������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
  * ��    ����A ָ����Բ�ĺ�����᳤�ȣ���Χ��0 ~ 255
  * ��    ����B ָ����Բ��������᳤�ȣ���Χ��0 ~ 255
  * ��    ����IsFilled ָ����Բ�Ƿ����
  *           ��Χ��OLED_UNFILLED		�����
  *                 OLED_FILLED			���
  * �� �� ֵ����
  */
void OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled)
{
	int16_t x, y, j;
	int16_t a = A, b = B;
	float d1, d2;
	
	
	x = 0;
	y = b;
	d1 = b * b + a * a * (-b + 0.5);
	
	if (IsFilled)	//ָ����Բ���
	{
		/*������ʼ��Y����*/
		for (j = -y; j < y; j ++)
		{
			/*��ָ�����򻭵㣬��䲿����Բ*/
			OLED_DrawPoint(X, Y + j);
			OLED_DrawPoint(X, Y + j);
		}
	}
	
	/*����Բ������ʼ��*/
	OLED_DrawPoint(X + x, Y + y);
	OLED_DrawPoint(X - x, Y - y);
	OLED_DrawPoint(X - x, Y + y);
	OLED_DrawPoint(X + x, Y - y);
	
	/*����Բ�м䲿��*/
	while (b * b * (x + 1) < a * a * (y - 0.5))
	{
		if (d1 <= 0)		//��һ�����ڵ�ǰ�㶫��
		{
			d1 += b * b * (2 * x + 3);
		}
		else				//��һ�����ڵ�ǰ�㶫�Ϸ�
		{
			d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
			y --;
		}
		x ++;
		
		if (IsFilled)	//ָ����Բ���
		{
			/*�����м䲿��*/
			for (j = -y; j < y; j ++)
			{
				/*��ָ�����򻭵㣬��䲿����Բ*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
		}
		
		/*����Բ�м䲿��Բ��*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X + x, Y - y);
	}
	
	/*����Բ���ಿ��*/
	d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;
	
	while (y > 0)
	{
		if (d2 <= 0)		//��һ�����ڵ�ǰ�㶫��
		{
			d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
			x ++;
			
		}
		else				//��һ�����ڵ�ǰ�㶫�Ϸ�
		{
			d2 += a * a * (-2 * y + 3);
		}
		y --;
		
		if (IsFilled)	//ָ����Բ���
		{
			/*�������ಿ��*/
			for (j = -y; j < y; j ++)
			{
				/*��ָ�����򻭵㣬��䲿����Բ*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
		}
		
		/*����Բ���ಿ��Բ��*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X + x, Y - y);
	}
}

/**
  * ��    ����OLED��Բ��
  * ��    ����X ָ��Բ����Բ�ĺ����꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 127
  * ��    ����Y ָ��Բ����Բ�������꣬��Χ��-32768 ~ 32767����Ļ����0 ~ 63
  * ��    ����Radius ָ��Բ���İ뾶����Χ��0 ~ 255
  * ��    ����StartAngle ָ��Բ������ʼ�Ƕȣ���Χ��-180 ~ 180
  *           ˮƽ����Ϊ0�ȣ�ˮƽ����Ϊ180�Ȼ�-180�ȣ��·�Ϊ�������Ϸ�Ϊ������˳ʱ����ת
  * ��    ����EndAngle ָ��Բ������ֹ�Ƕȣ���Χ��-180 ~ 180
  *           ˮƽ����Ϊ0�ȣ�ˮƽ����Ϊ180�Ȼ�-180�ȣ��·�Ϊ�������Ϸ�Ϊ������˳ʱ����ת
  * ��    ����IsFilled ָ��Բ���Ƿ���䣬����Ϊ����
  *           ��Χ��OLED_UNFILLED		�����
  *                 OLED_FILLED			���
  * �� �� ֵ����
  */
void OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled)
{
	int16_t x, y, d, j;
	
	/*�˺�������Bresenham�㷨��Բ�ķ���*/
	
	d = 1 - Radius;
	x = 0;
	y = Radius;
	
	/*�ڻ�Բ��ÿ����ʱ���ж�ָ�����Ƿ���ָ���Ƕ��ڣ��ڣ��򻭵㣬���ڣ���������*/
	if (OLED_IsInAngle(x, y, StartAngle, EndAngle))	{OLED_DrawPoint(X + x, Y + y);}
	if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y - y);}
	if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + x);}
	if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y - x);}
	
	if (IsFilled)	//ָ��Բ�����
	{
		/*������ʼ��Y����*/
		for (j = -y; j < y; j ++)
		{
			/*�����Բ��ÿ����ʱ���ж�ָ�����Ƿ���ָ���Ƕ��ڣ��ڣ��򻭵㣬���ڣ���������*/
			if (OLED_IsInAngle(0, j, StartAngle, EndAngle)) {OLED_DrawPoint(X, Y + j);}
		}
	}
	
	while (x < y)		//����X���ÿ����
	{
		x ++;
		if (d < 0)		//��һ�����ڵ�ǰ�㶫��
		{
			d += 2 * x + 1;
		}
		else			//��һ�����ڵ�ǰ�㶫�Ϸ�
		{
			y --;
			d += 2 * (x - y) + 1;
		}
		
		/*�ڻ�Բ��ÿ����ʱ���ж�ָ�����Ƿ���ָ���Ƕ��ڣ��ڣ��򻭵㣬���ڣ���������*/
		if (OLED_IsInAngle(x, y, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y + y);}
		if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + x);}
		if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y - y);}
		if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y - x);}
		if (OLED_IsInAngle(x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y - y);}
		if (OLED_IsInAngle(y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y - x);}
		if (OLED_IsInAngle(-x, y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y + y);}
		if (OLED_IsInAngle(-y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y + x);}
		
		if (IsFilled)	//ָ��Բ�����
		{
			/*�����м䲿��*/
			for (j = -y; j < y; j ++)
			{
				/*�����Բ��ÿ����ʱ���ж�ָ�����Ƿ���ָ���Ƕ��ڣ��ڣ��򻭵㣬���ڣ���������*/
				if (OLED_IsInAngle(x, j, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y + j);}
				if (OLED_IsInAngle(-x, j, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y + j);}
			}
			
			/*�������ಿ��*/
			for (j = -x; j < x; j ++)
			{
				/*�����Բ��ÿ����ʱ���ж�ָ�����Ƿ���ָ���Ƕ��ڣ��ڣ��򻭵㣬���ڣ���������*/
				if (OLED_IsInAngle(-y, j, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y + j);}
				if (OLED_IsInAngle(y, j, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + j);}
			}
		}
	}
}

#endif

