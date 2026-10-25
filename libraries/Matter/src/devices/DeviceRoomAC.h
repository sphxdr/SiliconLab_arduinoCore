#pragma once

#include "MatterDevice.h"
#include "DeviceFan.h"

class DeviceRoomAC : public Device
{
public:
  enum Changed_t{
    kChanged_HeatingSetpointValue = kChanged_Last << 1,
	kChanged_CoolingSetpointValue = kChanged_Last << 2,
    kChanged_LocalTemperatureValue = kChanged_Last << 3,
	kChanged_LocalHumidityValue = kChanged_Last << 4,
    kChanged_FanPercentSetting = kChanged_Last << 5,
    kChanged_FanPercentCurrent = kChanged_Last << 6,
    kChanged_FanModeSetting = kChanged_Last << 7,
    kChanged_SystemModeValue = kChanged_Last << 8,
  } Changed;

  /*DeviceThermostat(const char* device_name, int16_t local_temperature, int16_t heating_setpoint);
  DeviceFan(const char* device_name);
  DeviceAirPurifier(const char* device_name, uint8_t air_quality_value);*/
  DeviceRoomAC(const char* device_name);

  int16_t GetLocalTemperatureValue();
  void SetLocalTemperatureValue(int16_t local_temp);
  //int16_t GetLocalHumidityValue();
  //void SetLocalHumidityValue(int16_t local_humidity);
  int16_t GetCoolingSetpointValue();
  void SetCoolingSetpointValue(int16_t cooling_setpoint);
  int16_t GetHeatingSetpointValue();
  void SetHeatingSetpointValue(int16_t heating_setpoint);
  
  uint8_t GetSystemMode();
  void SetSystemMode(uint8_t system_mode);

  void SetAbsMinHeatingSetpoint(int16_t abs_min_heating_setpoint);
  int16_t GetAbsMinHeatingSetpoint();
  void SetMinHeatingSetpoint(int16_t min_heating_setpoint);
  int16_t GetMinHeatingSetpoint();

  void SetAbsMaxHeatingSetpoint(int16_t abs_max_heating_setpoint);
  int16_t GetAbsMaxHeatingSetpoint();
  void SetMaxHeatingSetpoint(int16_t max_heating_setpoint);
  int16_t GetMaxHeatingSetpoint();
  
  void SetAbsMinCoolingSetpoint(int16_t abs_min_heating_setpoint);
  int16_t GetAbsMinCoolingSetpoint();
  void SetMinCoolingSetpoint(int16_t min_heating_setpoint);
  int16_t GetMinCoolingSetpoint();

  void SetAbsMaxCoolingSetpoint(int16_t abs_max_heating_setpoint);
  int16_t GetAbsMaxCoolingSetpoint();
  void SetMaxCoolingSetpoint(int16_t max_heating_setpoint);
  int16_t GetMaxCoolingSetpoint();
  
  uint8_t GetFanPercentSetting();
  void SetFanPercentSetting(uint8_t percent);
  uint8_t GetFanPercentCurrent();
  void SetFanPercentCurrent(uint8_t percent);
  void SetFanMode(uint8_t fan_mode);
  uint8_t GetFanMode();
  uint8_t GetFanModeSequence();
  uint8_t GetFanSpeedMax();
  
  //uint32_t GetRoomAcClusterFeatureMap();
  //uint16_t GetRoomAcClusterRevision();
  uint8_t GetControlSequenceOfOperation();
  uint32_t GetThermostatClusterFeatureMap();
  uint16_t GetThermostatClusterRevision();
  uint32_t GetFanClusterFeatureMap();
  uint16_t GetFanClusterRevision();

  CHIP_ERROR HandleReadEmberAfAttribute(ClusterId clusterId,
                                        chip::AttributeId attributeId,
                                        uint8_t* buffer,
                                        uint16_t maxReadLength) override;

  CHIP_ERROR HandleWriteEmberAfAttribute(ClusterId clusterId,
                                         chip::AttributeId attributeId,
                                         uint8_t* buffer) override;

private:
  void HandleRoomACDeviceStatusChanged(Changed_t itemChangedMask);
//DeviceRoomAC
  
  int16_t local_temperature;
  int16_t cooling_setpoint;
  int16_t heating_setpoint;
  uint8_t system_mode;
  int16_t abs_min_cooling_setpoint;
  int16_t abs_min_heating_setpoint;
  int16_t min_cooling_setpoint;
  int16_t min_heating_setpoint;
  int16_t abs_max_cooling_setpoint;
  int16_t abs_max_heating_setpoint;
  int16_t max_cooling_setpoint;
  int16_t max_heating_setpoint;
  
