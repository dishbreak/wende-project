function [commandedAzEl] = c3sim_kalman_perfect(cameraRoverPosition,cameraLaserPosition)


%Persistent Global Data
global cameraOrigin;
global laserOrigin;
global playingFieldOrigin;
global globalCommands;
global playingFieldRadius;
global laserHeight;
global updateRate;
global gridlockError;
global laserOriginError;
global c3RoverFilter;
global cameraMajorError; %m (stdev)
global processNoise;

filtering =1;


%***************ADD ANY FILTERING & GUIDANCE ALGORITHMS HERE***************
%FILTER DATA

% Constant Matrices
%I   =   eye(2);
%F   =   [1 updateRate; 0 1];          %   [mat]       2x2 System matrix
%G   =   [updateRate^2/2; updateRate]; %   [mat]       2x1 Input gain matrix
%H   =   [1 0];                        %   [mat]       1x2 Measurement matrix
if filtering
% Constant Matrices
I   =   eye(6);   % Identity Matrix
R   = processNoise .* eye(2); % [m/s^2] Process Noise 
% What is the Max Accel the rover can pull within one update

% Initialize state transition matrix
F=[ 1 0 updateRate 0          0          0;...          % [x  ]
    0 1 0          updateRate 0          0;...          % [y  ]
    0 0 1          0          updateRate 0;...          % [Vx]
    0 0 0          1          0          updateRate;... % [Vy]
    0 0 0          0          1          0 ;...         % [Ax]
    0 0 0          0          0          1 ];           % [Ay]

H = [ 1 0 0 0 0 0; 0 1 0 0 0 0 ];    % Initialize measurement matrix

%persistent X P                % Initial state conditions
if c3RoverFilter.initilized == 0 %if isempty(X)
    c3RoverFilter.X = zeros(6, 1);             % x_est=[x,y,Vx,Vy,Ax,Ay]'
    c3RoverFilter.P = 9*ones(6,6);%3*cameraMajorError*eye(6, 6);
    c3RoverFilter.X(1,1)=cameraRoverPosition(1);
    c3RoverFilter.X(2,1)=cameraRoverPosition(2);
    c3RoverFilter.initialized = 1;    
end

% Prediction
c3RoverFilter.X = F * c3RoverFilter.X;
c3RoverFilter.P = F * c3RoverFilter.P * F' + I;
S = H * c3RoverFilter.P' * H' + R; 
B = H * c3RoverFilter.P'; % Bias
c3RoverFilter.pP11 = sqrt(c3RoverFilter.P(1,1));
c3RoverFilter.pP22 = sqrt(c3RoverFilter.P(2,2));

% Save Residuals
c3RoverFilter.rk = [cameraRoverPosition(1)-c3RoverFilter.X(1,1),cameraRoverPosition(2)-c3RoverFilter.X(2,1)];

% Determine Gain Matrix
K = (S \ B)';

% Estimated state and covariance
c3RoverFilter.X = c3RoverFilter.X + K * (cameraRoverPosition' - H * c3RoverFilter.X); % z= 2x1
c3RoverFilter.P = c3RoverFilter.P - K * H * c3RoverFilter.P;

% Compute the estimated measurements
y = H * c3RoverFilter.X;

c3RoverFilter.xk = [c3RoverFilter.X(1,1),c3RoverFilter.X(2,1)];
c3RoverFilter.vk = [c3RoverFilter.X(3,1),c3RoverFilter.X(4,1)];
c3RoverFilter.ak = [c3RoverFilter.X(5,1),c3RoverFilter.X(6,1)];
c3RoverFilter.a = K(1,1);
c3RoverFilter.b = K(3,1);
c3RoverFilter.g = K(5,1);
c3RoverFilter.K = K;

% Filter Performance
  % Record Lag
  c3RoverFilter.lag = min(((K(1,1)/(K(2,1)/updateRate))-0.5),100); % filter lag

  % Record Covariance
  c3RoverFilter.P11 = sqrt(c3RoverFilter.P(1,1));
  c3RoverFilter.P22 = sqrt(c3RoverFilter.P(2,2));

% if c3RoverFilter.initialized == 0
%     c3RoverFilter.xk_1 = cameraRoverPosition;
%     c3RoverFilter.vk_1 = [0,0];
%     c3RoverFilter.a = 0.30;
%     c3RoverFilter.b = 0.15;
%     c3RoverFilter.initialized = 1;
% end
% 
% c3RoverFilter.xk = c3RoverFilter.xk_1 + (c3RoverFilter.vk_1*updateRate);
% c3RoverFilter.vk = c3RoverFilter.vk_1;
% Q = G*(W^2)*G';
% c3RoverFilter.P = F*c3RoverFilter.P*F' + Q;
% c3RoverFilter.rk = cameraRoverPosition - c3RoverFilter.xk;
% c3RoverFilter.xk = c3RoverFilter.xk + c3RoverFilter.a*c3RoverFilter.rk;
% c3RoverFilter.vk = c3RoverFilter.vk + (c3RoverFilter.b * c3RoverFilter.rk)/updateRate;
% c3RoverFilter.xk_1 = c3RoverFilter.xk;
% c3RoverFilter.vk_1 = c3RoverFilter.vk;
else % no filtering case
    if (c3RoverFilter.initialized==0)
        c3RoverFilter.xk = cameraRoverPosition;
        c3RoverFilter.xk_1 = c3RoverFilter.xk;
        c3RoverFilter.vk = [0,0];
        c3RoverFilter.initialized=1;
    else
        c3RoverFilter.xk = cameraRoverPosition;
        c3RoverFilter.vk = (c3RoverFilter.xk-c3RoverFilter.xk_1) /updateRate;
        c3RoverFilter.xk_1 = c3RoverFilter.xk;
    end
end

%Calculate Rover Range From Center (and return if it is within playing field)
cameraRoverRange = (c3RoverFilter.xk(1)^2 + c3RoverFilter.xk(2)^2)^(1/2);
if cameraRoverRange <= playingFieldRadius
    commandedAzEl = [0,0];
    globalCommands = [0,0];
   
    return
end 


%CALCULATE PREDICTED INTERCEPT POINT (PIP)
cameraPipX = c3RoverFilter.xk(1) + c3RoverFilter.vk(1)*updateRate;
cameraPipY = c3RoverFilter.xk(2) + c3RoverFilter.vk(2)*updateRate;
%**************************************************************************


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
commandedAzEl(1) = dAz/updateRate;
commandedAzEl(2) = dEl/updateRate;

%*******Calculate Commands in Global (For Display Purposes Only)*******
globalCameraPipX = cameraPipX*cosd(-cameraTheta) - cameraPipY*sind(-cameraTheta);
globalCameraPipY = cameraPipX*sind(-cameraTheta) + cameraPipY*cosd(-cameraTheta);
globalCameraLaserPosition(1) = cameraLaserPosition(1)*cosd(-cameraTheta) - cameraLaserPosition(2)*sind(-cameraTheta);
globalCameraLaserPosition(2) = cameraLaserPosition(1)*sind(-cameraTheta) + cameraLaserPosition(2)*cosd(-cameraTheta);
globalCommands(1) = globalCameraPipX - globalCameraLaserPosition(1);
globalCommands(2) = globalCameraPipY - globalCameraLaserPosition(2);
%*************************************************************************