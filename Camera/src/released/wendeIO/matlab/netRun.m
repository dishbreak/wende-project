%%
% Sets up the Environment
clear; clc; close all;

% Change this to be wherever you have the files at.
cd c:\java\wendeIO\matlab;
javaaddpath ..\bin;

% Create Java Class
net = SendMsg
% Initialize Network Ports ( port based on config file)
net.initNet(0)
net.initNet(1)
net.initNet(2)

%%
% Function Parameter List
% setSystemStatus(int status)
% getSystemStatus() % returns status in correct type for setStatus below
% setStatus(long time,CameraMsgs.systemStatus sysStat, boolean laserOn, String text)
% setTracks(long time,	CameraMsgs.systemStatus sysStat, int[][] targets, int[][] lasers, boolean laserOn)
% setImage(long time, int channels, int sizeX, int sizeY, com.google.protobuf.ByteString serialImage)

totalLoops = 100;
countImg = 0;
countTrk = 0;
countStatus = 0;

% how often to update and send message
statusEvery = 2;
trackEvery = 1;
imageEvery = 5;

delay = 0;
statusString = 'Camera IPT Rocks';
% net image size
height = 180;
width = 300;
tic
for n = 1:1:totalLoops
  image = imread('RoverNLaser.PNG');
  scaledImage = imresize(image,[height,width]);
    
    [h,w,c] = size(scaledImage);
    m = 1;
    for i = 1:h
        for j = 1:w
            for k = c:-1:1
                serialArray(m) = scaledImage(i,j,k);
                m = m + 1;
            end
        end
    end
        net.setSystemStatus(mod(n,6));
    if(mod(n,statusEvery) == 0)
        net.setStatus(100000, net.getSystemStatus , 0, statusString);
        countStatus = countStatus + net.sendStatus;
    end
    
    if(mod(n,trackEvery) == 0)
%         net.setTracks(1001, net.getSystemStatus, [[2000*n/totalLoops,1000*n/totalLoops],[700,-700]], [[700,700]], 0);
        net.setTracks(1001, net.getSystemStatus, [[10*n*sin(n/10),10*n*cos(n/10)],[700,-700]], [[700,700]], 0);
        countTrk = countTrk + net.sendTracks;
    end
    
    if(mod(n,imageEvery) == 0)
        if(mod(n,2))
            net.setImage(1012221, 3, 300, 180, serialArray);
        else
            net.setImage(1012221, 3, 300, 180, 255-serialArray);
        end
        countImg = countImg + net.sendImage;
    end
pause(delay);    
end

time = toc;
fprintf('Sent %d Status Messages at %.3f messages per second\n',countStatus, countStatus/time);
fprintf('Sent %d Track  Messages at %.3f messages per second\n',countTrk, countTrk/time);
fprintf('Sent %d Image  Messages at %.3f messages per second\n',countImg, countImg/time);

%%
% close Network Connections - flaky still.
net.closeNet(0)
net.closeNet(1)
net.closeNet(2)