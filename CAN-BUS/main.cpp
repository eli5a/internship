#include <stdio.h>
#include <tchar.h>
#include "SerialClass.h"	// Library described above
#include <string>

#include <fstream>
#include <iomanip>

#include <iostream>

using namespace std;


const char* pfad1 = "C:\\Users\\hile\\Desktop\\versuch1.txt";
const char* pfad2 = "C:\\Users\\hile\\Desktop\\daten1.txt";

fstream versuch(pfad1, ios::out|ios::app);
fstream daten(pfad2, ios::out|ios::app);

void write(Serial*SP,char *data, int &len)
{
    char data_write[200] = {0};
    int nbChar = 0;

    cout << endl << "____________________" << endl;
    versuch << endl << "____________________" << endl;
    strcpy(data_write, data);
    nbChar = len;
    if(SP->WriteData(data_write,nbChar))
    {
        for(int i=0;i<nbChar;i++)
            if(data_write[i] == 0x0d)
                data_write[i] = 0x0a;

        for(int i=0;i<nbChar;i++)
        {
           cout << data_write[i];
           versuch << data_write[i];
        }

        cout << " wurde gesendet" << endl;
        versuch << " wurde gesendet" << endl;
    }
    Sleep(1000);
    versuch.close();
    return;

}

void read(Serial*SP, char *data_read, int &received_Bytes, int &nbbytes)
{
    received_Bytes = SP->ReadData(data_read,nbbytes);
    if(received_Bytes > 0)
    {
        data_read[received_Bytes] = 0;
        for(int i=0;i<received_Bytes;i++)
             if(data_read[i] == 0x0d)
                data_read[i] = 0x0a;
    }
    return;
}

int value(char*data_read,int i, int j)
{
    char* buff[100];
    char conv[10] = "0x00";

    conv[2] = data_read[i];
    conv[3] = data_read[j];
    int value = strtol(conv,buff,16);

    return value;
}

void supported(char*data_read, int&received_Bytes)
{

    int a = value(data_read,12,13);
    int b = value(data_read,15,16);
    int c = value(data_read,18,19);
    int d = value(data_read,21,22);

    char buffer[32];

    itoa(a,buffer,2);
    cout << setw(8) << setfill('0') << buffer << " ";
    versuch << setw(8) << setfill('0') << buffer << " ";
    daten << setw(8) << setfill('0') << buffer << " ";
    itoa(b,buffer,2);
    cout << setw(8) << setfill('0') << buffer << " ";
    versuch << setw(8) << setfill('0') << buffer << " ";
    daten << setw(8) << setfill('0') << buffer << " ";
    itoa(c,buffer,2);
    cout << setw(8) << setfill('0') << buffer << " ";
    versuch << setw(8) << setfill('0') << buffer << " ";
    daten << setw(8) << setfill('0') << buffer << " ";
    itoa(d,buffer,2);
    cout << setw(8) << setfill('0') << buffer << " ";
    versuch << setw(8) << setfill('0') << buffer << " ";
    daten << setw(8) << setfill('0') << buffer << " ";

    cout << endl;
    versuch << endl;
    daten << endl;

    versuch.close();
    daten.close();
    return;
}

void geschw(char*data_read)
{
    daten << "Geschwindigkeit: ";
    versuch << "Geschwindigkeit: ";

    int A = value(data_read,12,13);

    versuch << dec << A << " " << "km/h" <<endl;
    daten << dec << A << " " << "km/h" <<endl;

    versuch.close();
    daten.close();
    return;
}

void umdreh(char*data_read)
{
    daten << "Motor-Umdrehungen: ";
    versuch << "Motor-Umdrehungen: ";

    int A = value(data_read,12,13);
    int B = value(data_read,15,16);

    int erg = ((256*A)+B)/4;
    versuch << dec << erg << " " << "rpm" << endl;
    daten << dec << erg << " " << "rpm" << endl;

    versuch.close();
    daten.close();
    return;
}

