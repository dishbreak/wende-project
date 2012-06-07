classdef C3Simulator
    %C3Simulator Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        travelRange         = 0;            % distance travel since crossing out of playing field
        cameraRoverPosition = zeros(1,2);   % [x,y] of the rover as seen by the camera
        timeHold            = 0;            % Time of crossing out of playing field
        tracker_mode        = 1;            % tracker mode (1 = alpha beta, 2 = Kalman)
        intilizedFilter     = 0;            % Set so that filter is not initilized
    end
    
    methods
        % Constructor
        function [obj] = C3Simulator(tracker_mode)            
            obj.tracker_mode        = tracker_mode;     % tracker mode (1 = alpha beta, 2 = Kalman)
            obj.travelRange         = 0;                % distance travel since crossing out of playing field
            obj.cameraRoverPosition = zeros(1,2);       % [x,y] of the rover as seen by the camera
            obj.timeHold            = 0;                % Time of crossing out of playing field
            obj.tracker_mode        = 1;                % Default the tracker to alpha beta
            obj.intilizedFilter     = 0;                % Set so that filter is not initilized
        end
        
        function [cameraPipX,cameraPipY] = Filter(obj,cameraRoverPosition, updatePeriod)
            if (obj.tracker_mode == 1) %Alpha/Beta Filter
                [cameraPipX, cameraPipY] = C3_AlphaBetaFilter(cameraRoverPosition, updatePeriod);
            else                   %Kalman Filter (1 State)
                [cameraPipX, cameraPipY] = C3_KalmanFilter(cameraRoverPosition, updatePeriod,5);
            end
        end
        
        %Filter input
        function [c3] = testC3(obj)
            obj.cameraRoverPosition =  obj.cameraRoverPosition+1
            
            disp('test');
        end
        
    end
    
end

