#ifndef POS_CAN_SENSOR_H
#define POS_CAN_SENSOR_H

#include <can_sensors/can_sensors.h>

class PositionCanSensor : public CanSensor
{
public:
        //our robot lives in a right handed coordinate system where +Z is up
        //+X is normal to the surface of the tag
        //+Y is "to the left" if you are the tag and facing towards the field
		//X          = distance from tag in m to camera along axis normal to tag surface
		//Y          = distance from tag in m to camera along axis parallel to the ground
		//theta      = positive theta is towards +Y, ranges from -PI to PI
		//servoTheta = positive theta is towards +Y, ranges from -PI to PI

	PositionCanSensor(int cID, char* interface);

    enum class SendStatus  // this requires cpp11
    {
      WRITE_FAILED,
      WRITE_SUCCESS
    };

	SendStatus sendData(float x, float y, float theta, float servoTheta);
};
#endif
