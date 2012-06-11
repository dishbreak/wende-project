%%
% Sets up the Environment
clear; clc; close all;
cd c:\java\wendeIO\matlab;
javaaddpath ..\bin;

%%
clear; clc; close all;
CameraSystem = CSS;
TestMode = false;


net = SendMsg
net.initNet(0)
net.initNet(1)
net.initNet(2)

CameraSystem = CameraSystem.getCameraInfo;
CameraSystem.ConfigCamera
CameraSystem.StartCamera
n = 0;
while(n~=11)
    CameraSystem = GetFrame(CameraSystem);
    CameraSystem.FlushCameraData
    scaledImage = imresize(CameraSystem.FR,[180,300]);
    
[h,w,c] = size(scaledImage);
m = 1;
for i = 1:1
    for j = 1:h
        for k = 1:w
            serialArray(m) = scaledImage(j,k,i);
            m = m + 1;
        end
    end
end
    n=n+1;
%setStatus(long time,CameraMsgs.systemStatus sysStat, boolean laserOn, String text)
net.setStatus(100000, net.getStatus , 0, 'Camera IPT Rocks');
net.sendStatus;

%setTracks(long time,	CameraMsgs.systemStatus sysStat, int[][] targets, int[][] lasers, boolean laserOn)
net.setTracks(1001, net.getStatus, [[0,12],[2,200]], [[700,700]], 0);
net.sendTracks;

%setImage(long time, int channels, int sizeX, int sizeY, com.google.protobuf.ByteString serialImage)
net.setImage(1012221, 3, 300, 180, serialArray);
net.sendImage;

end
CameraSystem.StopCamera

% do at end
net.closeNet(0);
net.closeNet(1);
net.closeNet(2);

%%
% Creates a serialized version of FR -- need to run OptCSS.m first
scaledImage = imresize(CameraSystem.FR,[180,300]);
[h,w,c] = size(scaledImage);
m = 1;
for i = 1:c
    for j = 1:h
        for k = 1:w
            serialArray(m) = scaledImage(j,k,i);
            m = m + 1;
        end
    end
end
%%

net = SendMsg
net.initNet(0)
net.initNet(1)
net.initNet(2)

%setStatus(long time,CameraMsgs.systemStatus sysStat, boolean laserOn, String text)
net.setStatus(100000, net.getStatus , 0, 'Matlab Test');
net.sendStatus;

%setTracks(long time,	CameraMsgs.systemStatus sysStat, int[][] targets, int[][] lasers, boolean laserOn)
net.setTracks(1001, net.getStatus, [[0,12],[2,200]], [[700,700]], 0);
net.sendTracks;

%setImage(long time, int channels, int sizeX, int sizeY, com.google.protobuf.ByteString serialImage)
net.setImage(1012221, 3, 300, 180, serialArray);
net.sendImage;


for i = 1:100
net.setTracks(1000+i, net.getStatus, [[i,2*i],[3*i,4*i]] , [[i + 2, 1],[i-12, -200]], mod(i,2));
net.sendTracks;
end

%setStatus(long time,CameraMsgs.systemStatus sysStat, boolean laserOn, String text)
net.setStatus(100000, net.getStatus , 0, 'Matlab Test');
net.sendStatus;

%setTracks(long time,	CameraMsgs.systemStatus sysStat, int[][] targets, int[][] lasers, boolean laserOn)
net.setTracks(1001, net.getStatus, [[0,12],[2,200]], [[700,700]], 0);
net.sendTracks;

%setImage(long time, int channels, int sizeX, int sizeY, com.google.protobuf.ByteString serialImage)
net.setImage(1012221, 3, 300, 180, serialArray);
net.sendImage;

% do at end
net.closeNet(0);
net.closeNet(1);
net.closeNet(2);