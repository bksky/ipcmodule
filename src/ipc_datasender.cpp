#include "ipc_base.h"
#include "ipc_shm.h"
#include <mutex>

class AgoraIpcDataSender : public IAgoraIpcDataSender {
public:
  AgoraIpcDataSender() : m_ipcData(), m_initialized(false) {}

  ~AgoraIpcDataSender() {
    if (m_initialized) {
      m_ipcData.close_channel(0, CHANNEL_WRITE);
      m_ipcData.close();
      m_ipcData.remove(m_id);
    }
  };

  virtual bool initialize(const std::string &id) override {
    std::lock_guard<std::mutex> lock(m_mtx);
    if(m_initialized == true)
      return true;

    if (m_ipcData.create(id, 1) != 0) {
      return false;
    }
    if (m_ipcData.open(id) != 0) {
      m_ipcData.remove(id);
      return false;
    }
    if (m_ipcData.open_channel(0, CHANNEL_WRITE) != 0) {
      m_ipcData.close();
      m_ipcData.remove(id);
      return false;
    }
    m_id = id;
    m_initialized = true;
    return true;
  }

  virtual void sendData(char *payload, unsigned int len) override {
    std::lock_guard<std::mutex> lock(m_mtx);
    m_ipcData.write(0, payload, len);
  }

  virtual void sendMultiData(
      const std::vector<std::pair<char *, int32_t>> &payloads) override {
    std::lock_guard<std::mutex> lock(m_mtx);
    m_ipcData.write(0, payloads);
  }

  virtual void Disconnect() override {
    std::lock_guard<std::mutex> lock(m_mtx);
    m_ipcData.close_channel(0, CHANNEL_WRITE);
  }

private:
  shm_ipc<1, DATA_DELIVER_BLOCK_SIZE> m_ipcData;
  bool m_initialized;
  std::string m_id;
  std::mutex m_mtx;
};

IAgoraIpcDataSender* AGORA_CALL createIpcSender(){
    return new AgoraIpcDataSender();
}

void releaseIpcSender(IAgoraIpcDataSender* sender){
    if(sender){
        delete sender;
    }
}