%% Funntion: C3_KalmanFilter
%   Purpose: CALCULATE PREDICTED INTERCEPT POINT (PIP) using 
%            an Kalman filter
%
%    Inputs: Camera Rover Position --- 1x2 matrix [x,y]
%            Update Period         --- Time since last camera update
%%
function [cameraPipX, cameraPipY] = C3_KalmanFilter(cameraRoverPosition, updatePeriod, processNoise, intilizeFilter, c3Filters)

% Local Static Variables 
persistent c3RoverFilter

% if the static variables is not initilized
if isempty(c3RoverFilter) || intilizeFilter
    %FILTERING
    c3RoverFilter.X             = zeros(6, 1);
    c3RoverFilter.P             = 9*ones(6,6);
    c3RoverFilter.X(1,1)        = cameraRoverPosition(1);
    c3RoverFilter.X(2,1)        = cameraRoverPosition(2);
end

% Constant Matrices
I   = eye(6);                   % Identity Matrix
R   = c3Filters.processNoise .* eye(2);   % [m/s^2] Process Noise 

% Initialize state transition matrix
F=[ 1 0 updatePeriod 0            0            0;...            % [x  ]
    0 1 0            updatePeriod 0            0;...            % [y  ]
    0 0 1            0            updatePeriod 0;...            % [Vx]
    0 0 0            1            0            updatePeriod;... % [Vy]
    0 0 0            0            1            0 ;...           % [Ax]
    0 0 0            0            0            1 ];             % [Ay]

% Initialize measurement matrix
H = [ 1 0 0 0 0 0; 0 1 0 0 0 0 ];    

% Prediction
c3RoverFilter.X     = F * c3RoverFilter.X;
c3RoverFilter.P     = F * c3RoverFilter.P * F' + I;
S                   = H * c3RoverFilter.P' * H' + R; 
B                   = H * c3RoverFilter.P';                     % Bias
c3RoverFilter.pP11  = sqrt(c3RoverFilter.P(1,1));
c3RoverFilter.pP22  = sqrt(c3RoverFilter.P(2,2));

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
c3RoverFilter.lag = min(((K(1,1)/(K(2,1)/updatePeriod))-0.5),100); % filter lag

% Record Covariance
c3RoverFilter.P11 = sqrt(c3RoverFilter.P(1,1));
c3RoverFilter.P22 = sqrt(c3RoverFilter.P(2,2));

%CALCULATE PREDICTED INTERCEPT POINT (PIP)
cameraPipX = c3RoverFilter.xk(1) + c3RoverFilter.vk(1)*updatePeriod;
cameraPipY = c3RoverFilter.xk(2) + c3RoverFilter.vk(2)*updatePeriod;
