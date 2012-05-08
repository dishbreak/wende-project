%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%            WENDE CAMERA OPERATION              %%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clc;close all;clear
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Set Class Structure        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CameraSystem = CSS;
TestMode = true;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Configure and Calibrate    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CameraSystem = CameraSystem.getCameraInfo;
CameraSystem.ConfigCamera
CameraSystem.StartCamera
CameraSystem = GetFrame(CameraSystem);
CameraSystem.StopCamera
CameraSystem = CalibratePlayingField(CameraSystem);
if TestMode
    imshow(CameraSystem.FR)
    hold on
    for i = 1:length(CameraSystem.CalibPointsMtx)
        plot(CameraSystem.CalibPointsMtx(i,1),CameraSystem.CalibPointsMtx(i,2),'r*','MarkerSize',20)
    end
    figure
    imshow('CalibImg.png')
    hold on
    for i = 1:length(CameraSystem.CalibPointsMtx)
        plot(CameraSystem.TestPointsMtx(i,1),CameraSystem.TestPointsMtx(i,2),'r*','MarkerSize',20)
    end
    imageSize = get(CameraSystem.vid,'VideoResolution');
    TransImage = imtransform(CameraSystem.FR, CameraSystem.transformMtx,'XData',[1 imageSize(1)],'YData',[1 imageSize(2)]);
    figure
    imshow(TransImage)
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Run Laser Detection        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
n=1;
CameraSystem.StartCamera
tic
while(n~=2)
    CameraSystem = GetFrame(CameraSystem);
    CameraSystem = ProcessLaserFrame(CameraSystem);
    if TestMode
        imshow(CameraSystem.currFrame)
        hold on
        CameraSystem.PlotDetection(CameraSystem.bb,'r')
    end
    CameraSystem = ProcessRoverFrame(CameraSystem);
    CameraSystem = findRoverSep(CameraSystem);
    %testSep(n) = CameraSystem.roverSep;
    if TestMode
        CameraSystem.PlotDetection(CameraSystem.bbObj1,'g')
        CameraSystem.PlotDetection(CameraSystem.bbObj2,'g')
        hold off
        %f(n)=getframe;
    end
    n = n + 1;
   CameraSystem.FlushCameraData
end
toc
CameraSystem.StopCamera




