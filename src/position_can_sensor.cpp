#include <can_sensors/position_can_sensor.h>

PositionCanSensor::PositionCanSensor(int cID, char* interface)
:CanSensor(cID, interface)
{}

PositionCanSensor::SendStatus PositionCanSensor::sendData(float x, float y, float theta, float servoTheta)
{
    //turn posData struct into format acceptable for sending
	//this was agreed up during the Consortum Required for API Permanance

	int16_t xdata = (int16_t)(x*1000); //get X in mm
	int16_t ydata = (int16_t)(y*1000); //get Y in mm
	int16_t thetadata = (int16_t)(theta*1000); //get th in mRad
    int16_t servothetadata = (int16_t)(servoTheta*1000); //get sTh in mRad

	uint8_t data[8];

	data[0] = (uint8_t)(xdata >> 8);
	data[1] = (uint8_t)(xdata & 0xFF);
	data[2] = (uint8_t)(ydata >> 8);
	data[3] = (uint8_t)(ydata & 0xFF);
	data[4] = (uint8_t)(thetadata >> 8);
	data[5] = (uint8_t)(thetadata & 0xFF);
	data[6] = (uint8_t)(servothetadata >> 8);
	data[7] = (uint8_t)(servothetadata & 0xFF);

    if (this->canSend(data, 8) == CanWriteStatus::CAN_WRITE_SUCCESS) return PositionCanSensor::SendStatus::WRITE_SUCCESS;
    else return PositionCanSensor::SendStatus::WRITE_FAILED;

}
