function [commandedAzEl] = c3sim(cameraRoverPosition,cameraLaserPosition)


%Persistent Global Data
global cameraOrigin;
global laserOrigin;
global playingFieldOrigin;
global globalCommands;
global playingFieldRadius;
global laserHeight;
global updatePeriod;
global gridlockError;
global laserOriginError;
global c3RoverFilter;


%***************ADD ANY FILTERING & GUIDANCE ALGORITHMS HERE***************
%CALCULATE PREDICTED INTERCEPT POINT (PIP)
if c3RoverFilter.initialized == 0
    c3RoverFilter.xk_1 = cameraRoverPosition;
    c3RoverFilter.vk_1 = [0,0];
    c3RoverFilter.a = .5;
    c3RoverFilter.b = .3;
    c3RoverFilter.initialized = 1;
end
c3RoverFilter.xk = c3RoverFilter.xk_1 + (c3RoverFilter.vk_1*updatePeriod);
c3RoverFilter.vk = c3RoverFilter.vk_1;
c3RoverFilter.rk = cameraRoverPosition - c3RoverFilter.xk;
c3RoverFilter.xk = c3RoverFilter.xk + c3RoverFilter.a*c3RoverFilter.rk;
c3RoverFilter.vk = c3RoverFilter.vk + (c3RoverFilter.b * c3RoverFilter.rk)/updatePeriod;
c3RoverFilter.xk_1 = c3RoverFilter.xk;
c3RoverFilter.vk_1 = c3RoverFilter.vk;



cameraPipX = c3RoverFilter.xk(1) + c3RoverFilter.vk(1)*updatePeriod;
cameraPipY = c3RoverFilter.xk(2) + c3RoverFilter.vk(2)*updatePeriod;
%**************************************************************************

%Calculate Rover Range From Center (and return if it is within playing field)
cameraRoverRange = (cameraRoverPosition(1)^2 + cameraRoverPosition(2)^2)^(1/2);
if cameraRoverRange <= playingFieldRadius
    commandedAzEl = [0,0];
    globalCommands = [0,0];
    return
end


%FIND CAMERA TO LASER TRANSLATION AND ROTATION PARAMETERS
%Camera to Laser Rotation Angle
cameraTheta = atan2(playingFieldOrigin(1)-cameraOrigin(1),playingFieldOrigin(2)-cameraOrigin(2))*180/pi + gridlockError;
laserTheta = atan2(playingFieldOrigin(1)-laserOrigin(1),playingFieldOrigin(2)-laserOrigin(2))*180/pi;
theta = laserTheta - cameraTheta;

%Find Laser Origin in Camera Space
laserOriginCameraSpace(1) = laserOrigin(1)*cosd(cameraTheta) - laserOrigin(2)*sind(cameraTheta);
laserOriginCameraSpace(2) = laserOrigin(1)*sind(cameraTheta) + laserOrigin(2)*cosd(cameraTheta);

%Camera to Laser Translation Vector (Camera Coordinate system has origin in playing field center)
cameraToLaserX = laserOriginCameraSpace(1) - playingFieldOrigin(1) + laserOriginError;
cameraToLaserY = laserOriginCameraSpace(2) - playingFieldOrigin(2) + laserOriginError;

%Alternative Method (Plug in a theta [camera to laser angle] and range [laser origin to playing field origin]
% display('In bearing, the "+Y" axis is "0," and positive angles go clockwise');
% theta = input('Input Theta Degrees (Bearing between Camera & Laser): ');
% range = input('Input Range (Distance from Laser Origin to Center of Playing Field): ');
% cameraToLaserX = -range*sind(theta);
% cameraToLaserY = -range*cosd(theta);


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

%*******Calculate Commands in Global (For Display Purposes Only)*******
globalCameraPipX = cameraPipX*cosd(-cameraTheta) - cameraPipY*sind(-cameraTheta);
globalCameraPipY = cameraPipX*sind(-cameraTheta) + cameraPipY*cosd(-cameraTheta);
globalCameraLaserPosition(1) = cameraLaserPosition(1)*cosd(-cameraTheta) - cameraLaserPosition(2)*sind(-cameraTheta);
globalCameraLaserPosition(2) = cameraLaserPosition(1)*sind(-cameraTheta) + cameraLaserPosition(2)*cosd(-cameraTheta);
globalCommands(1) = globalCameraPipX - globalCameraLaserPosition(1);
globalCommands(2) = globalCameraPipY - globalCameraLaserPosition(2);
%*************************************************************************