void temp(char*data_read)
{
    daten << "Motor-Kuehlwassertemperatur: ";
    versuch << "Motor-Kuehlwassertemperatur: ";

    int A = value(data_read,12,13);

    int erg = A-40;
    versuch << dec << erg << " " << "°C" << endl;
    daten << dec << erg << " " << "°C" << endl;

    versuch.close();
    daten.close();
    return;
}

void zeit(char*data_read)
{
    daten << "Zeit seit Motorstart: ";
    versuch << "Zeit seit Motorstart: ";

    int A = value(data_read,12,13);
    int B = value(data_read,15,16);

    int erg = (256*A)+B;
    versuch << dec << erg << " " << "s" << endl;
    daten << dec << erg << " " << "s" << endl;

    versuch.close();
    daten.close();
    return;
}

void print(Serial*SP,char *data, int &len, int sleeptime)
{
    char data_read[200] = {0};
    int nbbytes = 200;
    int received_Bytes = 0;

    write(SP, data, len);
    Sleep(sleeptime);
    read(SP,data_read, received_Bytes, nbbytes);

    if(received_Bytes > 0)
    {

        cout << "Anzahl zurueckgeschickter Bytes: " << received_Bytes << endl;
        versuch << "Anzahl zurueckgeschickter Bytes: " << received_Bytes << endl;

        if(!strcmp(data,"01 00\r"))
        {
            for(int i=0;i<received_Bytes;i++)
            {
                cout << data_read[i];
                versuch << data_read[i];
            }
            cout << endl;
            versuch << endl;

            for(int i=0;i<received_Bytes;i++)
            {
                printf("%x ",data_read[i]);
                versuch << hex << int (data_read[i]) << " ";
            }
            cout << endl;
            versuch << endl;
            supported(data_read,received_Bytes);
        }

        else
        {
            for(int i=0;i<received_Bytes;i++)
            {
                cout << data_read[i];
                versuch << data_read[i];
            }
            cout << endl;
            versuch << endl;
            for(int i=0;i<received_Bytes;i++)
            {
                printf("%x ",data_read[i]);
                versuch << hex << int (data_read[i]) << " ";
            }
            cout << endl;
            versuch << endl;

            if(!strcmp(data,"01 0D\r"))
            {
                geschw(data_read);
            }
            else if(!strcmp(data,"01 0C\r"))
            {
                umdreh(data_read);
            }
            else if(!strcmp(data,"01 05\r"))
            {
                temp(data_read);
            }
            else if(!strcmp(data,"01 1F\r"))
            {
                zeit(data_read);
            }
            else
            {
                daten << endl << endl << endl;
                for(int i=0;i<received_Bytes;i++)
                {
                    daten << data_read[i];
                }
                daten << endl;
            }
        }
    }
    versuch.close();
    daten.close();
}


int _tmain(int argc, _TCHAR* argv[])
{
	printf("Welcome to the serial test app!\n\n");

	Serial* SP = new Serial("\\\\.\\COM6");    //Com23

	if (SP->IsConnected())
    {
        cout << "We're connected" << endl;
    }
    else
    {
        return -1;
    }

    char data[200] = {0};
    int len = 0;

    data[0] = 0x0D;
    len = 1;

    print(SP,data,len,0);

    strcpy(data, "atz\r");
    len = 4;

    print(SP,data,len,0);

    strcpy(data, "atz\r");
    len = 4;

    print(SP,data,len,0);

    print(SP,data,len,0);

    ///Welche PIDs werden unterstützt

    strcpy(data, "01 00\r");
    len = 6;

    print(SP,data,len,500);

    ///Geschwindigkeit, Motorumdrehung, Kühltemperatur und Zeit seit Motorstart auslesen als Schleife

    for(int g=0;g<500;g++)
    {
        strcpy(data, "01 0D\r");
        len = 6;
        print(SP,data,len,500);

        strcpy(data, "01 0C\r");
        len = 6;
        print(SP,data,len,500);

        strcpy(data, "01 05\r");
        len = 6;
        print(SP,data,len,500);

        strcpy(data, "01 1F\r");
        len = 6;
        print(SP,data,len,500);
    }

    SP->~Serial();
    return 0;
}
