%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%            WENDE CAMERA OPERATION              %%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
warning off;clc;close all;clear;clear java; javaaddpath ./
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Set Class Structure        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CameraSystem = CSS;
TestMode = false;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Configure            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CameraSystem = CameraSystem.getCameraInfo;
CameraSystem.ConfigCamera
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Get Frame Stats      %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
disp('Calculating Frame Stats...')
n=1;
CameraSystem.StartCamera
%Capture 10 frames and extract the last one as the background to account fo
%camera drivers settling.
while(n~=11)
    CameraSystem = GetFrame(CameraSystem);
    CameraSystem.FlushCameraData
    n=n+1;
end
bg = CameraSystem.FR;
CameraSystem = getBackgroundData(CameraSystem,bg);
disp('DONE!')
%CameraSystem.StopCamera
CameraSystem.FlushCameraData
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Calibrate            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
disp('Calibrating Playing Field...')
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
disp('DONE!')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Run Detection        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
n=1;
%CameraSystem.StartCamera
disp('Run Detection Proccess...')
while(n~=2)
    T_1 = tic;
    CameraSystem = GetFrame(CameraSystem);
    T_1_Out(n) = toc(T_1);
    T_2 = tic;
    CameraSystem = TrackMotion(CameraSystem);
    T_2_Out(n) = toc(T_2);
    T_3 = tic;
    CameraSystem = ProcessLaserFrame(CameraSystem);
    T_3_Out(n) = toc(T_3);

    if TestMode
        imshow(CameraSystem.FR)
        hold on
        CameraSystem.PlotDetection(CameraSystem.bb,CameraSystem.col,CameraSystem.row,'r','o')
    end
    T_4 = tic;
    CameraSystem = ProcessRoverFrame(CameraSystem);
    T_4_Out(n) = toc(T_4);
    CameraSystem = findRoverSep(CameraSystem);
    if TestMode
        CameraSystem.PlotDetection(CameraSystem.bbObj1,CameraSystem.colObj1,CameraSystem.rowObj1,'g','o')
        CameraSystem.PlotDetection(CameraSystem.bbObj2,CameraSystem.colObj2,CameraSystem.rowObj2,'g','o')
        hold off
    end
    n = n + 1;
   CameraSystem.FlushCameraData
end
disp('DONE!')
disp('-----------------------------------')
disp(['Total Frames : ' num2str(n)])
tTime = mean(T_1_Out)+mean(T_2_Out)+mean(T_3_Out)+mean(T_4_Out);
disp(['Get Frame Time: ' num2str(mean(T_1_Out)) ' ' num2str((mean(T_1_Out)/tTime)*100) '%'])
disp(['Track Motion Time: ' num2str(mean(T_2_Out)) ' ' num2str((mean(T_2_Out)/tTime)*100) '%'])
disp(['Process Laser Frame Time: ' num2str(mean(T_3_Out)) ' ' num2str((mean(T_3_Out)/tTime)*100) '%'])
disp(['Process Rover Frame Time: ' num2str(mean(T_4_Out)) ' ' num2str((mean(T_4_Out)/tTime)*100) '%'])
disp(['Total Time : ' num2str(tTime)])
CameraSystem.StopCamera
