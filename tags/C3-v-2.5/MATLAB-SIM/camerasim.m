function [cameraRoverPosition,cameraLaserPosition] = camerasim(roverMode)

%Persistent Global Data
global cameraOrigin;
global playingFieldOrigin;
global roverPosition;
global laserPosition;
global cameraMajorError;
global cameraMinorError;

%Perform Coordinate Rotation
theta = atan2(playingFieldOrigin(1)-cameraOrigin(1),playingFieldOrigin(2)-cameraOrigin(2))*180/pi;
[rr,cc]=size(roverPosition);
cameraRoverPosition = zeros(rr,cc);
for ll =1:1:rr
    cameraRoverPosition(ll,1) = roverPosition(ll,1)*cosd(theta) - roverPosition(ll,2)*sind(theta) + randn*cameraMajorError;
    cameraRoverPosition(ll,2) = roverPosition(ll,1)*sind(theta) + roverPosition(ll,2)*cosd(theta) + randn*cameraMinorError;
end
cameraLaserPosition(1) = laserPosition(1)*cosd(theta) - laserPosition(2)*sind(theta) + randn*cameraMajorError;
cameraLaserPosition(2) = laserPosition(1)*sind(theta) + laserPosition(2)*cosd(theta) + randn*cameraMinorError;
