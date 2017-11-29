#include <can_sensors/position_can_sensor.h>

PositionCanSensor::PositionCanSensor(int cID, char* interface)
:CanSensor(cID, interface)
{}

PositionCanSensor::SendStatus PositionCanSensor::sendData(float x, float y, float theta, float servoTheta)
{
    //turn posData struct into format acceptable for sending
	//this was agreed up during the Consortum Required for API Permanance

	uint16_t xdata = (uint16_t)(x*1000); //get X in mm
	uint16_t ydata = (uint16_t)(y*1000); //get Y in mm
	int16_t thetadata = (int16_t)(theta*1000); //get th in mRad
    int16_t servothetadata = (int16_t)(servoTheta*1000); //get sTh in mRad

	uint8_t data[8];

	data[0] = xdata >> 8;
	data[1] = xdata & 0xFF;
	data[2] = ydata >> 8;
	data[3] = ydata & 0xFF;
	data[4] = thetadata >> 8;
	data[5] = thetadata & 0xFF;
	data[6] = servothetadata >> 8;
	data[7] = servothetadata & 0xFF;

    if (this->canSend(data, 8) == CanWriteStatus::CAN_WRITE_SUCCESS) return PositionCanSensor::SendStatus::WRITE_SUCCESS;
    else return PositionCanSensor::SendStatus::WRITE_FAILED;

}
