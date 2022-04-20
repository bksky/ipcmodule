#include <stdio.h>
#include <string>
#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif
#include "ipc_base.h"

using namespace std;

int sendcount = 0;
string buff1 = "[test1] hello test";
string buff2 = "[test2] this is test";

int main(int argc, char** argv){
    IAgoraIpcDataSender* pSender1 = createIpcSender();
    IAgoraIpcDataSender* pSender2 = createIpcSender();

    std::string id1 = "test1";
    std::string id2 = "test2";
    if(false == pSender1->initialize(id1)){
        cout << "IAgoraIpcDataSender initialize failed" << endl;
        return -1;
    }
    if(false == pSender2->initialize(id2)){
        cout << "IAgoraIpcDataSender initialize failed" << endl;
        return -1;
    }
    
    while(true){
        string data1 = buff1 + std::to_string(++sendcount);
        string data2 = buff2 + std::to_string(sendcount);
        pSender1->sendData((char*)data1.c_str(), data1.length());
        cout << "[test1] send data: " << data1 << endl;
        pSender2->sendData((char*)data2.c_str(), data2.length());
        cout << "[test2] send data: " << data2 << endl;
#ifdef WIN32
        Sleep(1000);
#endif
    }
    

    releaseIpcSender(pSender1);
    releaseIpcSender(pSender2);
    return 0;
}