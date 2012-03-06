%% Funntion: C3_AlphaBetaFilter
%   Purpose: CALCULATE PREDICTED INTERCEPT POINT (PIP) using 
%            an alpha beta filter
%            (http://en.wikipedia.org/wiki/Alpha_beta_filter)
%
%    Inputs: Camera Rover Position --- 1x2 matrix [x,y]
%            Update Period         --- Time since last camera update
%
% Note: unlike global persistant is only for the function it is called from
%%
function [cameraPipX, cameraPipY] = C3_AlphaBetaFilter(cameraRoverPosition,updatePeriod,intilizeFilter,c3Filters)

% local static variable
persistent c3RoverFilter;

% if the static variable is not initilized
% ?? should this be moved out to state  --- alpha beta values ?? 
if isempty(c3RoverFilter) || intilizeFilter
    c3RoverFilter.a             = c3Filters.alpha;
    c3RoverFilter.b             = c3Filters.beta;
    
    c3RoverFilter.xk_1          = cameraRoverPosition;
    c3RoverFilter.vk_1          = [0,0];
    c3RoverFilter.xk            = [0,0];
    c3RoverFilter.vk            = [0,0];
    c3RoverFilter.rk            = [0,0];
end

c3RoverFilter.xk   = c3RoverFilter.xk_1  + (c3RoverFilter.vk_1*updatePeriod);
c3RoverFilter.vk   = c3RoverFilter.vk_1;

c3RoverFilter.rk   = cameraRoverPosition -  c3RoverFilter.xk;

c3RoverFilter.xk   = c3RoverFilter.xk    +  c3RoverFilter.a * c3RoverFilter.rk;
c3RoverFilter.vk   = c3RoverFilter.vk    + (c3RoverFilter.b * c3RoverFilter.rk)/updatePeriod;

c3RoverFilter.xk_1 = c3RoverFilter.xk;
c3RoverFilter.vk_1 = c3RoverFilter.vk;

% calculate predicted intercept points
cameraPipX = c3RoverFilter.xk(1) + c3RoverFilter.vk(1)*updatePeriod;
cameraPipY = c3RoverFilter.xk(2) + c3RoverFilter.vk(2)*updatePeriod;
end