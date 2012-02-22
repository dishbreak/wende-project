function [cameraRoverPosition,cameraLaserPosition] = camerasim

%Persistent Global Data
global cameraOrigin;
global playingFieldOrigin;
global roverPosition;
global laserPosition;
global cameraMajorError;
global cameraMinorError;

%Perform Coordinate Rotation
theta = atan2(playingFieldOrigin(1)-cameraOrigin(1),playingFieldOrigin(2)-cameraOrigin(2))*180/pi;
cameraRoverPosition(1) = roverPosition(1)*cosd(theta) - roverPosition(2)*sind(theta) + randn*cameraMajorError;
cameraRoverPosition(2) = roverPosition(1)*sind(theta) + roverPosition(2)*cosd(theta) + randn*cameraMinorError;
cameraLaserPosition(1) = laserPosition(1)*cosd(theta) - laserPosition(2)*sind(theta) + randn*cameraMajorError;
cameraLaserPosition(2) = laserPosition(1)*sind(theta) + laserPosition(2)*cosd(theta) + randn*cameraMinorError;
