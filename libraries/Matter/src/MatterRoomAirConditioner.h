#ifndef MATTER_ROOM_AIR_CONDITIONER_H
#define MATTER_ROOM_AIR_CONDITIONER_H

#include "Matter.h"
//#include "devices/DeviceThermostat.h"
//#include "devices/DeviceFan.h"
#include "devices/DeviceRoomAC.h"
#include <platform/CHIPDeviceLayer.h>
#include <app-common/zap-generated/attributes/Accessors.h>

using namespace chip;
using namespace ::chip::DeviceLayer;

class MatterRoomAC : public ArduinoMatterAppliance {
public:
  enum ac_mode_t {
    OFF      = 0,
    AUTO     = 1,  // SystemModeEnum::kAuto
    COOL     = 3,  // SystemModeEnum::kCool
    HEAT     = 4,  // SystemModeEnum::kHeat
	E_HEAT   = 5,  //SystemModeEnum::kEmergencyHeat
	// MODE_PRE_COOL =  6,  //SystemModeEnum::kPrecooling
    FAN_ONLY = 7,  // SystemModeEnum::kFanOnly
    DRY      = 8,  // SystemModeEnum::kDry
	// MODE_SLEEP      = 9,  // SystemModeEnum::kSleep
  };

  //Already defined on Fan device
  /*enum fan_mode_t {
    FAN_OFF    = 0,
    FAN_LOW    = 1,
    FAN_MEDIUM = 2,
    FAN_HIGH   = 3,
    FAN_AUTO   = 5,
  };*/
    enum fan_mode_t {
    Off,
    Low,
    Med,
    High,
    On,
    Auto,
    Smart
  };
  //TBD ECONO_COOL as Bool->Not yet on matter spec
  /*
  ACTypeEnum (tipus de compressor)
COOLING_FIXED (1), HEAT_PUMP_FIXED (2), COOLING_INVERTER (3), HEAT_PUMP_INVERTER (4)*/
/*
    ACLouverPositionEnum (posició de persiana)
CLOSED (1), OPEN (2), QUARTER (3), HALF (4), THREE_QUARTERS (5)*/
  /*//Not yet on Matter standard
  enum tilt_vane_mode_t {
	  TILT
	  Horizontal
	  3/4
	  1/2
	  1/4
	  Vertical
	  Swing
	  Auto
  }
  */
  /*ACTypeEnum (tipus de compressor)
COOLING_FIXED (1), HEAT_PUMP_FIXED (2), COOLING_INVERTER (3), HEAT_PUMP_INVERTER (4)*/
  /* ACRefrigerantTypeEnum (tipus de refrigerant)
R22 (1), R410A (2), R407C (3)*/
  MatterRoomAC();
  ~MatterRoomAC();
  bool begin();
  void end();
  void set_onoff(bool value);
  bool get_onoff();
  
  //Fan
  void set_mode(DeviceFan::fan_mode_t fan_mode);
  DeviceFan::fan_mode_t get_mode();
  //Termostat
  int16_t get_local_temperature_raw();
  void set_local_temperature_raw(int16_t local_temp);
  //uint16_t get_local_humidity_raw();
  //void set_local_humidity_raw(uint16_t local_humi);
  int16_t get_cooling_setpoint_raw();
  void set_cooling_setpoint_raw(int16_t cooling_setpoint);
  int16_t get_heating_setpoint_raw();
  void set_heating_setpoint_raw(int16_t heating_setpoint);

  float get_local_temperature();
  void set_local_temperature(float local_temp);
  //float get_local_humidity();
  //void set_local_humidity(float local_humi);
  float get_cooling_setpoint();
  void set_cooling_setpoint(float cooling_setpoint);
  float get_heating_setpoint();
  void set_heating_setpoint(float heating_setpoint);
  //Setpoints Cooling
  void set_absolute_minimum_cooling_setpoint(float abs_min_cooling_setpoint);
  void set_minimum_cooling_setpoint(float min_cooling_setpoint);
  void set_absolute_maximum_cooling_setpoint(float abs_max_cooling_setpoint);
  void set_maximum_cooling_setpoint(float max_cooling_setpoint);
  float get_absolute_minimum_cooling_setpoint();
  float get_minimum_cooling_setpoint();
  float get_absolute_maximum_cooling_setpoint();
  float get_maximum_cooling_setpoint();
  //Setpoints Heating
  void set_absolute_minimum_heating_setpoint(float abs_min_heating_setpoint);
  void set_minimum_heating_setpoint(float min_heating_setpoint);
  void set_absolute_maximum_heating_setpoint(float abs_max_heating_setpoint);
  void set_maximum_heating_setpoint(float max_heating_setpoint);
  float get_absolute_minimum_heating_setpoint();
  float get_minimum_heating_setpoint();
  float get_absolute_maximum_heating_setpoint();
  float get_maximum_heating_setpoint();
  
  ac_mode_t get_system_mode();
  void set_system_mode(ac_mode_t system_mode);
 private:
  //DeviceThermostat* thermostat_device;
  //DeviceFan* fan_device;
  DeviceRoomAC* room_ac_device;
  EmberAfEndpointType* device_endpoint;
  DataVersion* endpoint_dataversion_storage;
  bool initialized;
};
  
  #endif // MATTER_ROOM_AIR_CONDITIONER_H