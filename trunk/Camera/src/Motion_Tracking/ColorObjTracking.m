%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Motion Tracker 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clc;close all;clear
warning off
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Set Class Structure        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
MotionTracker = CSS;
MTools = MTracker;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Configure and Calibrate    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
MotionTracker = MotionTracker.getCameraInfo;
MotionTracker.ConfigCamera
MotionTracker.StartCamera
pause(1)
MotionTracker = GetFrame(MotionTracker);
MotionTracker.FlushCameraData
MotionTracker.StopCamera
diffImage = double(MotionTracker.FR);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Run Sim                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
n=1;
MotionTracker.StartCamera
tic
while(n~=101)
    MotionTracker = GetFrame(MotionTracker);
    currFrame = double(MotionTracker.FR);
   % MotionTracker.timeStamp
    % Difference Image (Euclidian distance in RGB space)
    dif = sqrt(sum((diffImage-currFrame).^2,3));
    % Compute Difference Image Histogram
    [h, hc] = hist(MTools.makelinear(dif), 256);
    % Perform Thresholding
    Tr = hc(MTools.RosinThreshold(h));
    MTools.shownormimage(dif >= Tr);
    MotionTracker.FlushCameraData
    %f(n)=getframe;
    n = n + 1;
end
toc
MotionTracker.StopCamera



