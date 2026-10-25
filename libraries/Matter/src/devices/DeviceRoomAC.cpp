#include "DeviceRoomAC.h"
#include "DeviceFan.h"

DeviceRoomAC::DeviceRoomAC(const char* device_name) :
  Device(device_name),
  local_temperature(local_temperature),
  //local_humidity(local_humidity),
  heating_setpoint(heating_setpoint),
  cooling_setpoint(cooling_setpoint),
  system_mode(0),
  abs_min_heating_setpoint(700),
  min_heating_setpoint(1600),
  abs_max_heating_setpoint(3200),
  max_heating_setpoint(3000),
  abs_min_cooling_setpoint(700),
  min_cooling_setpoint(1600),
  abs_max_cooling_setpoint(3200),
  max_cooling_setpoint(3000),
  current_fan_percent(0),
  current_fan_mode(DeviceFan::fan_mode_t::Off)
{
  this->SetDeviceType(device_type_t::kDeviceType_RoomAirConditioner);
}

//Local Temperature
int16_t DeviceRoomAC::GetLocalTemperatureValue()
{
  return this->local_temperature;
}

void DeviceRoomAC::SetLocalTemperatureValue(int16_t local_temp)
{
  bool changed = this->local_temperature != local_temp;
  ChipLogProgress(DeviceLayer, "RoomAirConditioner[%s]: new local temp='%d'", this->device_name, local_temp);
  this->local_temperature = local_temp;

  if (changed) {
    this->HandleRoomACDeviceStatusChanged(kChanged_LocalTemperatureValue);
    CallDeviceChangeCallback();
  }
}
//Local Humidity
/*int16_t DeviceRoomAC::GetLocalHumidityValue();
{
  return this->local_humidity;
}

void DeviceRoomAC::SetLocalHumidityValue(int16_t local_humid);
{
  bool changed = this->local_humidity != local_humid;
  ChipLogProgress(DeviceLayer,"RoomAirConditioner[%s]: new local temp='%d'",this->device_name,local_humid);
  this->local_humidity = local_humid;
  
  if (changed) {
    this->HandleRoomACDeviceStatusChanged(kChanged_LocalHumidityValue);
	CallDeviceChangeCallback();
  }
}*/
//Thermostat Setpoints
int16_t DeviceRoomAC::GetHeatingSetpointValue()
{
  return this->heating_setpoint;
}

void DeviceRoomAC::SetHeatingSetpointValue(int16_t heating_setpoint)
{
  bool changed = this->heating_setpoint != heating_setpoint;

  if (heating_setpoint < this->abs_min_heating_setpoint) {
    heating_setpoint = this->abs_min_heating_setpoint;
  }
  if (heating_setpoint > this->abs_max_heating_setpoint) {
    heating_setpoint = this->abs_max_heating_setpoint;
  }

  ChipLogProgress(DeviceLayer, "RoomAirConditioner[%s]: new heating setpoint='%d'", this->device_name, heating_setpoint);
  this->heating_setpoint = heating_setpoint;

  if (changed) {
    this->HandleRoomACDeviceStatusChanged(kChanged_HeatingSetpointValue);
    CallDeviceChangeCallback();
  }
}
int16_t DeviceRoomAC::GetCoolingSetpointValue()
{
  return this->cooling_setpoint;
}

void DeviceRoomAC::SetCoolingSetpointValue(int16_t cooling_setpoint)
{
  bool changed = this->cooling_setpoint != cooling_setpoint;

  if (cooling_setpoint < this->abs_min_cooling_setpoint) {
    cooling_setpoint = this->abs_min_cooling_setpoint;
  }
  if (cooling_setpoint > this->abs_max_cooling_setpoint) {
    cooling_setpoint = this->abs_max_cooling_setpoint;
  }

  ChipLogProgress(DeviceLayer, "RoomAirConditioner[%s]: new cooling setpoint='%d'", this->device_name, cooling_setpoint);
  this->cooling_setpoint = cooling_setpoint;

  if (changed) {
    this->HandleRoomACDeviceStatusChanged(kChanged_CoolingSetpointValue);
    CallDeviceChangeCallback();
  }
}
//Thermostat SystemMode
uint8_t DeviceRoomAC::GetSystemMode()
{
  return this->system_mode;
}

