%%
% Sets up the Environment
clear; clc; close all;
cd c:\java\wendeIO\matlab;
javaaddpath ..\bin;

% Create Java Class
net = SendMsg
% Initialize Network Ports ( port based on config file)
net.initNet(0)
net.initNet(1)
net.initNet(2)


% Function Parameter List
% setStatus(int status)
% getStatus() % returns status in correct type for setStatus below
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

delay = 0.0;
statusString = 'Camera IPT Rocks';
% net image size
height = 180;
width = 300;
tic
for n = 0:1:totalLoops
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
        net.setStatus(mod(n,6));
    if(mod(n,statusEvery) == 0)
        net.setStatus(100000, net.getStatus , 0, statusString);
        countStatus = countStatus + net.sendStatus;
    end
    
    if(mod(n,trackEvery) == 0)
        net.setTracks(1001, net.getStatus, [[2000*n/totalLoops,1000*n/totalLoops]], [[700,700]], 0);
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

countStatus
countTrk
countImg
toc

%%
% close Network Connections - flaky still.
net.closeNet(0);
net.closeNet(1);
net.closeNet(2);