#include <stdio.h>
#include <iostream>
#include <string>
#include <functional>
#ifdef WIN32
#include <windows.h>
#endif
#include "ipc_base.h"
#include <array>
#include <vector>
#include <unordered_map>
using namespace std;
using namespace std::placeholders;

class IPCReceiverTest {
public:
	IPCReceiverTest();
};

std::string ipcName = "agoravideosource_";
std::string remotehannelId = "test";
unsigned int remoteUid = 0;

// local user channelId: "local"  "video_source"  "TRANSCODED", local uid: 0 1
// remote uid, agora channel remote userid 
std::string localChannelId1 = "local";
unsigned int localuid1 = 0;

struct video_ipc_info {
	int stride;
	int stride0;
	int width;
	int height;
	int strideU;
	int strideV;
	int length;
};

struct video_ipc_header_type {
	uint8_t format;
	uint8_t mirrored;
	uint16_t width;
	uint16_t height;
	uint16_t left;
	uint16_t top;
	uint16_t right;
	uint16_t bottom;
	uint16_t rotation;
	uint32_t timestamp;
};

using stream_buffer_type = std::vector<unsigned char>;

struct video_ipc_data {
	video_ipc_header_type type;
	video_ipc_info info;
	char* buffer;
};

void eventCb(const char* buffer, int length, unsigned int uid, const char* channelId){
	video_ipc_data* data = (video_ipc_data*)buffer;
	char szInfo[500] = { 0 };
	char* videoBuffer = (char*)data + sizeof(video_ipc_header_type) + sizeof(video_ipc_info);
	sprintf(szInfo, "uid:%u,channelId:%s, format=%d, stride=%d, width=%d, height=%d, strideU=%d, length=%d, videoBuffer=%x \n"
		, uid, channelId, data->type.format, data->info.stride, data->info.width, data->info.height, data->info.strideU, data->info.length, videoBuffer);
	cout << "recv data: " << szInfo << endl;
	
}
std::string getId(std::string channelId, unsigned int uid)
{
	char szUid[20] = { 0 };
	sprintf(szUid, "%d", uid);
	return  ipcName + localChannelId1 + "_" + szUid;
}

class IPCManagerTest
{
public:
	IPCManagerTest()
	{

	}

	~IPCManagerTest()
	{
		Clear();
	}

	void Clear()
	{
		for (auto& iter : m_ipcRecv) {
			releaseIpcReceiver(iter.second);
		}
		m_ipcRecv.clear();
	}

	bool Initialize(unsigned int uid, std::string channelId);
	void Run(unsigned int uid, std::string channelId);
	static IPCManagerTest* instance();
private:
	std::unordered_map < std::string, IAgoraIpcDataReceiver*> m_ipcRecv;
	//std::unordered_map<std::string,
	//	std::unordered_map<unsigned int, IAgoraIpcDataReceiver*>> m_ipcRecv;

};
IPCManagerTest* IPCManagerTest::instance()
{
	static IPCManagerTest manager;
	return &manager;
}

int main(int argc, char** argv) {

	if (!IPCManagerTest::instance()->Initialize(localuid1, localChannelId1)) {
		cout << "IAgoraIpcDataReceiver1 initialize failed" << endl;
		return -1;
	}

	IPCManagerTest::instance()->Run(localuid1, localChannelId1);

	if (argc == 3) {
		remotehannelId = argv[1];
		std::string strUserId = argv[2];
		remoteUid = strtoul(strUserId.c_str(), nullptr, 10);


		if (!IPCManagerTest::instance()->Initialize(remoteUid, remotehannelId)) {
			cout << "IAgoraIpcDataReceiver2 initialize failed" << endl;
			return -1;
		}
		IPCManagerTest::instance()->Run(remoteUid, remotehannelId);
	}
#ifdef WIN32
	while (getchar() != 'q') {

		Sleep(1000);
	}
#else
	while (true) {

}
#endif
    return 0;
}

bool IPCManagerTest::Initialize(unsigned int uid, std::string channelId) {
	std::string id = getId(channelId, uid);
	if (m_ipcRecv.find(id) != m_ipcRecv.end())
		return true;
	IAgoraIpcDataReceiver* pReceiver = createIpcReceiver();
	bool ret = pReceiver->initialize(id, std::bind(&eventCb, _1, _2, _3, _4), localuid1, localChannelId1);
	if (ret) {
		m_ipcRecv[id] = pReceiver;
	}
	return ret;
}

void IPCManagerTest::Run(unsigned int uid, std::string channelId)
{
	std::string id = getId(channelId, uid);
	if (m_ipcRecv.find(id) == m_ipcRecv.end())
		return;
	m_ipcRecv[id]->run(true);
}

