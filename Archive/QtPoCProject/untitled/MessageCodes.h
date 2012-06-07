#ifndef MESSAGECODES_H
#define MESSAGECODES_H

class MessageCodes
{
public:
    enum StatusCode {
        LaserCommsExcellent = 601,
        LaserCommsGood = 602,
        LaserCommsPoor = 603,
        LaserCommsOffline = 101,
        CameraCommsExcellent = 611,
        CameraCommsGood = 612,
        CameraCommsPoor = 613,
        CameraCommsOffline = 111,
        LaserOnline = 621,
        LaserOffline = 622,
        LaserUnknown = 21,
        CameraOnline = 633,
        CameraOffline = 131,
        CameraUnknown = 41,
        RoverAcquired = 631,
        RoverNotAcquired = 731,
        LaserActive = 641,
        LaserNotActive = 741,
        wendeOnline = 1000,
        wendeOffline = 1
    };
};
#endif // MESSAGECODES_H