void DeviceRoomAC::SetSystemMode(uint8_t system_mode)
{
  bool changed = this->system_mode != system_mode;
  ChipLogProgress(DeviceLayer, "RoomAirConditioner[%s]: new system mode='%u'", this->device_name, system_mode);
  this->system_mode = system_mode;

  if (changed) {
    this->HandleRoomACDeviceStatusChanged(kChanged_SystemModeValue);
    CallDeviceChangeCallback();
  }
}
//Termostat Boundaries
void DeviceRoomAC::SetAbsMinHeatingSetpoint(int16_t abs_min_heating_setpoint)//Heating
{
  if (abs_min_heating_setpoint > this->min_heating_setpoint) {
    return;
  }
  this->abs_min_heating_setpoint = abs_min_heating_setpoint;
}

int16_t DeviceRoomAC::GetAbsMinHeatingSetpoint()
{
  return this->abs_min_heating_setpoint;
}

void DeviceRoomAC::SetMinHeatingSetpoint(int16_t min_heating_setpoint)
{
  if (min_heating_setpoint < this->abs_min_heating_setpoint) {
    return;
  }
  this->min_heating_setpoint = min_heating_setpoint;
}

int16_t DeviceRoomAC::GetMinHeatingSetpoint()
{
  return this->min_heating_setpoint;
}

void DeviceRoomAC::SetAbsMaxHeatingSetpoint(int16_t abs_max_heating_setpoint)
{
  if (this->max_heating_setpoint > abs_max_heating_setpoint) {
    return;
  }
  this->abs_max_heating_setpoint = abs_max_heating_setpoint;
}

int16_t DeviceRoomAC::GetAbsMaxHeatingSetpoint()
{
  return this->abs_max_heating_setpoint;
}

void DeviceRoomAC::SetMaxHeatingSetpoint(int16_t max_heating_setpoint)
{
  if (max_heating_setpoint > this->abs_max_heating_setpoint) {
    return;
  }
  this->max_heating_setpoint = max_heating_setpoint;
}

int16_t DeviceRoomAC::GetMaxHeatingSetpoint()
{
  return this->max_heating_setpoint;
}
void DeviceRoomAC::SetAbsMinCoolingSetpoint(int16_t abs_min_cooling_setpoint)//Cooling
{
  if (abs_min_cooling_setpoint > this->min_cooling_setpoint) {
    return;
  }
  this->abs_min_cooling_setpoint = abs_min_cooling_setpoint;
}

int16_t DeviceRoomAC::GetAbsMinCoolingSetpoint()
{
  return this->abs_min_cooling_setpoint;
}

void DeviceRoomAC::SetMinCoolingSetpoint(int16_t min_cooling_setpoint)
{
  if (min_cooling_setpoint < this->abs_min_cooling_setpoint) {
    return;
  }
  this->min_cooling_setpoint = min_cooling_setpoint;
}

int16_t DeviceRoomAC::GetMinCoolingSetpoint()
{
  return this->min_cooling_setpoint;
}

void DeviceRoomAC::SetAbsMaxCoolingSetpoint(int16_t abs_max_cooling_setpoint)
{
  if (this->max_cooling_setpoint > abs_max_cooling_setpoint) {
    return;
  }
  this->abs_max_cooling_setpoint = abs_max_cooling_setpoint;
}

int16_t DeviceRoomAC::GetAbsMaxCoolingSetpoint()
{
  return this->abs_max_cooling_setpoint;
}

void DeviceRoomAC::SetMaxCoolingSetpoint(int16_t max_cooling_setpoint)
{
  if (max_cooling_setpoint > this->abs_max_cooling_setpoint) {
    return;
  }
  this->max_cooling_setpoint = max_cooling_setpoint;
}

int16_t DeviceRoomAC::GetMaxCoolingSetpoint()
{
  return this->max_cooling_setpoint;
}

//Fan PercentSetting
uint8_t DeviceRoomAC::GetFanPercentSetting()
{
  return this->current_fan_percent;
}

void DeviceRoomAC::SetFanPercentSetting(uint8_t percent)
{
  if (percent > 100) {
    percent = 100;
  }

  bool changed = this->current_fan_percent != percent;
  ChipLogProgress(DeviceLayer, "RoomAirConditioner[%s]: new fan percent='%d'", this->device_name, percent);
  this->current_fan_percent = percent;

  if (changed) {
    this->HandleRoomACDeviceStatusChanged(kChanged_FanPercentSetting);
    this->HandleRoomACDeviceStatusChanged(kChanged_FanPercentCurrent);
    CallDeviceChangeCallback();
  }
}

