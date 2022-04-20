#include <stdio.h>
#include <iostream>
#include <string>
#include <functional>
#ifdef WIN32
#include <windows.h>
#endif
#include "ipc_base.h"

using namespace std;
using namespace std::placeholders;

void eventCb(const char* buffer, int length){
    cout << "recv data: " << buffer << endl;
}

int main(int argc, char** argv){
    IAgoraIpcDataReceiver* pReceiver1 = createIpcReceiver();
    IAgoraIpcDataReceiver* pReceiver2 = createIpcReceiver();

    string id1 = "test1";
    string id2 = "test2";
    if(false == pReceiver1->initialize(id1, std::bind(&eventCb, _1, _2))){
        cout << "IAgoraIpcDataReceiver1 initialize failed" << endl;
        return -1;
    }
    if(false == pReceiver2->initialize(id2, std::bind(&eventCb, _1, _2))){
        cout << "IAgoraIpcDataReceiver1 initialize failed" << endl;
        return -1;
    }

    pReceiver1->run(true);
    pReceiver2->run(true);

    while(true){
#ifdef WIN32
        Sleep(1000);
#endif
    }

    pReceiver1->stop();
    pReceiver2->stop();

    releaseIpcReceiver(pReceiver1);
    releaseIpcReceiver(pReceiver2);
    return 0;
}