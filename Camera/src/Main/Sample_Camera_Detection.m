%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%            WENDE CAMERA OPERATION              %%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clc; close all; clear
warning off all
imaqmem(3e9);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%      Set Class Structure       %%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

CameraSystem = CSS;
CameraSystem = ObjectDetection;
CameraSystem = CoordinateTransform;
CameraSystem = CameraSystem.getCameraInfo;
TestMode = false;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Configure and Calibrate    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CameraSystem.ConfigCamera;
CameraSystem.StartCamera;
pause(1)

for i = 1:300
    %laserLoc = CameraSystem.DetectLaser;
    CameraSystem = CameraSystem.DetectTarget;
     imshow(CameraSystem.tarFR); 
    if ~isempty(CameraSystem.targetPos)
        targetPosition = ConvertToStandard(CameraSystem,CameraSystem.targetPos);
        hold on
        plot(CameraSystem.targetPos(1),CameraSystem.targetPos(2), '-m+','markersize',28)
        a=text(CameraSystem.targetPos(1),CameraSystem.targetPos(2), strcat('X: ', num2str(targetPosition(1)), '    Y: ', num2str(targetPosition(2))));
        set(a, 'FontName', 'Arial', 'FontWeight', 'bold', 'FontSize', 12, 'Color', 'yellow');
        hold off
    end
    flushdata(CameraSystem.vid);
end
CameraSystem.StopCamera;
