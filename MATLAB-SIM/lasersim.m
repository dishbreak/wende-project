function lasersim(commandedAzEl)

%Persistent Global Data
global laserPosition;
global localLaserVelocity;
global laserOrigin;
global laserHeight;
global laserMaxSpeed;
global laserMinSpeed;
global laserMinStep;
global laserMaxAcceleration;
global laserAzError;
global laserElError;
global laserAzBias;
global laserElBias;
global playingFieldOrigin;
global dTime;
global laserHysteresis;

if isempty(laserHysteresis)
    laserHysteresis = [0 0];
end


%****************TRANSFORM LASER POSITION INTO LASER SPACE*****************
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



%*************************APPLY GUIDANCE COMMANDS**************************
%Calculate Laser Velocity Limits
laserVelocityAzLimits(1) = min(localLaserVelocity(1) + laserMaxAcceleration*dTime,laserMaxSpeed); %Azimuth Velocity Upper Limit
laserVelocityAzLimits(2) = max(localLaserVelocity(1) - laserMaxAcceleration*dTime,-laserMaxSpeed); %Azimuth Velocity Lower Limit
laserVelocityElLimits(1) = min(localLaserVelocity(2) + laserMaxAcceleration*dTime,laserMaxSpeed); %Elevation Velocity Upper Limit
laserVelocityElLimits(2) = max(localLaserVelocity(2) - laserMaxAcceleration*dTime,-laserMaxSpeed); %Elevation Velocity Lower Limit

%Calculate Average Laser Velocity (for this dTime)
initialLaserVelocity = localLaserVelocity;
finalLaserVelocity(1) = max(min(commandedAzEl(1,1),laserVelocityAzLimits(1)),laserVelocityAzLimits(2));
finalLaserVelocity(2) = max(min(commandedAzEl(1,2),laserVelocityElLimits(1)),laserVelocityElLimits(2));
averageLaserVelocity = (initialLaserVelocity + finalLaserVelocity)/2;
localLaserVelocity = finalLaserVelocity;

%Azimuth
if abs(initialLaserVelocity(1)) > laserMinSpeed || abs(finalLaserVelocity(1)) > laserMinSpeed
    %Move Laser in Commanded Direction (In Relative Az)
    localLaserAz = localLaserAz + averageLaserVelocity(1)*dTime;
    %Add Bias & Error Into Laser Az Movements
    localLaserAz = localLaserAz + laserAzBias*averageLaserVelocity(1)*dTime + randn*laserAzError;
    %Reset Laser Friction Hysteresis
    laserHysteresis(1) = 0;
else %Laser Must Move Minimum Step due to Friction Hysteresis
    if abs(laserHysteresis(1) + averageLaserVelocity(1)*dTime) >= laserMinStep
        %Move Laser 1 Minimum Step in Commanded Direction (In Relative Az)
        localLaserAz = localLaserAz + laserMinStep;
        %Add Bias & Error Into Laser Az Movements
        localLaserAz = localLaserAz + laserAzBias*laserMinStep + randn*laserAzError;
        %Reset Laser Friction Hysteresis
        laserHysteresis(1) = 0;
    else %Laser is stuck due to Friction Hysteresis
        laserHysteresis(1) = laserHysteresis(1) + averageLaserVelocity(1)*dTime;
    end
end

%Elevation
if abs(initialLaserVelocity(2)) > laserMinSpeed || abs(finalLaserVelocity(2)) > laserMinSpeed
    %Move Laser in Commanded Direction (In Relative El)
    localLaserEl = localLaserEl + averageLaserVelocity(2)*dTime;
    %Add Bias & Error Into Laser El Movements
    localLaserEl = localLaserEl + laserElBias*averageLaserVelocity(2)*dTime + randn*laserElError;
    %Reset Laser Friction Hysteresis
    laserHysteresis(2) = 0;
else %Laser Must Move Minimum Step due to Friction Hysteresis
    if abs(laserHysteresis(2) + averageLaserVelocity(2)*dTime) >= laserMinStep
        %Move Laser 1 Minimum Step in Commanded Direction (In Relative El)
        localLaserEl = localLaserEl + laserMinStep;
        %Add Bias & Error Into Laser El Movements
        localLaserEl = localLaserEl + laserElBias*laserMinStep + randn*laserElError;
        %Reset Laser Friction Hysteresis
        laserHysteresis(2) = 0;
    else %Laser is stuck due to Friction Hysteresis
        laserHysteresis(2) = laserHysteresis(2) + averageLaserVelocity(2)*dTime;
    end
end



%*************TRANSFORM LASER POSITION BACK INTO GLOBAL SPACE**************
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