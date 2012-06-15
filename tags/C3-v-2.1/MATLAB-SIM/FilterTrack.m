classdef FilterTrack
    %FilterTrack Summary of this class goes here
    %   Detailed explanation goes here    
    properties
        %Kalman Filter
        c3RoverFilter.X             = zeros(6, 1);
        c3RoverFilter.P             = 9*ones(6,6);
        c3RoverFilter.X(1,1)        = cameraRoverPosition(1);
        c3RoverFilter.X(2,1)        = cameraRoverPosition(2);
    end
    
    methods
    end    
end