  uint8_t current_fan_percent;
  DeviceFan::fan_mode_t current_fan_mode;

  static const uint8_t thermostat_control_sequence_of_operation = 4u; // Cooling & Heating
  
  static const uint8_t fan_mode_sequence        = 2u;   // Off/Low/Med/High/Auto
  static const uint8_t fan_speed_max            = 100u; // 0-100%
  
  static const uint32_t thermostat_cluster_feature_map          = 23u; // Heating capability enabled (Bit 0), Cooling enabled (Bit 1), Auto enabled (Bit 20)
  static const uint16_t thermostat_cluster_revision             = 5u; //Up to 10u
  static const uint32_t fan_cluster_feature_map = 1u;   // 1-100 speeds supported
  static const uint16_t fan_cluster_revision    = 1u;
};

/*
//Futuribles

enum class ACLouverPositionEnum : uint8_t {
kClosed = 0x01,
kOpen = 0x02,
kQuarter = 0x03,
kHalf = 0x04,
kThreeQuarters = 0x05,
// All received enum values that are not listed above will be mapped
// to kUnknownEnumValue. This is a helper enum value that should only
// be used by code to process how it handles receiving and unknown
// enum value. This specific should never be transmitted.
kUnknownEnumValue = 0,
};


// Enum for ACRefrigerantTypeEnum
enum class ACRefrigerantTypeEnum : uint8_t {
kUnknown = 0x00,
kR22 = 0x01,
kR410a = 0x02,
kR407c = 0x03,
//kR32 = 0xFF, //->Mitsu B209 HDR 20260705
// All received enum values that are not listed above will be mapped
// to kUnknownEnumValue. This is a helper enum value that should only
// be used by code to process how it handles receiving and unknown
// enum value. This specific should never be transmitted.
kUnknownEnumValue = 4,
};

// Bitmap for ACErrorCodeBitmap
enum class ACErrorCodeBitmap : uint32_t {
kCompressorFail = 0x1,
kRoomSensorFail = 0x2,
kOutdoorSensorFail = 0x4,
kCoilSensorFail = 0x8,
kFanFail = 0x10,
};

// Bitmap for Feature
enum class Feature : uint32_t {
kMultiSpeed = 0x1,
kAuto = 0x2,
kRocking = 0x4,
kWind = 0x8,
kStep = 0x10,
kAirflowDirection = 0x20,
};

// Bitmap for RockBitmap
enum class RockBitmap : uint8_t {
kRockLeftRight = 0x1,
kRockUpDown = 0x2,
kRockRound = 0x4,
};
*/

/*
//Docs
// Enum for ControlSequenceOfOperationEnum
enum class ControlSequenceOfOperationEnum : uint8_t {
kCoolingOnly = 0x00,
kCoolingWithReheat = 0x01,
kHeatingOnly = 0x02,
kHeatingWithReheat = 0x03,
kCoolingAndHeating = 0x04,
kCoolingAndHeatingWithReheat = 0x05,
// All received enum values that are not listed above will be mapped
// to kUnknownEnumValue. This is a helper enum value that should only
// be used by code to process how it handles receiving and unknown
// enum value. This specific should never be transmitted.
kUnknownEnumValue = 6,
};

// Enum for ThermostatRunningModeEnum
enum class ThermostatRunningModeEnum : uint8_t {
kOff = 0x00,
kCool = 0x03,
kHeat = 0x04,
// All received enum values that are not listed above will be mapped
// to kUnknownEnumValue. This is a helper enum value that should only
// be used by code to process how it handles receiving and unknown
// enum value. This specific should never be transmitted.
kUnknownEnumValue = 1,
};

// Enum for SystemModeEnum
enum class SystemModeEnum : uint8_t {
kOff = 0x00,
kAuto = 0x01,
kCool = 0x03,
kHeat = 0x04,
kEmergencyHeat = 0x05,
kPrecooling = 0x06,
kFanOnly = 0x07,
kDry = 0x08,
kSleep = 0x09,
// All received enum values that are not listed above will be mapped
// to kUnknownEnumValue. This is a helper enum value that should only
// be used by code to process how it handles receiving and unknown
// enum value. This specific should never be transmitted.
kUnknownEnumValue = 2,
};


*/