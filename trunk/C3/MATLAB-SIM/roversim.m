function roversim(roverMode)

%Persistent Global Data
global dTime;
global roverPosition;
global roverBearing;

%Speed
if roverMode == 0 || roverMode == 1
    roverSpeed = 0.05;
elseif roverMode == 2
    roverSpeed = 0.2;
elseif roverMode == 3
    roverSpeed = .05;
    roverRadSpeed = 0.01;
    disp(['Radial Speed = ' num2str(roverRadSpeed*100) ' cm/s']);
end

%disp(['Speed = ' num2str(roverSpeed*100) ' cm/s']);

%Direction
xdir = sind(roverBearing);
ydir = cosd(roverBearing);
roverDirection(1) = xdir/(xdir^2 + ydir^2)^(1/2);
roverDirection(2) = ydir/(xdir^2 + ydir^2)^(1/2);

if roverMode == 0 && (roverPosition(1)^2 + roverPosition(2)^2)^(1/2) > 0.7 + 0.65
    %Do Nothing
elseif roverMode == 0 || roverMode == 1 || roverMode == 2
    roverPosition(1) = roverPosition(1) + roverDirection(1)*roverSpeed*dTime;
    roverPosition(2) = roverPosition(2) + roverDirection(2)*roverSpeed*dTime;
else %roverMode == 3
    %Convert X/Y to Polar Coordinates
    roverPolar(1) = (roverPosition(1)^2 + roverPosition(2)^2)^(1/2);
    roverPolar(2) = atan2(roverPosition(1),roverPosition(2))*180/pi;
    if roverPolar(1) < 0.7
        roverPolar(1) = 0.7;
        roverPolar(2) = roverBearing;
    end
    %Move Rover in Polar Coordinates
    roverPolar(1) = roverPolar(1) + roverRadSpeed*dTime;
    roverPolar(2) = roverPolar(2) + (roverSpeed/roverPolar(1))*(180/pi)*dTime;
    
    %Convert Polar Back to X/Y Coordinates
    roverPosition(1) = roverPolar(1)*sind(roverPolar(2));
    roverPosition(2) = roverPolar(1)*cosd(roverPolar(2));
end