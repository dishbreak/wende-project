function CameraSystemDriver(evalSection)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%            WENDE CAMERA OPERATION              %%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if 1 == evalSection
warning off;clc;close all;clear;clear java; javaaddpath ./bin
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Set Class Structure        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CameraSystem = CSS; TestMode = true;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Configure Camera           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CameraSystem = CameraSystem.getCameraInfo;
CameraSystem.ConfigCamera
%% Configure Network
elseif 2 == evalSection
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Configure Network          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CameraSystem = CameraSystem.ConfigNetwork;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Open Network Ports         %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CameraSystem.Network.initNet(0); CameraSystem.Network.initNet(1); CameraSystem.Network.initNet(2);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Set System Status          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CameraSystem.Network.setSystemStatus(3); 
%% 
elseif 3==evalSection
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Get Frame Stats            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
disp('Calculating Frame Stats...'); n=1;
CameraSystem.StartCamera
while(n~=11)
    CameraSystem = GetFrame(CameraSystem);
    CameraSystem.FlushCameraData
    n=n+1;
end
CameraSystem = getBackgroundData(CameraSystem,CameraSystem.FR);
disp('DONE!')
CameraSystem.StopCamera
CameraSystem.FlushCameraData
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Calibrate Playing Field  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
disp('Calibrating Playing Field...')
CameraSystem = CalibratePlayingField(CameraSystem);
CameraSystem = pxlsToCM(CameraSystem);
if TestMode
    imshow(CameraSystem.FR); hold on
    for i = 1:length(CameraSystem.CalibPointsMtx)
        plot(CameraSystem.CalibPointsMtx(i,1),CameraSystem.CalibPointsMtx(i,2),'r*','MarkerSize',20)
    end
    figure; imshow('CalibImg.png'); hold on
    for i = 1:length(CameraSystem.CalibPointsMtx)
        plot(CameraSystem.TestPointsMtx(i,1),CameraSystem.TestPointsMtx(i,2),'r*','MarkerSize',20)
    end
    TransImage = imtransform(CameraSystem.FR, CameraSystem.transformMtx);
    figure; imshow(TransImage)
end
disp('DONE!')
%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Detection Proccess         %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
elseif 4 == evalSection
n=1; frameTotal = 300;
%--------------------------------------------
% Set Msg Timers to C3
%--------------------------------------------
SendImageTime = timer('StartFcn','CameraSystem.SendImage','TimerFcn','CameraSystem.SendImage', 'Period', 2,'ExecutionMode','fixedRate');
SendSysTime = timer('StartFcn','CameraSystem.SendSysStatus','TimerFcn','CameraSystem.SendSysStatus', 'Period', .25,'ExecutionMode','fixedRate');
%--------------------------------------------
% Run
%--------------------------------------------
disp('Run Detection Proccess...')
SimulationRun = 0; SimButton = figure('Position', [400 400 40 40],'Toolbar','none','MenuBar','none','NumberTitle','off');
uicontrol('Style', 'pushbutton','String', 'Stop', 'Callback', 'SimulationRun = 1; close(SimButton)' ,'Position', [8 0 100 40]);
CameraSystem.StartCamera
% start(SendImageTime);
start(SendSysTime);
if TestMode
    simDisp = figure;
end
try
    while(SimulationRun==0)
        %--------------------------------------------
        % Camera System Main Functions
        %--------------------------------------------
        T_1 = tic; CameraSystem = GetFrame(CameraSystem); T_1_Out(n) = toc(T_1);
        T_2 = tic; CameraSystem = TrackMotion(CameraSystem); T_2_Out(n) = toc(T_2);
        T_3 = tic; CameraSystem = ProcessLaserFrame(CameraSystem); T_3_Out(n) = toc(T_3);
        T_4 = tic; CameraSystem = ProcessRoverFrame(CameraSystem); T_4_Out(n) = toc(T_4);
        %--------------------------------------------
        % Set and Send Rover/Laser Detection to C3
        %--------------------------------------------
        try
            CameraSystem.Network.setTracks(now, CameraSystem.Network.getSystemStatus,[[CameraSystem.transCoordRover1PPI],[CameraSystem.transCoordRover2PPI]],[CameraSystem.transCoordLaserPPI], CameraSystem.LaserStatus,CameraSystem.RoverFlag1,CameraSystem.RoverFlag2);
            CameraSystem.Network.sendTracks;
        catch
            disp('No Tracks Sent');
        end
        %--------------------------------------------
        % Camera Test Display
        %--------------------------------------------
        if TestMode
            figure(simDisp)
            imshow(CameraSystem.FR)
            hold on
            plot(CameraSystem.col,CameraSystem.row,'ro','MarkerSize',3,'LineWidth',3)
            plot(CameraSystem.colObj1,CameraSystem.rowObj1,'go','MarkerSize',3,'LineWidth',3)
            plot(CameraSystem.colObj2,CameraSystem.rowObj2,'go','MarkerSize',3,'LineWidth',3)
            hold off
            %F(:,:,:,n) = getframe(gcf);
        end
        n = n + 1; CameraSystem.FlushCameraData; drawnow;
    end
    disp('DONE!')
%   stop(SendImageTime);
    stop(SendSysTime);
    CameraSystem.StopCamera
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % Frame Stats                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    disp('-----------------------------------')
    disp(['Total Frames : ' num2str(n)])
    tTime = mean(T_1_Out)+mean(T_2_Out)+mean(T_3_Out)+mean(T_4_Out);
    disp(['Get Frame Time: ' num2str(mean(T_1_Out)) ' ' num2str((mean(T_1_Out)/tTime)*100) '%'])
    disp(['Track Motion Time: ' num2str(mean(T_2_Out)) ' ' num2str((mean(T_2_Out)/tTime)*100) '%'])
    disp(['Process Laser Frame Time: ' num2str(mean(T_3_Out)) ' ' num2str((mean(T_3_Out)/tTime)*100) '%'])
    disp(['Process Rover Frame Time: ' num2str(mean(T_4_Out)) ' ' num2str((mean(T_4_Out)/tTime)*100) '%'])
    disp(['Total Time : ' num2str(tTime)])
    disp('-----------------------------------')
catch camExt
    disp('ERROR!')
    stop(SendImageTime);
    stop(SendSysTime);
    CameraSystem.StopCamera; CameraSystem.FlushCameraData
    throw(camExt)
end
%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Close Network Ports        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
elseif 5 == evalSection
CameraSystem.Network.closeNet(0); CameraSystem.Network.closeNet(1); CameraSystem.Network.closeNet(2);
end
