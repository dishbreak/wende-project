
function [DTI, TravelRange, TTI, commandedAzEl, cameraPipX, cameraPipY, cameraTheta] = c3sim(cameraRoverPosition,   ...
                                                                                             cameraLaserPosition,   ...
                                                                                             tracker_mode,          ...
                                                                                             playingFieldRadius,    ...
                                                                                             time,                  ...
                                                                                             intilizeFilter,        ...
                                                                                             c3Filters)

%Persistent Global Data
global cameraOrigin;
global laserOrigin;
global playingFieldOrigin;
global globalCommands;
global laserHeight;
global updatePeriod;
global gridlockError;
global laserOriginError;

%% static function variables
persistent travelRange;
persistent cameraRoverPositionPrev;
persistent timeHold;

%% CALCULATE PREDICTED INTERCEPT POINT (PIP)
if (tracker_mode == 1) %Alpha/Beta Filter
    [cameraPipX, cameraPipY] = C3_AlphaBetaFilter(cameraRoverPosition, updatePeriod,intilizeFilter,c3Filters);
else                   %Kalman Filter (1 State)
    [cameraPipX, cameraPipY] = C3_KalmanFilter(cameraRoverPosition, updatePeriod,5,intilizeFilter,c3Filters);
end

%% VALIDATE POSTION IN PLAYING FIELD
%Calculate Rover Range From Center (and return if it is within playing field)
[DTI] = EuclideanDistance(cameraRoverPosition,[0,0]);

%% If Outside Playing Field Point Laser
if DTI <= playingFieldRadius
    commandedAzEl           = [0,0];
    globalCommands          = [0,0];
    cameraTheta             = 0;
    travelRange             = 0;
    cameraRoverPositionPrev = cameraRoverPosition;
    timeHold                = time;
    TTI                     = 0;
else
    %Calculate the travel Range past playing field
    travelRange             = travelRange + EuclideanDistance(cameraRoverPositionPrev,cameraRoverPosition);
    cameraRoverPositionPrev = cameraRoverPosition;
    
    %
    TTI                     = time - timeHold;
    
    %FIND CAMERA TO LASER TRANSLATION AND ROTATION PARAMETERS
    %Camera to Laser Rotation Angle
    cameraTheta = atan2(playingFieldOrigin(1)-cameraOrigin(1), playingFieldOrigin(2)-cameraOrigin(2))*180/pi + gridlockError;
    laserTheta  = atan2(playingFieldOrigin(1)-laserOrigin(1) , playingFieldOrigin(2)-laserOrigin(2)) *180/pi;
    theta       = laserTheta - cameraTheta;

    %Find Laser Origin in Camera Space
    laserOriginCameraSpace(1) = laserOrigin(1)*cosd(cameraTheta) - laserOrigin(2)*sind(cameraTheta);
    laserOriginCameraSpace(2) = laserOrigin(1)*sind(cameraTheta) + laserOrigin(2)*cosd(cameraTheta);

    %Camera to Laser Translation Vector (Camera Coordinate system has origin in playing field center)
    cameraToLaserX = laserOriginCameraSpace(1) - playingFieldOrigin(1) + laserOriginError;
    cameraToLaserY = laserOriginCameraSpace(2) - playingFieldOrigin(2) + laserOriginError;

    %TRANSFORM LASER & ROVER POSITIONS INTO LASER SPACE
    %Translate Camera X/Y into Relative X/Y Coordinate System (relative to Laser Origin)
    relativeLaserX = cameraLaserPosition(1) - cameraToLaserX;
    relativeLaserY = cameraLaserPosition(2) - cameraToLaserY;
    relativePipX = cameraPipX - cameraToLaserX;
    relativePipY = cameraPipY - cameraToLaserY;

    %Rotate Relative X/Y Into Local X/Y Coordinate System (Y axis goes from laser origin to playing field origin)
    localLaserX = relativeLaserX*cosd(theta) - relativeLaserY*sind(theta);
    localLaserY = relativeLaserX*sind(theta) + relativeLaserY*cosd(theta);
    localLaserR = (relativeLaserX^2+relativeLaserY^2)^(1/2);
    localPipX = relativePipX*cosd(theta) - relativePipY*sind(theta);
    localPipY = relativePipX*sind(theta) + relativePipY*cosd(theta);
    localPipR = (relativePipX^2+relativePipY^2)^(1/2);

    %Transform Local X/Y into Local Az/El Coordinate System
    localLaserAz = atan2(localLaserX,localLaserY)*180/pi;
    localLaserEl = atan2(localLaserR,laserHeight)*180/pi;
    localPipAz = atan2(localPipX,localPipY)*180/pi;
    localPipEl = atan2(localPipR,laserHeight)*180/pi;

    %CALCULATE GUIDANCE COMMANDS
    %Calculate Az/El Delta
    dAz = localPipAz - localLaserAz;
    dEl = localPipEl - localLaserEl;

    %Calculate Guidance Commands (in laser's Az/El deg/s)
    commandedAzEl(1) = dAz/updatePeriod;
    commandedAzEl(2) = dEl/updatePeriod;
end

TravelRange = travelRange;