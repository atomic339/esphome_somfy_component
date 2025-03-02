#pragma once

#include "esphome.h"
#include <EEPROM.h>
#include <Somfy_Remote_Lib.h>  // Biblioteka zewnętrzna legion2/Somfy_Remote_Lib

class SomfyComponent : public Component {
 public:
  SomfyComponent(uint8_t pin, uint32_t remote_id, const std::string &remote_name)
      : pin_(pin), remote_id_(remote_id), remote_name_(remote_name) {}

  void setup() override {
    // Inicjalizacja pinu
    pinMode(pin_, OUTPUT);
    digitalWrite(pin_, LOW);

    // Inicjalizacja Somfy Remote
    somfy_remote_ = new SomfyRemote(remote_id_, new EEPROMRollingCodeStorage(remote_name_.c_str()));
  }

  void loop() override {
    // Tutaj ewentualnie logika cykliczna, jeśli potrzebna
  }

  void send_command_up() {
    somfy_remote_->sendCommand(SomfyCommand::Up);
  }

  void send_command_down() {
    somfy_remote_->sendCommand(SomfyCommand::Down);
  }

  void send_command_stop() {
    somfy_remote_->sendCommand(SomfyCommand::Stop);
  }

  void send_command_prog() {
    somfy_remote_->sendCommand(SomfyCommand::Prog);
  }

 private:
  uint8_t pin_;
  uint32_t remote_id_;
  std::string remote_name_;
  SomfyRemote* somfy_remote_;
};
