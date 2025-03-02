#pragma once

#include "esphome.h"
#include <EEPROM.h>
#include <Somfy_Remote_Lib.h>  // Zewnętrzna biblioteka legion2/Somfy_Remote_Lib

class SomfyComponent : public esphome::Component {
 public:
  // Konstruktor z parametrami przekazywanymi z YAML
  SomfyComponent(uint8_t pin, uint32_t remote_id, const std::string &remote_name)
      : pin_(pin), remote_id_(remote_id), remote_name_(remote_name) {}

  // Metoda wywoływana podczas inicjalizacji
  void setup() override {
    // Inicjalizacja pinu
    pinMode(pin_, OUTPUT);
    digitalWrite(pin_, LOW);

    // Inicjalizacja obiektu SomfyRemote
    somfy_remote_ = new SomfyRemote(pin_, remote_id_, new EEPROMRollingCodeStorage(remote_name_.c_str()));

    // Rejestracja serwisów dla Home Assistant
    register_service(&SomfyComponent::send_command_up, "somfy_up");
    register_service(&SomfyComponent::send_command_down, "somfy_down");
    register_service(&SomfyComponent::send_command_stop, "somfy_stop");
    register_service(&SomfyComponent::send_command_prog, "somfy_prog");

    ESP_LOGD("SomfyComponent", "Component initialized with pin: %d, remote_id: 0x%06X, remote_name: %s",
             pin_, remote_id_, remote_name_.c_str());
  }

  // Metoda wywoływana w pętli (opcjonalna)
  void loop() override {
    // Obecnie pusta, ale można tu dodać logikę cykliczną, jeśli potrzebna
  }

  // Metody wysyłające komendy do Somfy
  void send_command_up() {
    ESP_LOGD("SomfyComponent", "Sending UP command");
    somfy_remote_->sendCommand(SomfyCommand::Up);
  }

  void send_command_down() {
    ESP_LOGD("SomfyComponent", "Sending DOWN command");
    somfy_remote_->sendCommand(SomfyCommand::Down);
  }

  void send_command_stop() {
    ESP_LOGD("SomfyComponent", "Sending STOP command");
    somfy_remote_->sendCommand(SomfyCommand::Stop);
  }

  void send_command_prog() {
    ESP_LOGD("SomfyComponent", "Sending PROG command");
    somfy_remote_->sendCommand(SomfyCommand::Prog);
  }

 private:
  uint8_t pin_;              // Pin GPIO do wysyłania sygnału
  uint32_t remote_id_;       // ID pilota Somfy (np. 0x1A7A11)
  std::string remote_name_;  // Nazwa pilota (do przechowywania kodu w EEPROM)
  SomfyRemote* somfy_remote_; // Wskaźnik do obiektu SomfyRemote
};

// Ustawienie typu komponentu w ESPHome
namespace esphome {
  // Rejestracja komponentu w systemie ESPHome
  ESPHOME_REGISTRY(SomfyComponent, "somfy_component");
}  // namespace esphome