uint8_t DeviceRoomAC::GetFanPercentCurrent()
{
  return this->current_fan_percent;
}

void DeviceRoomAC::SetFanPercentCurrent(uint8_t percent)
{
  this->SetFanPercentSetting(percent);
}

void DeviceRoomAC::SetFanMode(uint8_t fan_mode)
{
  bool changed = this->current_fan_mode != fan_mode;
  ChipLogProgress(DeviceLayer, "RoomAirConditioner[%s]: new fan mode='%d'", this->device_name, fan_mode);
  this->current_fan_mode = (DeviceFan::fan_mode_t)fan_mode;

  if (changed) {
    this->HandleRoomACDeviceStatusChanged(kChanged_FanModeSetting);
    CallDeviceChangeCallback();
  }
}

uint8_t DeviceRoomAC::GetFanMode()
{
  return this->current_fan_mode;
}

uint8_t DeviceRoomAC::GetFanModeSequence()
{
  return this->fan_mode_sequence;
}

uint8_t DeviceRoomAC::GetFanSpeedMax()
{
  return this->fan_speed_max;
}
//RoomAirConditioner Matter Info
/*uint32_t  DeviceRoomAC::GetRoomAcClusterFeatureMap()
{
  return this->roomAC_cluster_feature_map;
}
uint16_t  DeviceRoomAC::GetRoomAcClusterRevision()
{
  return this->roomAC_cluster_revision;
}*/
uint8_t  DeviceRoomAC::GetControlSequenceOfOperation()
{
  return this->thermostat_control_sequence_of_operation;
}
//Termostat Matter Info
uint32_t DeviceRoomAC::GetThermostatClusterFeatureMap()
{
  return this->thermostat_cluster_feature_map;
}
uint16_t DeviceRoomAC::GetThermostatClusterRevision()
{
  return this->thermostat_cluster_revision;
}
//Fan Matter Info
uint32_t DeviceRoomAC::GetFanClusterFeatureMap()
{
  return this->fan_cluster_feature_map;
}
uint16_t DeviceRoomAC::GetFanClusterRevision()
{
  return this->fan_cluster_revision;
}
//Write Ember Endpoint
CHIP_ERROR DeviceRoomAC::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                    chip::AttributeId attributeId,
                                                    uint8_t* buffer,
                                                    uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }
 
  using namespace ::chip::app::Clusters::Thermostat::Attributes;
  using namespace ::chip::app::Clusters::FanControl::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadRoomACAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);
 
  if (clusterId == chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }
 
  // Aquest Device només gestiona aquests dos clústers al seu (únic) endpoint
  if ((clusterId != chip::app::Clusters::Thermostat::Id)
      && (clusterId != chip::app::Clusters::FanControl::Id)) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }
 
  if (clusterId == chip::app::Clusters::Thermostat::Id) {
    if ((attributeId == LocalTemperature::Id) && (maxReadLength == 2)) {
      int16_t localTemp = this->GetLocalTemperatureValue();
      memcpy(buffer, &localTemp, sizeof(localTemp));
    } else if ((attributeId == OccupiedCoolingSetpoint::Id) && (maxReadLength == 2)) {
      int16_t coolingSetpoint = this->GetCoolingSetpointValue();
      memcpy(buffer, &coolingSetpoint, sizeof(coolingSetpoint));
	} else if ((attributeId == OccupiedHeatingSetpoint::Id) && (maxReadLength == 2)) {
      int16_t heatingSetpoint = this->GetHeatingSetpointValue();
      memcpy(buffer, &heatingSetpoint, sizeof(heatingSetpoint));
    } else if ((attributeId == SystemMode::Id) && (maxReadLength == 1)) {
      uint8_t systemMode = this->GetSystemMode();
      memcpy(buffer, &systemMode, sizeof(systemMode));
    } else if ((attributeId == ControlSequenceOfOperation::Id) && (maxReadLength == 1)) {
      uint8_t seq_op = this->GetControlSequenceOfOperation();
      memcpy(buffer, &seq_op, sizeof(seq_op));
    } else if ((attributeId == AbsMinCoolSetpointLimit::Id) && (maxReadLength == 2)) {
      int16_t abs_min_cool_setpoint = this->GetAbsMinCoolingSetpoint();
      memcpy(buffer, &abs_min_cool_setpoint, sizeof(abs_min_cool_setpoint));
    } else if ((attributeId == AbsMaxCoolSetpointLimit::Id) && (maxReadLength == 2)) {
      int16_t abs_max_cool_setpoint = this->GetAbsMaxCoolingSetpoint();
      memcpy(buffer, &abs_max_cool_setpoint, sizeof(abs_max_cool_setpoint));
    } else if ((attributeId == MinCoolSetpointLimit::Id) && (maxReadLength == 2)) {
      int16_t min_cool_setpoint = this->GetMinCoolingSetpoint();
      memcpy(buffer, &min_cool_setpoint, sizeof(min_cool_setpoint));
    } else if ((attributeId == MaxCoolSetpointLimit::Id) && (maxReadLength == 2)) {
      int16_t max_cool_setpoint = this->GetMaxCoolingSetpoint();
      memcpy(buffer, &max_cool_setpoint, sizeof(max_cool_setpoint));
	} else if ((attributeId == AbsMinHeatSetpointLimit::Id) && (maxReadLength == 2)) {
      int16_t abs_min_heat_setpoint = this->GetAbsMinHeatingSetpoint();
      memcpy(buffer, &abs_min_heat_setpoint, sizeof(abs_min_heat_setpoint));
    } else if ((attributeId == AbsMaxHeatSetpointLimit::Id) && (maxReadLength == 2)) {
      int16_t abs_max_heat_setpoint = this->GetAbsMaxHeatingSetpoint();
      memcpy(buffer, &abs_max_heat_setpoint, sizeof(abs_max_heat_setpoint));
    } else if ((attributeId == MinHeatSetpointLimit::Id) && (maxReadLength == 2)) {
      int16_t min_heat_setpoint = this->GetMinHeatingSetpoint();
      memcpy(buffer, &min_heat_setpoint, sizeof(min_heat_setpoint));
    } else if ((attributeId == MaxHeatSetpointLimit::Id) && (maxReadLength == 2)) {
      int16_t max_heat_setpoint = this->GetMaxHeatingSetpoint();
      memcpy(buffer, &max_heat_setpoint, sizeof(max_heat_setpoint));
    } else if ((attributeId == ::chip::app::Clusters::Thermostat::Attributes::FeatureMap::Id) && (maxReadLength == 4)) {
      uint32_t featureMap = this->GetThermostatClusterFeatureMap();
      memcpy(buffer, &featureMap, sizeof(featureMap));
    } else if ((attributeId == ::chip::app::Clusters::Thermostat::Attributes::ClusterRevision::Id) && (maxReadLength == 2)) {
      uint16_t clusterRevision = this->GetThermostatClusterRevision();
      memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
    } else {
      return CHIP_ERROR_INVALID_ARGUMENT;
    }
  } else { // FanControl::Id
    if ((attributeId == FanMode::Id) && (maxReadLength == 1)) {
      uint8_t fanmode = this->GetFanMode();
      memcpy(buffer, &fanmode, sizeof(fanmode));
    } else if ((attributeId == FanModeSequence::Id) && (maxReadLength == 1)) {
      uint8_t fanmode_sequence = this->GetFanModeSequence();
      memcpy(buffer, &fanmode_sequence, sizeof(fanmode_sequence));
    } else if ((attributeId == PercentSetting::Id) && (maxReadLength == 1)) {
      uint8_t percent_setting = this->GetFanPercentSetting();
      memcpy(buffer, &percent_setting, sizeof(percent_setting));
    } else if ((attributeId == PercentCurrent::Id) && (maxReadLength == 1)) {
      uint8_t percent_current = this->GetFanPercentCurrent();
      memcpy(buffer, &percent_current, sizeof(percent_current));
    } else if ((attributeId == SpeedMax::Id) && (maxReadLength == 1)) {
      uint8_t speed_max = this->GetFanSpeedMax();
      memcpy(buffer, &speed_max, sizeof(speed_max));
    } else if ((attributeId == SpeedSetting::Id) && (maxReadLength == 1)) {
      uint8_t speed_setting = this->GetFanPercentSetting();
      memcpy(buffer, &speed_setting, sizeof(speed_setting));
    } else if ((attributeId == SpeedCurrent::Id) && (maxReadLength == 1)) {
      uint8_t speed_current = this->GetFanPercentCurrent();
      memcpy(buffer, &speed_current, sizeof(speed_current));
    } else if ((attributeId == ::chip::app::Clusters::FanControl::Attributes::FeatureMap::Id) && (maxReadLength == 4)) {
      uint32_t featureMap = this->GetFanClusterFeatureMap();
      memcpy(buffer, &featureMap, sizeof(featureMap));
    } else if ((attributeId == ::chip::app::Clusters::FanControl::Attributes::ClusterRevision::Id) && (maxReadLength == 2)) {
      uint16_t clusterRevision = this->GetFanClusterRevision();
      memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
    } else {
      return CHIP_ERROR_INVALID_ARGUMENT;
    }
  }
 
  return CHIP_NO_ERROR;
}
//Write Ember Endpoint
CHIP_ERROR DeviceRoomAC::HandleWriteEmberAfAttribute(ClusterId clusterId,
                                                     chip::AttributeId attributeId,
                                                     uint8_t* buffer)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::Thermostat::Attributes;
  using namespace ::chip::app::Clusters::FanControl::Attributes;
  ChipLogProgress(DeviceLayer, "HandleWriteRoomACAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  // Aquest Device només gestiona aquests dos clústers al seu (únic) endpoint
  if ((clusterId != chip::app::Clusters::Thermostat::Id)
      && (clusterId != chip::app::Clusters::FanControl::Id)) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }
  if (clusterId == chip::app::Clusters::Thermostat::Id) {
	  if (attributeId == OccupiedCoolingSetpoint::Id) {
		this->SetCoolingSetpointValue(*((int16_t*)buffer));
	  } else if (attributeId == OccupiedHeatingSetpoint::Id) {
		this->SetHeatingSetpointValue(*((int16_t*)buffer));
	  } else if (attributeId == SystemMode::Id) {
		this->SetSystemMode(*buffer);
	  } else {
		return CHIP_ERROR_INVALID_ARGUMENT;
	  }
} else { // FanControl::Id
    if (attributeId == PercentSetting::Id) {
      this->SetFanPercentSetting(*buffer);
    } else if (attributeId == SpeedSetting::Id) {
      this->SetFanPercentSetting(*buffer);
    } else if (attributeId == FanMode::Id) {
      this->SetFanMode(*buffer);
    } else {
      return CHIP_ERROR_INVALID_ARGUMENT;
    }
  }

  return CHIP_NO_ERROR;
}
void DeviceRoomAC::HandleRoomACDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;
 
  if (itemChangedMask & kChanged_HeatingSetpointValue) {
    ScheduleMatterReportingCallback(this->endpoint_id, Thermostat::Id, Thermostat::Attributes::OccupiedHeatingSetpoint::Id);
  }
  if (itemChangedMask & kChanged_CoolingSetpointValue) {
    ScheduleMatterReportingCallback(this->endpoint_id, Thermostat::Id, Thermostat::Attributes::OccupiedCoolingSetpoint::Id);
  }
  if (itemChangedMask & kChanged_LocalTemperatureValue) {
    ScheduleMatterReportingCallback(this->endpoint_id, Thermostat::Id, Thermostat::Attributes::LocalTemperature::Id);
  }
  /*if (itemChangedMask & kChanged_LocalHumidityValue) {
    ScheduleMatterReportingCallback(this->endpoint_id, Thermostat::Id, Thermostat::Attributes::LocalHumidity::Id);
  }*/
  if (itemChangedMask & kChanged_SystemModeValue) {
    ScheduleMatterReportingCallback(this->endpoint_id, Thermostat::Id, Thermostat::Attributes::SystemMode::Id);
  }
  if (itemChangedMask & kChanged_FanPercentSetting) {
    ScheduleMatterReportingCallback(this->endpoint_id, FanControl::Id, FanControl::Attributes::PercentSetting::Id);
  }
  if (itemChangedMask & kChanged_FanPercentCurrent) {
    ScheduleMatterReportingCallback(this->endpoint_id, FanControl::Id, FanControl::Attributes::PercentCurrent::Id);
  }
  if (itemChangedMask & kChanged_FanModeSetting) {
    ScheduleMatterReportingCallback(this->endpoint_id, FanControl::Id, FanControl::Attributes::FanMode::Id);
  }
}
