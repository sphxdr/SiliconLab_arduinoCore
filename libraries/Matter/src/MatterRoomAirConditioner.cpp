#include "MatterRoomAirConditioner.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

static const EmberAfDeviceType kRoomAirConditionerDeviceTypes[] = { { DEVICE_TYPE_ROOM_AIR_CONDITIONER, DEVICE_VERSION_DEFAULT } };

constexpr CommandId thermostatIncomingCommands[] = {
  app::Clusters::Thermostat::Commands::SetpointRaiseLower::Id,
  kInvalidCommandId,
};

// Temperature sensor cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(thermostatAttrsRoomAC)
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::LocalTemperature::Id, INT16S, 2, 0),                                /* Local Temperature */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::ControlSequenceOfOperation::Id, INT8U, 1, 0),                       /* Control Sequence Of Operation */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::SystemMode::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE),                 /* System Mode */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::OccupiedHeatingSetpoint::Id, INT16S, 2, ATTRIBUTE_MASK_WRITABLE),   /* Occupied Heating Setpoint */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::AbsMinHeatSetpointLimit::Id, INT16S, 2, 0),                         /* Absolute Minimum Heating Setpoint */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::AbsMaxHeatSetpointLimit::Id, INT16S, 2, 0),                         /* Absolute Maximum Heating Setpoint */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::MinHeatSetpointLimit::Id, INT16S, 2, 0),                            /* Minimum Heating Setpoint */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::MaxHeatSetpointLimit::Id, INT16S, 2, 0),                            /* Maximum Heating Setpoint */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::OccupiedCoolingSetpoint::Id, INT16S, 2, ATTRIBUTE_MASK_WRITABLE),
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::AbsMinCoolSetpointLimit::Id, INT16S, 2, 0),
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::AbsMaxCoolSetpointLimit::Id, INT16S, 2, 0),
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::MinCoolSetpointLimit::Id, INT16S, 2, 0),
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::MaxCoolSetpointLimit::Id, INT16S, 2, 0),
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                                    /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                                 /* ClusterRevision auto added by LIST_END */



// Fan control cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(fanControlAttrsRoomAC)
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::FanMode::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE),            /* FanMode */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::FanModeSequence::Id, INT8U, 1, 0),                          /* FanModeSequence */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::PercentSetting::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE),     /* PercentSetting */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::PercentCurrent::Id, INT8U, 1, 0),                           /* PercentCurrent */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::SpeedMax::Id, INT8U, 1, 0),                                 /* SpeedMax */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::SpeedSetting::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE),       /* SpeedSetting */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::SpeedCurrent::Id, INT8U, 1, 0),                             /* SpeedCurrent */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                            /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                         /* ClusterRevision auto added by LIST_END */

// Room AC endpoint cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(roomAcEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(Thermostat::Id, thermostatAttrsRoomAC, ZAP_CLUSTER_MASK(SERVER), thermostatIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(FanControl::Id, fanControlAttrsRoomAC, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterRoomAC
 ******************************************************************************/
MatterRoomAC::MatterRoomAC() :
  //room_ac_device(nullptr),
  //fan_device(nullptr),
  room_ac_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterRoomAC
 ******************************************************************************/
