#pragma once

#include "esphome.h"
#include <EEPROM.h>
#include <Somfy_Remote_Lib.h>

class SomfyComponent : public esphome::Component {
 public:
  float get_setup_priority() const override { return setup_priority::DATA; }

  void setup() override {
    pinMode(pin_, OUTPUT);
    digitalWrite(pin_, LOW);
    somfy_remote_ = new SomfyRemote(pin_, remote_id_, new EEPROMRollingCodeStorage(remote_name_.c_str()));
    register_service(&SomfyComponent::send_command_up, "somfy_up");
    register_service(&SomfyComponent::send_command_down, "somfy_down");
    register_service(&SomfyComponent::send_command_stop, "somfy_stop");
    register_service(&SomfyComponent::send_command_prog, "somfy_prog");
    ESP_LOGD("SomfyComponent", "Component initialized with pin: %d, remote_id: 0x%06X, remote_name: %s",
             pin_, remote_id_, remote_name_.c_str());
  }

  void loop() override {}

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

  // Settery do przekazywania wartości z YAML
  void set_pin(uint8_t pin) { pin_ = pin; }
  void set_remote_id(uint32_t remote_id) { remote_id_ = remote_id; }
  void set_remote_name(const std::string &remote_name) { remote_name_ = remote_name; }

  // Definicja CONFIG_SCHEMA
  static void setup_from_yaml(const std::string &yaml_key, const YamlObject &yaml) {
    auto *comp = new SomfyComponent();
    App.register_component(comp);

    if (yaml["pin"].is_defined()) {
      comp->set_pin(yaml["pin"].as<uint8_t>());
    }
    if (yaml["remote_id"].is_defined()) {
      comp->set_remote_id(yaml["remote_id"].as<uint32_t>());
    }
    if (yaml["remote_name"].is_defined()) {
      comp->set_remote_name(yaml["remote_name"].as<std::string>());
    }
  }

  DECLARE_CONFIG_SCHEMA(
      CONF_SCHEMA()
          .add_key("pin", CONF_PIN, uint8_t)
          .add_key("remote_id", CONF_INT, uint32_t)
          .add_key("remote_name", CONF_STRING, std::string)
  )

 private:
  uint8_t pin_;
  uint32_t remote_id_;
  std::string remote_name_;
  SomfyRemote* somfy_remote_ = nullptr;
};

namespace esphome {
  ESPHOME_REGISTRY(SomfyComponent, "somfy_component", SomfyComponent::setup_from_yaml);
}
