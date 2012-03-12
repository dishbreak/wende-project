clc; clear all; close all;
addpath(genpath('../../'))
clear
close all
warning off all
clc
videoInfo = CSS;
videoInfo = videoInfo.getCameraInfo;
videoInfo.ConfigCamera;
videoInfo.StartCamera;
vid = videoInfo.vid;
try
    pause(2)
    for i = 1:10
        samples(:,:,:,i) = getdata(vid);
        pause(0.05)
    end
    bg = uint8(mean(samples,4));
    std = uint8(std(double(samples),1,4));
    std = rgb2gray(std);
    tol = 6.*std;
    numframes=300;
    i=1;
    bg=wiener2(rgb2gray(bg),[5 5]);
    disp('Start Moving')
    tic
    
    while(videoInfo.vid.FramesAcquired<=numframes)
        im = getdata(vid);
        im=wiener2(rgb2gray(im),[5 5]);
        %se = strel('square',5);
        %im = imfill(im,'holes');
        brighterThanBg = (im > bg + tol);
        darkerThanBg = (im < bg - tol);
        Im = (brighterThanBg | darkerThanBg);
        Im = bwareaopen(Im,700);
        imshow(Im)
        %mData(i)=getframe;
        i=i+1;
        videoInfo.FlushCameraData
    end
    t=toc;
    procTime=(i)/t;
    videoInfo.StopCamera;
    disp(['Total Time = ',num2str(floor((procTime)))])
catch err
    videoInfo.StopCamera;
    rethrow(err)
end