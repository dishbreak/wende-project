%% Funntion: EuclideanDistance
%   Purpose: Calculates the ecuclidean distance between 2 point in N 
%            dimensions 
%
%            distance = sqrt(sum((Ai-Bi)^2)) for i=1:N
%
%            Note: This function assumes that A and B are have the same 
%                  dimensions
%
%    Inputs: A --- N Dimension Point
%            B --- N Dimension Point
%
%   Outputs: distance --- the eucledian distance between A & B.
%%
function [distance] = EuclideanDistance(A,B)

distance   = sqrt(sum((A-B).^2));
