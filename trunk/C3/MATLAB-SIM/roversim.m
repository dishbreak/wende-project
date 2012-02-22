function roversim(roverMode)

%Persistent Global Data
global dTime;
global roverPosition;
global roverBearing;

%Mode
if roverMode == 0 || roverMode == 1
    roverSpeed = 0.05;
elseif roverMode == 2
    roverSpeed = 0.2;
end

%Direction
xdir = sind(roverBearing);
ydir = cosd(roverBearing);
roverDirection(1) = xdir/(xdir^2 + ydir^2)^(1/2);
roverDirection(2) = ydir/(xdir^2 + ydir^2)^(1/2);

if roverMode == 0 && (roverPosition(1)^2 + roverPosition(2)^2)^(1/2) > 0.7 + 0.65
    %Do Nothing
else
    roverPosition(1) = roverPosition(1) + roverDirection(1)*roverSpeed*dTime;
    roverPosition(2) = roverPosition(2) + roverDirection(2)*roverSpeed*dTime;
end