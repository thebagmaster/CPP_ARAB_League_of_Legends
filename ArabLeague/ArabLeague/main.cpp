#include <iostream>
#include <windows.h>
#include <winver.h>
#include "curl\curl.h"
#define PAGESIZE 4096
#define ISCLIENT 0
using namespace std;

void storeName(char*);
bool existsInArray(char*);
bool isBanned(char*);
bool isSummoner(char);
void ban(bool unban=false);
void readMem();
void getBans();
void resetGlobals();
void printTitle();
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
bool SetClipboardText(char*);

char people[30][30];
char bans[5000][30];
int  peopleI = 0;
int  bansNum = 0;

int main()
{
    char tag;
    char clip[200] = "ARABLEAGUE MONITOR STARTED... arableague.zymichost.com... please Be patient, scanning room for bans ";
    do
    {
        int i;
        SetClipboardText(clip);
        resetGlobals();
        getBans();
        readMem();
        printTitle();
        memset(clip, 0, 200);

        for(i = 0; i < peopleI; i++)
        {
            cout << i << ":" << people[i];
            if(isBanned(people[i]))
            {
                HANDLE hConsole = GetStdHandle (STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                cout << " BANNED";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                strcat(clip,people[i]);
                strcat(clip," IS BANNED, Boot Please \n");
            }
            if(!strcmp(people[i],"thebagmaster"))
            {
                HANDLE hConsole = GetStdHandle (STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                cout << " CREATOR";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            if(!strcmp(people[i],"darnfool")||!strcmp(people[i],"mookiefish")||!strcmp(people[i],"epitaph07")||!strcmp(people[i],"h0j0")||!strcmp(people[i],"wickedjackal")||!strcmp(people[i],"darkcellmp")||!strcmp(people[i],"biggyph00l"))
            {
                HANDLE hConsole = GetStdHandle (STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                cout << " COMRADE";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            cout << endl;
        }
        strcat(clip," Ban Check Ended ");
        SetClipboardText(clip);
//        for(i = 0; i < bansNum; i++)
//            cout << i << ":" << bans[i] << endl;
        cout << endl;
        cout << "b #\tban\t" << "r\trefresh" << endl;
        cout << "q\tquit\t";
        if(!ISCLIENT)
            cout << "u\tunban";
        cout << endl;
        cin >> tag;
        switch(tag)
        {
            case 'b':
                ban();
                system("PAUSE");
                break;
            case 'u':
                if(!ISCLIENT)
                    ban(true);
                else
                    tag = 'p';
                system("PAUSE");
                break;
            case 'r':
                break;
            case 'q':
                cout << "quitting..." << endl;
                break;
            default:
                cout << "unknown command " << tag << endl;
                system("PAUSE");
        }
        system("CLS");
    }while(tag != 'q');
    return 0;
}

void readMem()
{
    DWORD pid;
    HWND handle=FindWindow(0,"PVP.net Client");
    GetWindowThreadProcessId(handle, &pid);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    char *base = (char *)0;
    int d = 0;
    do {
        char buffer[PAGESIZE];
        if (ReadProcessMemory(hProcess, base, buffer, PAGESIZE, NULL)!=0)
        {
            for(int i = 0; i < PAGESIZE; i++)
            {
                if(buffer[i] == 0x01 &&
                   buffer[i+1] == 0x04 &&
                   buffer[i+2] == 0x00 &&
                   buffer[i+3] == 0x04 &&
                   buffer[i+4] == 0x00 &&
                   buffer[i+5] == 0x04 &&
                   buffer[i+6] == 0x00 &&
                   buffer[i+7] == 0x02 &&
                   buffer[i+8] == 0x06)
                {//players
                    char temp[30];
                    int k = 0;
                    int j = i+6;
                    while(buffer[j] != 0x04 && k <= 20)
                    {
                        if(isSummoner(buffer[j]))
                            temp[k++] = tolower(buffer[j]);
                        j++;
                    }
                    temp[k++] = '\0';
                    //cout << temp << endl;
                    if(k>1)
                        storeName(temp);
                }
            }
        }
        base += PAGESIZE;
        if(++d % 18000 == 0)
            cout << ".";
    } while (base != 0);
    CloseHandle(hProcess);
}
void storeName(char* temp)
{
    if(!existsInArray(temp))
        strcpy (people[peopleI++],temp);
}
bool existsInArray(char* temp)
{
    for(int i = 0; i < peopleI; i++)
        if(strcmp(people[i],temp)==0)
            return true;
    return false;
}
bool isBanned(char* temp)
{
    for(int i = 0; i < bansNum; i++)
        if(strcmp(bans[i],temp)==0)
            return true;
    return false;
}
bool isSummoner(char temp)
{
    return ((temp > 96 && temp < 123)||(temp > 64 && temp < 91)||(temp > 47 && temp < 58));
}
void ban(bool unban)
{
    int index;
    bool named = false;
    char temp[30];

    cin >> index;
    if(index == -1)
    {
        cout << endl << "enter name: ";
        cin >> temp;
        named = true;
    }

    if((named && !isBanned(temp))||(!named && !isBanned(people[index]))||unban)
    {
        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        if(curl) {
            char url[80] = "http://arab.zymichost.com/";
            if(ISCLIENT)
                strcat(url, "clientban.php");
            else
                strcat(url, "log.php");
            strcat(url, "?text=");;
            if(named)
                strcat(url, temp);
            else
                strcat(url, people[index]);
            if(unban && !ISCLIENT)
                strcat(url, "&unban=true");
            curl_easy_setopt(curl, CURLOPT_URL, url);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
    }
    else
        cout << "Already Banned..." << endl;
    char clip[200];
    memset(clip, 0, 200);
    if(named)
        strcat(clip,temp);
    else
        strcat(clip,people[index]);
    if(ISCLIENT)
        strcat(clip," has been banned.\n");
    else
        strcat(clip," has been permabanned.\n");
    SetClipboardText(clip);
    cout << clip;
}
void getBans()
{
    cout << "getting bans...";
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://arab.zymichost.com/");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    cout << " done" << endl;
}
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    int max=size*nmemb;
    char sitebans[max];
    strncpy(sitebans, (char*)ptr, max);
    for(int i=0, j=0; i < max; i++,j++)
    {

        if(isSummoner(sitebans[i]))
            bans[bansNum][j] = sitebans[i];
        else
        {
            j = -1;
            bansNum++;
        }
    }
    return 0;
}
bool SetClipboardText(char tmp[200])
{
    bool ok = FALSE;
    if (OpenClipboard(NULL))
    {
        EmptyClipboard();
        HGLOBAL hClipboardData;
        hClipboardData = GlobalAlloc(GMEM_DDESHARE,
                            strlen(tmp)+1);
        char * pchData;
        pchData = (char*)GlobalLock(hClipboardData);
        strcpy(pchData, LPCSTR(tmp));
        GlobalUnlock(hClipboardData);
        SetClipboardData(CF_TEXT,hClipboardData);
        CloseClipboard();
    }
    return ok;
}
void resetGlobals()
{
        for(int i = 0; i < peopleI; i++)
            memset(people[i], 0, 30);
        for(int i = 0; i < bansNum; i++)
            memset(bans[i], 0, 30);
        peopleI = 0;
        bansNum = 0;
}
void printTitle()
{
    system("CLS");
    HANDLE hConsole = GetStdHandle (STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    cout << endl << (char)201;
    for(int i=0; i < 25; i++)
        cout << (char)205;
    cout << (char)187 << endl;
    cout << (char)186;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << " ARABLEAGUE MONITOR v0.1 " ;
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    cout << (char)186;
    cout << endl << (char)200;
    for(int i=0; i < 25; i++)
        cout << (char)205;
    cout << (char)188 << endl << endl;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
