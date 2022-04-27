#include "ipc_base.h"
#include "ipc_shm.h"
#include <thread>

class AgoraIpcDataReceiver : public IAgoraIpcDataReceiver {
public:
  AgoraIpcDataReceiver()
      : m_handler(nullptr), m_initialized(false), m_ipcData() {}

  ~AgoraIpcDataReceiver() { stop(); }

  virtual bool
  initialize(const std::string &id,
             const std::function<void(const char *, int, unsigned int, const char*)> &handler,
	  unsigned int uid, const std::string& channelId) override {
    if(m_initialized == true)
      return true;
      
    if (m_ipcData.open(id) != 0) {
      return false;
    }
    if (m_ipcData.open_channel(0, CHANNEL_READ) != 0) {
      m_ipcData.close();
      return false;
    }
	m_channelId = channelId;
	m_uid = uid;
    m_id = id;
    m_handler = handler;
    m_initialized = true;
    return true;
  };

  virtual void run(bool async = false) override {
    auto callback = [this]() {
      char *buffer = new char[DATA_DELIVER_BLOCK_SIZE];

      if (!buffer)
        return;

      while (true) {
        if (m_ipcData.read(0, buffer, DATA_DELIVER_BLOCK_SIZE) >= 0) {
          if (m_handler) {
			  m_handler(buffer, DATA_DELIVER_BLOCK_SIZE, m_uid, m_channelId.c_str());
          }
        } else {
          break;
        }
      }
      delete[] buffer;
    };
    if (async) {
      m_thread.reset(new std::thread(callback));
    } else {
      callback();
    }
  };

   void stop() {
    if (m_initialized) {
      m_ipcData.close_channel(0, CHANNEL_READ);

      if (m_thread && m_thread->joinable())
        m_thread->join();
      m_ipcData.close();
    }
  };

private:
  std::function<void(const char *, int, unsigned int, const char*)> m_handler;
  std::string m_id;
  unsigned int m_uid;
  std::string m_channelId;
  bool m_initialized;
  shm_ipc<1, DATA_DELIVER_BLOCK_SIZE> m_ipcData;
  std::unique_ptr<std::thread> m_thread;
};

IAgoraIpcDataReceiver* AGORA_CALL createIpcReceiver(){
    return new AgoraIpcDataReceiver();
}

void releaseIpcReceiver(IAgoraIpcDataReceiver* receiver){
    if(receiver){
        delete receiver;
    }
}