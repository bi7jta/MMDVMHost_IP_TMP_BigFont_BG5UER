/*
 *   Copyright (C) 2016,2017 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "OLED.h"

#include "NetworkInfo.h"

#include "Log.h"


//Logo MMDVM for Idle Screen
static unsigned char logo_glcd_bmp[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0xF8, 0x03, 0xFC, 0x7F, 0x80, 0x3F, 0xC7, 0xFF, 0xFC, 0xF8, 0x00, 0xF9, 0xFC, 0x01, 0xFE,
    0x01, 0xFC, 0x07, 0xFC, 0x7F, 0xC0, 0x7F, 0xC4, 0x00, 0x02, 0x48, 0x00, 0x91, 0xFE, 0x03, 0xFE,
    0x03, 0xFC, 0x07, 0xFC, 0x7F, 0xC0, 0x7F, 0xC5, 0xFF, 0xF1, 0x24, 0x01, 0x23, 0xFE, 0x03, 0xFE,
    0x03, 0xFE, 0x0F, 0xBC, 0x7B, 0xE0, 0xFB, 0xC5, 0x00, 0x09, 0x24, 0x01, 0x23, 0xDF, 0x07, 0xDE,
    0x07, 0xDE, 0x0F, 0x3C, 0x79, 0xE0, 0xF3, 0xC5, 0x00, 0x05, 0x12, 0x02, 0x47, 0xCF, 0x07, 0x9E,
    0x07, 0x9F, 0x1F, 0x3C, 0x79, 0xF1, 0xF3, 0xC5, 0x00, 0x05, 0x12, 0x02, 0x47, 0x8F, 0x8F, 0x9E,
    0x0F, 0x8F, 0x1E, 0x3C, 0x78, 0xF1, 0xE3, 0xC5, 0x00, 0x05, 0x09, 0x04, 0x8F, 0x87, 0x8F, 0x1E,
    0x0F, 0x0F, 0xBE, 0x3C, 0x78, 0xFB, 0xE3, 0xC5, 0x00, 0x05, 0x09, 0x04, 0x8F, 0x07, 0xDF, 0x1E,
    0x1F, 0x07, 0xFC, 0x3C, 0x78, 0x7F, 0xC3, 0xC5, 0x00, 0x05, 0x04, 0x89, 0x1F, 0x03, 0xFE, 0x1E,
    0x1E, 0x03, 0xFC, 0x3C, 0x78, 0x7F, 0xC3, 0xC5, 0x00, 0x09, 0x04, 0x89, 0x1E, 0x01, 0xFE, 0x1E,
    0x3E, 0x03, 0xF8, 0x3C, 0x78, 0x3F, 0x83, 0xC5, 0xFF, 0xF1, 0x02, 0x72, 0x3E, 0x01, 0xFC, 0x1E,
    0x3C, 0x01, 0xF0, 0x3C, 0x78, 0x1F, 0x03, 0xC4, 0x00, 0x02, 0x02, 0x02, 0x3C, 0x00, 0xF8, 0x1E,
    0x7C, 0x01, 0xF0, 0x3C, 0x78, 0x1F, 0x03, 0xC7, 0xFF, 0xFC, 0x01, 0xFC, 0x7C, 0x00, 0xF8, 0x1E,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//Logo D-Star 128x16 px
static unsigned char logo_dstar_bmp[] =
{
    0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x60, 0x03, 0xFF, 0xC0, 0x00, 0x00, 0x1F, 0xF0, 0xFF, 0xFE, 0x07, 0x80, 0x3F, 0xF8,
    0x00, 0x00, 0xC0, 0x07, 0xC1, 0xE0, 0x00, 0x00, 0x78, 0x7C, 0xFF, 0xFE, 0x0F, 0xC0, 0x3F, 0xFC,
    0x00, 0x01, 0xC0, 0x07, 0x80, 0xF0, 0x00, 0x00, 0xE0, 0x3C, 0x07, 0x80, 0x0F, 0xC0, 0x78, 0x0E,
    0x00, 0x03, 0xC0, 0x07, 0x80, 0x70, 0x00, 0x00, 0xE0, 0x38, 0x07, 0x00, 0x1B, 0xC0, 0x78, 0x0E,
    0x00, 0x07, 0xC0, 0x07, 0x80, 0x70, 0x00, 0x01, 0xE0, 0x00, 0x07, 0x00, 0x33, 0xC0, 0x70, 0x1E,
    0x07, 0xFF, 0xFE, 0x07, 0x00, 0x70, 0x00, 0x01, 0xF8, 0x00, 0x07, 0x00, 0x63, 0xC0, 0x70, 0x3C,
    0x01, 0xFF, 0xF8, 0x0F, 0x00, 0x71, 0xFF, 0xE0, 0xFF, 0xF0, 0x0E, 0x00, 0xE1, 0xE0, 0xFF, 0xE0,
    0x00, 0x7F, 0xE0, 0x0F, 0x00, 0x60, 0x00, 0x00, 0x03, 0xF8, 0x0E, 0x00, 0xC1, 0xE0, 0xFF, 0xE0,
    0x00, 0x3F, 0x80, 0x0E, 0x00, 0xE0, 0x00, 0x00, 0x00, 0xF0, 0x0E, 0x01, 0xFF, 0xE0, 0xE0, 0x70,
    0x00, 0x7F, 0x00, 0x1E, 0x00, 0xE0, 0x00, 0x03, 0x80, 0x70, 0x0C, 0x03, 0xFC, 0xE0, 0xE0, 0x30,
    0x00, 0xFF, 0x00, 0x1E, 0x01, 0xC0, 0x00, 0x07, 0x80, 0xE0, 0x1C, 0x07, 0x00, 0xE1, 0xE0, 0x38,
    0x01, 0xEF, 0x00, 0x1C, 0x07, 0x80, 0x00, 0x07, 0xC1, 0xE0, 0x1C, 0x06, 0x00, 0xF1, 0xC0, 0x38,
    0x03, 0x87, 0x00, 0x3F, 0xFF, 0x00, 0x00, 0x03, 0xFF, 0x80, 0x1C, 0x0C, 0x00, 0xF3, 0xC0, 0x38,
    0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


//Logo Fusion 128x16
const unsigned char logo_fusion_bmp [] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0xFC, 0x00, 0x1F, 0xE1, 0xFE, 0x1F, 0xFF, 0xF8, 0x7F, 0xC3, 0xFF, 0xFF, 0x1F, 0xFF, 0xFE,
    0x03, 0xFC, 0x00, 0x3F, 0xC3, 0xFC, 0x3F, 0x80, 0x00, 0x7F, 0x87, 0xF0, 0xFF, 0x0F, 0xF1, 0xFF,
    0x07, 0xFF, 0xFC, 0x7F, 0x83, 0xF8, 0x7F, 0x80, 0x00, 0xFF, 0x0F, 0xF0, 0xFF, 0x1F, 0xE1, 0xFE,
    0x0F, 0xFF, 0xF0, 0x7F, 0x07, 0xF0, 0xFF, 0xFF, 0xC1, 0xFF, 0x1F, 0xE1, 0xFE, 0x3F, 0xC3, 0xFC,
    0x0F, 0xF0, 0x00, 0xFE, 0x0F, 0xE0, 0x7F, 0xFF, 0xE1, 0xFE, 0x3F, 0xC3, 0xFC, 0x3F, 0xC3, 0xFC,
    0x1F, 0xE0, 0x01, 0xFC, 0x1F, 0xE0, 0x1F, 0xFF, 0xE3, 0xFC, 0x3F, 0xC3, 0xF8, 0x7F, 0x87, 0xF8,
    0x3F, 0xC0, 0x03, 0xFC, 0x3F, 0xC0, 0x00, 0x3F, 0xC7, 0xF8, 0x7F, 0x87, 0xF8, 0xFF, 0x0F, 0xF0,
    0x7F, 0xC0, 0x03, 0xFF, 0xFF, 0xE0, 0x00, 0x7F, 0x07, 0xF8, 0x7F, 0xCF, 0xE1, 0xFF, 0x1F, 0xF8,
    0x7F, 0x80, 0x01, 0xFF, 0xFF, 0xC7, 0xFF, 0xFC, 0x0F, 0xF0, 0x3F, 0xFF, 0x81, 0xFE, 0x1F, 0xF0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


//Logo DMR 128x16 pxstatic

unsigned char logo_dmr_bmp[] =
{
    0x00, 0x01, 0xFF, 0xFF, 0xF8, 0x01, 0xF8, 0x00, 0x00, 0x1F, 0x1F, 0xFF, 0xFF, 0xFC, 0x00, 0x00,
    0x00, 0x01, 0xFF, 0xFF, 0xFF, 0x81, 0xFC, 0x00, 0x00, 0x3F, 0x1F, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xE1, 0xFE, 0x00, 0x00, 0xFF, 0x1F, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x01, 0xF8, 0x00, 0x0F, 0xF1, 0xFF, 0x80, 0x01, 0xFF, 0x1F, 0x80, 0x00, 0x1F, 0x00, 0x00,
    0x00, 0x01, 0xF8, 0x00, 0x03, 0xF9, 0xFF, 0xC0, 0x03, 0xFF, 0x1F, 0x80, 0x00, 0x0F, 0x00, 0x00,
    0x00, 0x01, 0xF8, 0x00, 0x01, 0xF9, 0xFF, 0xE0, 0x07, 0xFF, 0x1F, 0x80, 0x00, 0x0F, 0x00, 0x00,
    0x00, 0x01, 0xF8, 0x00, 0x01, 0xFD, 0xF3, 0xF0, 0x1F, 0x9F, 0x1F, 0x80, 0x00, 0x1F, 0x00, 0x00,
    0x00, 0x01, 0xF8, 0x00, 0x00, 0xFD, 0xF1, 0xFC, 0x3F, 0x1F, 0x1F, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x01, 0xF8, 0x00, 0x00, 0xFD, 0xF0, 0xFE, 0x7E, 0x1F, 0x1F, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x01, 0xF8, 0x00, 0x01, 0xFD, 0xF0, 0x7F, 0xFC, 0x1F, 0x1F, 0xFF, 0xFF, 0xFC, 0x00, 0x00,
    0x00, 0x01, 0xF8, 0x00, 0x01, 0xF9, 0xF0, 0x1F, 0xF0, 0x1F, 0x1F, 0x81, 0xFC, 0x00, 0x00, 0x00,
    0x00, 0x01, 0xF8, 0x00, 0x07, 0xF9, 0xF0, 0x0F, 0xE0, 0x1F, 0x1F, 0x80, 0x7F, 0x00, 0x00, 0x00,
    0x00, 0x01, 0xF8, 0x00, 0x3F, 0xF1, 0xF0, 0x07, 0xC0, 0x1F, 0x1F, 0x80, 0x3F, 0xC0, 0x00, 0x00,
    0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xC1, 0xF0, 0x03, 0x80, 0x1F, 0x1F, 0x80, 0x0F, 0xF0, 0x00, 0x00,
    0x00, 0x01, 0xFF, 0xFF, 0xFF, 0x01, 0xF0, 0x00, 0x00, 0x1F, 0x1F, 0x80, 0x03, 0xFC, 0x00, 0x00,
    0x00, 0x01, 0xFF, 0xFF, 0xF0, 0x01, 0xF0, 0x00, 0x00, 0x1F, 0x1F, 0x80, 0x01, 0xFF, 0x00, 0x00
};

COLED::COLED(unsigned char displayType, unsigned char displayBrightness, bool displayInvert, bool displayScroll, const std::string &callsign)
    : m_ipaddress("(ip unknown)"),
      m_displayType(displayType),
      m_displayBrightness(displayBrightness),
      m_displayInvert(displayInvert),
      m_displayScroll(displayScroll),
      m_callsign(callsign)
{
}

COLED::~COLED()
{
}


bool COLED::open()
{

    //init IP reading
    unsigned char info[100U];
    CNetworkInfo *m_network;


    info[0] = 0;

    m_network = new CNetworkInfo;
    m_network->getNetworkInterface(info);

    LogInfo("COLED:open(): get ipaddress is  %s", info);

    m_ipaddress = (char * )info;

    // SPI
    if (display.oled_is_spi_proto(m_displayType))
    {
        // SPI change parameters to fit to your LCD
        if ( !display.init(OLED_SPI_DC, OLED_SPI_RESET, OLED_SPI_CS, m_displayType) )
            return false;
    }
    else
    {
        // I2C change parameters to fit to your LCD
        if ( !display.init(OLED_I2C_RESET, m_displayType) )
            return false;
    }

    display.begin();

    display.invertDisplay(m_displayInvert ? 1 : 0);

    if (m_displayBrightness > 0U)
        display.setBrightness(m_displayBrightness);

    // init done
    display.clearDisplay();   // clears the screen  buffer
    display.display();        // display it (clear display)

    display.drawBitmap(0, 0, logo_glcd_bmp, 128, 16, WHITE);

#if 0
    OLED_statusbar();
    display.setCursor(0, OLED_LINE1);
    display.print("Startup");
#endif

    display.display();

    return true;
}


void  COLED::writeOledTimeBer(float time, float ber)
{
    display.fillRect(0, OLED_LINE4, display.width(), display.height(), BLACK);
    display.setCursor(5, OLED_LINE4);
    display.setTextSize( 1u);
    display.printf("Tx:%.1fS Ber:%.1f%%", time, ber);
    display.display();
}

void  COLED::writeOledRTimeBer(float time, float loss, float ber)
{
    display.fillRect(0, OLED_LINE4, display.width(), display.height(), BLACK);
    display.setCursor(5, OLED_LINE4);
    display.setTextSize( 1u);
    display.printf("Tx:%.1fS Loss:%.1f Ber:%.1f%%", time, loss, ber);
    display.display();
}


void COLED::setIdleInt()
{

    //init IP reading
    unsigned char info[100U];
    CNetworkInfo *m_network;


    info[0] = 0;

    m_network = new CNetworkInfo;
    m_network->getNetworkInterface(info);

    LogInfo("COLED:open(): get ipaddress is  %s", info);

    m_ipaddress = (char * )info;
    

    m_mode = MODE_IDLE;

    // SPI
    if (display.oled_is_spi_proto(m_displayType))
    {
        // SPI change parameters to fit to your LCD
        display.init(OLED_SPI_DC, OLED_SPI_RESET, OLED_SPI_CS, m_displayType);

    }
    else
    {
        // I2C change parameters to fit to your LCD
        display.init(OLED_I2C_RESET, m_displayType);
    }


    display.begin();
    display.invertDisplay(m_displayInvert);
    if( m_displayBrightness)
        display.setBrightness(m_displayBrightness);

    display.clearDisplay();


    display.setTextColor(WHITE);

    display.setCursor(0, OLED_LINE1);
    display.setTextSize(1);
    display.printf("%s", m_ipaddress.c_str());

    // Show call sign
    display.setCursor(30, OLED_LINE2);
    display.setTextSize(2);
    display.printf("%s", m_callsign.c_str());

    display.display();


    // Show temperature
    FILE *temperatureFile;
    double T;
    temperatureFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
    if (temperatureFile == NULL)
    {
        LogError("No temperature available");
    }
    else
    {
        fscanf (temperatureFile, "%lf", &T);

        // On BI7JTA nero PI，no need div  1000
        //
        // T /= 1000;

        LogInfo("The temperature is %6.3f C.\n", T);

        int temp = (int)T;
        LogInfo ("Temperature is:%d", temp);

        display.setCursor(18, OLED_LINE4);
        display.setTextSize(2);
        display.printf("Temp:%d C", temp);
        fclose (temperatureFile);
        display.setTextSize(1);
        display.display();
    }

    OLED_statusbar();

}



void COLED::setErrorInt(const char *text)
{
    m_mode = MODE_ERROR;


//    display.stopscroll();
    display.clearDisplay();

    OLED_statusbar();

    display.setCursor(0, OLED_LINE1);
    display.printf("%s\n", text);

    display.display();
}

void COLED::setLockoutInt()
{
    m_mode = MODE_LOCKOUT;

    display.clearDisplay();
    OLED_statusbar();

    display.setCursor(0, 30);
    display.setTextSize(3);
    display.print("Lockout");

    display.setTextSize(1);
    display.display();
}

void COLED::writeDStarInt(const char *my1, const char *my2, const char *your, const char *type, const char *reflector)
{
    m_mode = MODE_DSTAR;

    display.clearDisplay();
    display.fillRect(0, OLED_LINE1, display.width(), display.height(), BLACK); //clear everything beneath logo

    display.setCursor(0, OLED_LINE2);
    display.printf("%s %.8s/%4.4s", type, my1, my2);
    display.setCursor(0, OLED_LINE3);
    display.printf("-> %.8s", your);
    display.setCursor(0, OLED_LINE5);
    display.printf("via %.8s", reflector);
    OLED_statusbar();
    display.display();

}


void COLED::clearDStarInt()
{
    display.fillRect(0, OLED_LINE1, display.width(), display.height(), BLACK); //clear everything beneath the logo

    display.setCursor(40, 38);
    display.print("Listening");
    display.display();
}




void COLED::writeDMRInt(unsigned int slotNo, const std::string &src, bool group, const std::string &dst, const char *type)
{

    if (slotNo != 1U)
    {

        display.clearDisplay();

        display.setTextColor(WHITE);
        display.fillRect(0, OLED_LINE2, display.width(), display.height(), BLACK);

        display.setCursor(5, OLED_LINE2);
        display.setTextSize(2);
        display.printf("%s %s", type, src.c_str());

        display.setCursor(5, OLED_LINE4);
        display.setTextSize(1);
        if (group)
            display.printf("%s%s", "", dst.c_str() );
        else
            display.printf("%s%s", "TG", dst.c_str() );

        OLED_statusbar();
    }


    display.display();

}


void COLED::writeDMRTAInt(unsigned int slotNo,  unsigned char *talkerAlias, const char *type)
{
    char text[40U];

    if (m_mode != MODE_DMR)
        m_mode = MODE_DMR;

    if (type[0] != ' ' )
    {

        if ( slotNo != 1)
        {
            ::sprintf(text, "%s", talkerAlias);

            if (strlen((char *)talkerAlias) > 20 - 4)
                printf ("%s%s\n", "TalkerAlias>20:", text);

            if (strlen((char *)talkerAlias) > 24 - 4)
                printf ("%s%s\n", "TalkerAlias:>24", text);

            if (strlen((char *)talkerAlias) > 16 - 4)
                printf ("%s%s\n", "TalkerAlias>16:", text);

            display.fillRect(68, OLED_LINE4, display.width(), 10, BLACK);
            display.fillRect(0, OLED_LINE5, display.width(), 10, BLACK);

            display.setCursor(68, OLED_LINE4);
            display.printf("%s%s", "TA:", text);
            display.display();

        }
    }

    OLED_statusbar();
    display.display();
}




void COLED::clearDMRInt(unsigned int slotNo)
{

#if 0
    if (slotNo == 1U)
    {
        display.fillRect(0, OLED_LINE2, display.width(), 20, BLACK);
        display.setCursor(0, OLED_LINE2);
        display.print("1 Listening");
    }
    else
    {
        display.fillRect(0, OLED_LINE4, display.width(), 20, BLACK);
        display.setCursor(0, OLED_LINE4);
        display.print("2 Listening");
    }


    display.display();
#endif

}

void COLED::writeFusionInt(const char *source, const char *dest, const char *type, const char *origin)
{

    m_mode = MODE_YSF;

    display.clearDisplay();
    display.fillRect(0, OLED_LINE1, display.width(), display.height(), BLACK);

    display.setCursor(0, OLED_LINE2);
    display.printf("%s %.10s", type, source);

    display.setCursor(0, OLED_LINE3);
    display.printf("  %.10s", dest);

    OLED_statusbar();
    display.display();
}

void COLED::clearFusionInt()
{
    display.fillRect(0, OLED_LINE1, display.width(), display.height(), BLACK);

    display.setCursor(40, 38);
    display.print("Listening");

    display.display();
}

void COLED::writeP25Int(const char *source, bool group, unsigned int dest, const char *type)
{
    m_mode = MODE_P25;

    display.clearDisplay();
    display.fillRect(0, OLED_LINE1, display.width(), display.height(), BLACK);

    display.setCursor(0, OLED_LINE2);
    display.printf("%s %.10s", type, source);

    display.setCursor(0, OLED_LINE3);
    display.printf("  %s%u", group ? "TG" : "", dest);

    OLED_statusbar();
    display.display();
}

void COLED::clearP25Int()
{
    display.fillRect(0, OLED_LINE1, display.width(), display.height(), BLACK);

    display.setCursor(40, 38);
    display.print("Listening");

    display.display();
}

void COLED::writeCWInt()
{
    display.clearDisplay();

    display.setCursor(0, 30);
    display.setTextSize(3);
    display.print("CW TX");

    display.setTextSize(1);
    display.display();
    display.startscrollright(0x02, 0x0f);
}

void COLED::clearCWInt()
{
    display.clearDisplay();

    display.setCursor(0, 30);
    display.setTextSize(3);
    display.print("Idle");

    display.setTextSize(1);
    display.display();
    display.startscrollleft(0x02, 0x0f);
}

void COLED::close()
{
    display.setCursor(0, 30);
    display.setTextSize(3);
    display.print("MMDVM STOP");

    display.close();
}

void COLED::OLED_statusbar()
{

    display.stopscroll();

    display.fillRect(0, OLED_STATUSBAR, display.width(), 16, BLACK);

    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.setTextSize(1U);



    if (m_mode == MODE_DMR )
        display.drawBitmap(0, 0, logo_dmr_bmp, 128, 16, WHITE);
    else if (m_mode == MODE_DSTAR)
        display.drawBitmap(0, 0, logo_dstar_bmp, 128, 16, WHITE);
    else if (m_mode == MODE_YSF)
        display.drawBitmap(0, 0, logo_fusion_bmp, 128, 16, WHITE);
    else if (m_mode == MODE_P25)
        display.print("P25");
    else
    {
#if 1        
        // Show time
        time_t   now;         //实例化time_t结构
        struct   tm     *timenow;         //实例化tm结构指针
        time(&now);//此函数会返回从分元1970年1月1日的UTC时间从0时0分0秒算起到现在所经过的秒数.如果t是一个非空指针的话, 此函数会将返回值存在t所指的内存中.
        
        //time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
        timenow   =   localtime(&now);
        //localtime函数把从time取得的时间now换算成你电脑中的时间(就是你设置的地区)
        printf("Local   time   is   %s/n", asctime(timenow));
        //上句中asctime函数把时间转换成字符，通过printf()函数输出
        display.printf("%s", asctime(timenow));
#else
        display.drawBitmap(0, 0, logo_glcd_bmp, 128, 16, WHITE);
#endif

    }

    display.display();

    // if scroll = 0, stop scroll
    //			 = 1, left to right
    //			 = 2, right to left
    
    switch(m_displayScroll)
    {
    	case 1:
		    display.startscrollright(0x00U , 0x02U);
    		break;
    	case 2:
		    display.startscrollleft(0x00U , 0x02U);
    		break;

    	default:
    		break;
    }


}