MatterRoomAC::~MatterRoomAC()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterRoomAC instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterRoomAC::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceRoomAC* new_room_ac_device = new (std::nothrow)DeviceRoomAC("Room Air Conditioner");
  if (new_room_ac_device == nullptr) {
    return false;
  }
  new_room_ac_device->SetReachable(true);
  new_room_ac_device->SetProductName("Room Air Conditioner");
  
  // Set the device instance pointer in the base class
  this->base_matter_device = new_room_ac_device;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(new_room_ac_device);
    return false;
  }
  new_endpoint->cluster = roomAcEndpointClusters;
  new_endpoint->clusterCount = ArraySize(roomAcEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(roomAcEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_sensor_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_sensor_data_version == nullptr) {
    delete(new_room_ac_device);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(new_room_ac_device,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(kRoomAirConditionerDeviceTypes),
                                 Span<DataVersion>(new_sensor_data_version, ArraySize(roomAcEndpointClusters)),
                                 1);
  if (result < 0) {
    delete(new_room_ac_device);
    free(new_endpoint);
    free(new_sensor_data_version);
    return false;
  }

  this->room_ac_device = new_room_ac_device;
  this->device_endpoint = new_endpoint;
  this->endpoint_dataversion_storage = new_sensor_data_version;
  this->initialized = true;
  return true;
}

/***************************************************************************//**
 * Deinitializes the MatterRoomAC instance
 ******************************************************************************/
void MatterRoomAC::end()
{
  if (!this->initialized) {
    return;
  }
  (void)RemoveDeviceEndpoint(this->room_ac_device);
  free(this->device_endpoint);
  free(this->endpoint_dataversion_storage);
  delete(this->room_ac_device);
  this->initialized = false;
}

/***************************************************************************//**
 * Gets the thermostat's currently set raw local temperature value
 * The value is in celsius x 100.
 *
 * @return the locally measured raw temperature value
 ******************************************************************************/
int16_t MatterRoomAC::get_local_temperature_raw()
{
  return this->room_ac_device->GetLocalTemperatureValue();
}

/***************************************************************************//**
 * Sets the thermostat's locally measured raw temperature value
 * The accepted value is in celsius x 100.
 *
 * @param[in] local_temp the locally measured raw temperature value to set
 ******************************************************************************/
void MatterRoomAC::set_local_temperature_raw(int16_t local_temp)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->room_ac_device->SetLocalTemperatureValue(local_temp);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Sets the thermostat's raw cooling setpoint value
 * The accepted value is in celsius x 100.
 *
 * @param[in] cooling_setpoint the raw cooling setpoint to be set
 ******************************************************************************/
void MatterRoomAC::set_cooling_setpoint_raw(int16_t cooling_setpoint)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->room_ac_device->SetCoolingSetpointValue(cooling_setpoint);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Gets the thermostat's currently set raw cooling setpoint value
 * The value is in celsius x 100.
 *
 * @return the currently set raw cooling setpoint value
 ******************************************************************************/
int16_t MatterRoomAC::get_cooling_setpoint_raw()
{
  return this->room_ac_device->GetCoolingSetpointValue();
}

/***************************************************************************//**
 * Sets the thermostat's raw heating setpoint value
 * The accepted value is in celsius x 100.
 *
 * @param[in] heating_setpoint the raw heating setpoint to be set
 ******************************************************************************/
void MatterRoomAC::set_heating_setpoint_raw(int16_t heating_setpoint)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->room_ac_device->SetHeatingSetpointValue(heating_setpoint);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Gets the thermostat's currently set raw heating setpoint value
 * The value is in celsius x 100.
 *
 * @return the currently set raw heating setpoint value
 ******************************************************************************/
int16_t MatterRoomAC::get_heating_setpoint_raw()
{
  return this->room_ac_device->GetHeatingSetpointValue();
}

/***************************************************************************//**
 * Gets the thermostat's currently set local temperature value in celsius
 *
 * @return the locally measured temperature value
 ******************************************************************************/
float MatterRoomAC::get_local_temperature()
{
  return (float)this->get_local_temperature_raw() / 100.0f;
}
/***************************************************************************//**
 * Sets the thermostat's locally measured temperature value in celsius
 *
 * @param[in] local_temp the locally measured temperature value to set
 ******************************************************************************/
void MatterRoomAC::set_local_temperature(float local_temp)
{
  uint16_t raw_temp = (int16_t)(local_temp * 100.0f);
  this->set_local_temperature_raw(raw_temp);
}

/***************************************************************************//**
 * Sets the sensors's raw measured value
 * The value is percents multiplied by 100 - from 0-10000.
 *
 * @param[in] value the measured humidity value to be set
 ******************************************************************************/
/*void MatterRoomAC::set_local_humidity_raw(uint16_t local_humid)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->room_ac_device->SetLocalHumidityValue(value);
  PlatformMgr().UnlockChipStack();
}
/***************************************************************************//**
 * Sets the sensors's measured value
 * The value is percents from 0-100.
 *
 * @param[in] value the measured humidity value to be set
 ******************************************************************************/
/*void MatterRoomAC::get_local_humidity(double value)
{
  uint16_t out_value = static_cast<uint16_t>(value * 100.0l);
  this->set_local_humidity_raw(out_value);
}

/***************************************************************************//**
 * Gets the sensors's current raw measured value
 * The value is percents multiplied by 100 - from 0-10000.
 *
 * @return the sensor's current humidity value
 ******************************************************************************/
/*uint16_t MatterRoomAC::get_local_humidity_raw()
{
  return this->room_ac_device->GetLocalHumidityValue();
}

/***************************************************************************//**
 * Gets the sensors's current measured value
 * The value is percents from 0-100.
 *
 * @return the sensor's current humidity value
 ******************************************************************************/
/*float MatterRoomAC::get_local_humidity()
{
  float out_value = static_cast<float>(this->get_local_humidity_raw()) / 100.0;
  return out_value;
}

/***************************************************************************//**
 * Float conversion operator for getting the measured value
 *
 * @return the sensor's current humidity value
 ******************************************************************************/
/*MatterRoomAC::operator float()
{
  return this->get_local_humidity();
}

/***************************************************************************//**
 * Double conversion operator for getting the measured value
 *
 * @return the sensor's current humidity value
 ******************************************************************************/
/*MatterRoomAC::operator double()
{
  return (double)this->get_local_humidity();
}

/***************************************************************************//**
 * Assignment operator to set the sensors's measured humidity value
 *
 * @param[in] value the measured humidity value to be set
 ******************************************************************************/
/*void MatterRoomAC::operator=(float value)
{
  this->set_local_humidity_raw(value);
}

/***************************************************************************//**
 * Assignment operator to set the sensors's measured humidity value
 *
 * @param[in] value the measured humidity value to be set
 ******************************************************************************/
/*void MatterRoomAC::operator=(double value)
{
  this->set_local_humidity_raw(value);
}

/***************************************************************************//**
 * Gets the thermostat's currently set cooling setpoint value in celsius
 *
 * @return the currently set cooling setpoint value
 ******************************************************************************/
float MatterRoomAC::get_cooling_setpoint()
{
  return (float)this->get_cooling_setpoint_raw() / 100.0f;
}

/***************************************************************************//**
 * Sets the thermostat's cooling setpoint value in celsius
 *
 * @param[in] cooling_setpoint the cooling setpoint to be set
 ******************************************************************************/
void MatterRoomAC::set_cooling_setpoint(float cooling_setpoint)
{
  uint16_t raw_temp = (int16_t)(cooling_setpoint * 100.0f);
  this->set_cooling_setpoint_raw(raw_temp);
}

/***************************************************************************//**
 * Sets the thermostat's absolute minimum cooling setpoint value in celsius
 * This is the cooling device's physical limit which cannot be changed.
 *
 * @param[in] abs_min_cooling_setpoint the absolute minimum value cooling can be set to
 ******************************************************************************/
void MatterRoomAC::set_absolute_minimum_cooling_setpoint(float abs_min_cooling_setpoint)
{
  uint16_t raw_temp = (int16_t)(abs_min_cooling_setpoint * 100.0f);
  this->room_ac_device->SetAbsMinCoolingSetpoint(raw_temp);
}

/***************************************************************************//**
 * Sets the thermostat's minimum cooling setpoint value in celsius
 * This limit may be changed by the user to enforce cooling policies.
 * Cannot be lower than the absolute minimum value.
 *
 * @param[in] min_cooling_setpoint the minimum value cooling can be set to
 ******************************************************************************/
void MatterRoomAC::set_minimum_cooling_setpoint(float min_cooling_setpoint)
{
  uint16_t raw_temp = (int16_t)(min_cooling_setpoint * 100.0f);
  this->room_ac_device->SetMinCoolingSetpoint(raw_temp);
}

/***************************************************************************//**
 * Sets the thermostat's absolute maximum cooling setpoint value in celsius
 * This is the cooling device's physical limit which cannot be changed.
 *
 * @param[in] abs_max_cooling_setpoint the absolute maximum value cooling can be set to
 ******************************************************************************/
void MatterRoomAC::set_absolute_maximum_cooling_setpoint(float abs_max_cooling_setpoint)
{
  uint16_t raw_temp = (int16_t)(abs_max_cooling_setpoint * 100.0f);
  this->room_ac_device->SetAbsMaxCoolingSetpoint(raw_temp);
}

/***************************************************************************//**
 * Sets the thermostat's maximum cooling setpoint value in celsius
 * This limit may be changed by the user to enforce cooling policies.
 * Cannot be higher than the absolute maximum value.
 *
 * @param[in] max_cooling_setpoint the maximum value cooling can be set to
 ******************************************************************************/
void MatterRoomAC::set_maximum_cooling_setpoint(float max_cooling_setpoint)
{
  uint16_t raw_temp = (int16_t)(max_cooling_setpoint * 100.0f);
  this->room_ac_device->SetMaxCoolingSetpoint(raw_temp);
}

/***************************************************************************//**
 * Gets the thermostat's absolute minimum cooling setpoint value in celsius
 * This is the cooling device's physical limit which cannot be changed.
 *
 * @return the absolute minimum value cooling can be set to
 ******************************************************************************/
float MatterRoomAC::get_absolute_minimum_cooling_setpoint()
{
  return (float)this->room_ac_device->GetAbsMinCoolingSetpoint() / 100.0f;
}

/***************************************************************************//**
 * Gets the thermostat's minimum cooling setpoint value in celsius
 * This limit may be changed by the user to enforce cooling policies.
 * Cannot be lower than the absolute minimum value.
 *
 * @return the minimum value cooling can be set to
 ******************************************************************************/
float MatterRoomAC::get_minimum_cooling_setpoint()
{
  return (float)this->room_ac_device->GetMinCoolingSetpoint() / 100.0f;
}

/***************************************************************************//**
 * Gets the thermostat's absolute maximum cooling setpoint value in celsius
 * This is the cooling device's physical limit which cannot be changed.
 *
 * @return the absolute maximum value cooling can be set to
 ******************************************************************************/
float MatterRoomAC::get_absolute_maximum_cooling_setpoint()
{
  return (float)this->room_ac_device->GetAbsMaxCoolingSetpoint() / 100.0f;
}

/***************************************************************************//**
 * Gets the thermostat's maximum cooling setpoint value in celsius
 * This limit may be changed by the user to enforce cooling policies.
 * Cannot be higher than the absolute maximum value.
 *
 * @return the maximum value cooling can be set to
 ******************************************************************************/
float MatterRoomAC::get_maximum_cooling_setpoint()
{
  return (float)this->room_ac_device->GetMaxCoolingSetpoint() / 100.0f;
}

/***************************************************************************//**
 * Gets the thermostat's currently set heating setpoint value in celsius
 *
 * @return the currently set heating setpoint value
 ******************************************************************************/
float MatterRoomAC::get_heating_setpoint()
{
  return (float)this->get_heating_setpoint_raw() / 100.0f;
}

/***************************************************************************//**
 * Sets the thermostat's heating setpoint value in celsius
 *
 * @param[in] heating_setpoint the heating setpoint to be set
 ******************************************************************************/
void MatterRoomAC::set_heating_setpoint(float heating_setpoint)
{
  uint16_t raw_temp = (int16_t)(heating_setpoint * 100.0f);
  this->set_heating_setpoint_raw(raw_temp);
}

/***************************************************************************//**
 * Sets the thermostat's absolute minimum heating setpoint value in celsius
 * This is the heating device's physical limit which cannot be changed.
 *
 * @param[in] abs_min_heating_setpoint the absolute minimum value heating can be set to
 ******************************************************************************/
void MatterRoomAC::set_absolute_minimum_heating_setpoint(float abs_min_heating_setpoint)
{
  uint16_t raw_temp = (int16_t)(abs_min_heating_setpoint * 100.0f);
  this->room_ac_device->SetAbsMinHeatingSetpoint(raw_temp);
}

/***************************************************************************//**
 * Sets the thermostat's minimum heating setpoint value in celsius
 * This limit may be changed by the user to enforce heating policies.
 * Cannot be lower than the absolute minimum value.
 *
 * @param[in] min_heating_setpoint the minimum value heating can be set to
 ******************************************************************************/
void MatterRoomAC::set_minimum_heating_setpoint(float min_heating_setpoint)
{
  uint16_t raw_temp = (int16_t)(min_heating_setpoint * 100.0f);
  this->room_ac_device->SetMinHeatingSetpoint(raw_temp);
}

/***************************************************************************//**
 * Sets the thermostat's absolute maximum heating setpoint value in celsius
 * This is the heating device's physical limit which cannot be changed.
 *
 * @param[in] abs_max_heating_setpoint the absolute maximum value heating can be set to
 ******************************************************************************/
void MatterRoomAC::set_absolute_maximum_heating_setpoint(float abs_max_heating_setpoint)
{
  uint16_t raw_temp = (int16_t)(abs_max_heating_setpoint * 100.0f);
  this->room_ac_device->SetAbsMaxHeatingSetpoint(raw_temp);
}

/***************************************************************************//**
 * Sets the thermostat's maximum heating setpoint value in celsius
 * This limit may be changed by the user to enforce heating policies.
 * Cannot be higher than the absolute maximum value.
 *
 * @param[in] max_heating_setpoint the maximum value heating can be set to
 ******************************************************************************/
void MatterRoomAC::set_maximum_heating_setpoint(float max_heating_setpoint)
{
  uint16_t raw_temp = (int16_t)(max_heating_setpoint * 100.0f);
  this->room_ac_device->SetMaxHeatingSetpoint(raw_temp);
}

/***************************************************************************//**
 * Gets the thermostat's absolute minimum heating setpoint value in celsius
 * This is the heating device's physical limit which cannot be changed.
 *
 * @return the absolute minimum value heating can be set to
 ******************************************************************************/
float MatterRoomAC::get_absolute_minimum_heating_setpoint()
{
  return (float)this->room_ac_device->GetAbsMinHeatingSetpoint() / 100.0f;
}

/***************************************************************************//**
 * Gets the thermostat's minimum heating setpoint value in celsius
 * This limit may be changed by the user to enforce heating policies.
 * Cannot be lower than the absolute minimum value.
 *
 * @return the minimum value heating can be set to
 ******************************************************************************/
float MatterRoomAC::get_minimum_heating_setpoint()
{
  return (float)this->room_ac_device->GetMinHeatingSetpoint() / 100.0f;
}

/***************************************************************************//**
 * Gets the thermostat's absolute maximum heating setpoint value in celsius
 * This is the heating device's physical limit which cannot be changed.
 *
 * @return the absolute maximum value heating can be set to
 ******************************************************************************/
float MatterRoomAC::get_absolute_maximum_heating_setpoint()
{
  return (float)this->room_ac_device->GetAbsMaxHeatingSetpoint() / 100.0f;
}

/***************************************************************************//**
 * Gets the thermostat's maximum heating setpoint value in celsius
 * This limit may be changed by the user to enforce heating policies.
 * Cannot be higher than the absolute maximum value.
 *
 * @return the maximum value heating can be set to
 ******************************************************************************/
float MatterRoomAC::get_maximum_heating_setpoint()
{
  return (float)this->room_ac_device->GetMaxHeatingSetpoint() / 100.0f;
}

/***************************************************************************//**
 * Gets the thermostat's currently set system mode
 *
 * @return the currently set system mode
 ******************************************************************************/
MatterRoomAC::ac_mode_t MatterRoomAC::get_system_mode()
{
  return (ac_mode_t)this->room_ac_device->GetSystemMode();
}

/***************************************************************************//**
 * Sets the thermostat's system mode
 *
 * @param[in] system_mode the system mode to be set
 ******************************************************************************/
void MatterRoomAC::set_system_mode(MatterRoomAC::ac_mode_t system_mode)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->room_ac_device->SetSystemMode((uint8_t)system_mode);
  PlatformMgr().UnlockChipStack();
}
/***********************************************************************************/
