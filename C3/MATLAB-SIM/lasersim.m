function lasersim(commandedAzEl)

%Persistent Global Data
global laserPosition;
global laserOrigin;
global laserHeight;
global laserMaxSpeed;
global laserAzError;
global laserElError;
global laserAzBias;
global laserElBias;
global playingFieldOrigin;
global dTime;

%TRANSFORM LASER POSITION INTO LASER SPACE
%Translate Global X/Y into Relative X/Y Coordinate System (relative to Laser Origin)
relativeLaserX = laserPosition(1) - laserOrigin(1);
relativeLaserY = laserPosition(2) - laserOrigin(2);

%Rotate Relative X/Y Into Local X/Y Coordinate System
theta = atan2(playingFieldOrigin(1)-laserOrigin(1),playingFieldOrigin(2)-laserOrigin(2))*180/pi;
localLaserX = relativeLaserX*cosd(theta) - relativeLaserY*sind(theta);
localLaserY = relativeLaserX*sind(theta) + relativeLaserY*cosd(theta);
localLaserR = (relativeLaserX^2+relativeLaserY^2)^(1/2);

%Transform Local X/Y into Local Az/El Coordinate System
localLaserAz = atan2(localLaserX,localLaserY)*180/pi;
localLaserEl = atan2(localLaserR,laserHeight)*180/pi;

%Move Laser in Commanded Direction (In Relative Az/El)
localLaserAz = localLaserAz + max(min(commandedAzEl(1),laserMaxSpeed),-laserMaxSpeed)*dTime;
localLaserEl = localLaserEl + max(min(commandedAzEl(2),laserMaxSpeed),-laserMaxSpeed)*dTime;

%Add Bias & Error Into Laser Movements
localLaserAz = localLaserAz + laserAzBias*max(min(commandedAzEl(1),laserMaxSpeed),-laserMaxSpeed)*dTime + randn*laserAzError;
localLaserEl = localLaserEl + laserElBias*max(min(commandedAzEl(2),laserMaxSpeed),-laserMaxSpeed)*dTime + randn*laserElError;

%TRANSFORM LASER POSITION BACK INTO GLOBAL SPACE
%Transform Local Az/El into Local X/Y Coordinate System
localLaserR = tand(localLaserEl)*laserHeight;
localLaserX = sind(localLaserAz)*localLaserR;
localLaserY = cosd(localLaserAz)*localLaserR;

%Rotate Local X/Y into Relative X/Y Coordinate System
relativeLaserX = localLaserX*cosd(-theta) - localLaserY*sind(-theta);
relativeLaserY = localLaserX*sind(-theta) + localLaserY*cosd(-theta);

%Translate Relative X/Y into Global X/Y Coordinate System
laserPosition(1) = relativeLaserX + laserOrigin(1);
laserPosition(2) = relativeLaserY + laserOrigin(2);