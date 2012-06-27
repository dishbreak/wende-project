%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Camera Network Code
% Run the first block to setup envronment and create the Java Objects. -
% Need to run C3 Network Arbitor pointed to this computer's IP address for
% network connection to work.

% Run the second block to send messages to C3 - make changes to timing
% messages etc.

% run the 3rd Block to close the network connection from the Camera End. -
% This part still needs a bit of work between Camera and C3.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% Sets up the Environment
clear; clc; close all;

% Change this to be wherever you have this files at.
% cd c:\java\wendeIO\matlab;
cd 'C:\Documents and Settings\jford7\My Documents\ELDP\TDC\2011-2012\wende-project\Camera\src\released\wendeIO\matlab';
javaaddpath ..\bin;

% Create Java Class
net = SendMsg
% Initialize Network Ports ( port based on config file)
netFlagStat = net.initNet(0)
netFlagTrack = net.initNet(1)
netFlagImage = net.initNet(2)

%%
% Function Parameter List
% setSystemStatus(int status)
% getSystemStatus() % returns status in correct type for setStatus below
% setStatus(long time,CameraMsgs.systemStatus sysStat, boolean laserOn, String text)
% setTracks(long time,  CameraMsgs.systemStatus sysStat, int[][] targets, int[][] lasers, boolean laserOn)
% setImage(long time, int channels, int sizeX, int sizeY, com.google.protobuf.ByteString serialImage)

totalLoops = 200;
countImg = 0;
countTrk = 0;
countStatus = 0;

% how often to update and send message
statusEvery = 1;
trackEvery = 1;
imageEvery = 5;

delay = 0.0;
statusString = 'Camera IPT Rocks';
% net image size
height = 180;
width = 300;
tic
for n = 1:1:totalLoops
  if(mod(n,2))
      image = imread('RoverNLaser.PNG');
  else
      image = imread('CalibImg2.png');
  end
    scaledImage = imresize(image,[height,width]);
   
    [h,w,c] = size(scaledImage);
    m = 1;
    for i = 1:h
        for j = 1:w
            %for k = c:-1:1
            for k = c:-1:1
                serialArray(m) = scaledImage(i,j,k);
                m = m + 1;
            end
        end
    end
        net.setSystemStatus(3);
    if(mod(n,statusEvery) == 0 && netFlagStat)
        net.setStatus(100000, net.getSystemStatus , 0, statusString);
        netFlagStat = net.sendStatus;
        countStatus = countStatus + netFlagStat;
    end

    switch(mod(n,5))
        case(0)
            tracks = [0,0];
        case(1)
            tracks = [0,1000];
        case(2)
            tracks = [1000,0];
        case(3)
            tracks = [0,-1000];
        case(4)
            tracks = [-1000,0];
    end
    
    if(mod(n,trackEvery) == 0 && netFlagTrack)
%         net.setTracks(1001, net.getSystemStatus, [[2000*n/totalLoops,1000*n/totalLoops],[700,-700]], [[700,700]], 0);
%         net.setTracks(1001, net.getSystemStatus, [[1000*sin(n/10),1000*cos(n/10)],[700,-700]], [[700,700]], 1,1,0);
        net.setTracks(1001, net.getSystemStatus, [tracks(1),tracks(2)], [[700,700]], 1,1,0);
        
        netFlagTrack = net.sendTracks
        countTrk = countTrk + netFlagTrack;
    end
%    
%     if(mod(n,imageEvery) == 0 && netFlagImage)
%         net.setImage(1012221, 3, 300, 180, serialArray);
%         netFlagImage = net.sendImage;
%         countImg = countImg + netFlagImage;
%     end
pause(delay);    
end

if(~netFlagStat)
    net.closeNet(0);
end
if(~netFlagTrack)
    net.closeNet(1);
end
if(~netFlagImage)
    net.closeNet(2);
end

time = toc;
fprintf('Sent %d Status Messages at %.3f messages per second\n',countStatus, countStatus/time);
fprintf('Sent %d Track  Messages at %.3f messages per second\n',countTrk, countTrk/time);
fprintf('Sent %d Image  Messages at %.3f messages per second\n',countImg, countImg/time);

%%
% close Network Connections
net.closeNet(0)
net.closeNet(1)
net.closeNet(2)

