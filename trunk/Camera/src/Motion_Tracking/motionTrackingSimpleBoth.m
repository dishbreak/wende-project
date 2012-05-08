clc; clear all; close all;
addpath(genpath('../../'))
clear
close all
warning off all
clc
color = 1;
numframes=200;
brightness = 80;
contrast = 10;
exposure = -10;
saturation = 200;
sharpness = 50;
whitebalance =2500;
vid = videoinput('winvideo', 1, 'YUY2_1280x720');
src = getselectedsource(vid);
set(vid,'ReturnedColorSpace','rgb')
set(vid, 'FramesPerTrigger', 1);
set(vid, 'TriggerRepeat', Inf);
set(src,'WhiteBalanceMode','manual')
set(src,'ExposureMode','manual')
set(src,'BacklightCompensation','off')
set(src,'Brightness',brightness)
set(src,'Contrast',contrast)
set(src,'Exposure',exposure)
set(src,'Saturation',saturation)
set(src,'sharpness',sharpness)
set(src,'WhiteBalance',whitebalance)
start(vid)

try
    %     pause(2)
    for i = 1:10
        samples(:,:,:,i) = getdata(vid);
        
    end
    %     bg = uint8(mean(samples,4));
    %     std = uint8(std(double(samples),1,4));
    %     std = rgb2gray(std);
    bg = getdata(vid);
    %tol = 6.*std;
    tol = uint8(20*ones(720,1280));
    i=1;
    bg=wiener2(rgb2gray(bg),[5 5]);
    disp('Start Moving')
    tic
    pause(3)
    while(vid.FramesAcquired<=numframes)
        im1 = getdata(vid);
        im=wiener2(rgb2gray(im1),[5 5]);
        %se = strel('square',5);
        %im = imfill(im,'holes');
        brighterThanBg = (im > bg + tol);
        darkerThanBg = (im < bg - tol);
        Im2 = (brighterThanBg | darkerThanBg);
        Im = Im2;%bwareaopen(Im2,3);
        %imshow(Im)
        obj(:,:,1) = double(im1(:,:,1)).*Im;
        obj(:,:,2) = double(im1(:,:,2)).*Im;
        obj(:,:,3) = double(im1(:,:,3)).*Im;
        obj = uint8(obj);
        target = imsubtract(obj(:,:,2), rgb2gray(obj));
        target = im2bw(target,0.05);
        tarStats = regionprops(target, 'Area','BoundingBox', 'Centroid');
        [~,tarObject]=max([tarStats.Area]);
%         if isempty(tarStats)
%             tarStats = regionprops(obj, 'Area','BoundingBox', 'Centroid');
%             [~,tarObject]=max([tarStats.Area]);
%         end
try
        obj(round(tarStats(tarObject).Centroid(2)-20):round(tarStats(tarObject).Centroid(2)+20),round(tarStats(tarObject).Centroid(1)-20):round(tarStats(tarObject).Centroid(1)+20)) = zeros(41,41);
end
        %laser = imsubtract(rgb2gray(obj),obj(:,:,1));
        laser = im2bw(obj(:,:,1),0.30);
        lasStats = regionprops(laser, 'Area','BoundingBox', 'Centroid');
        [~,lasObject]=max([lasStats.Area]);
%         if isempty(lasStats)
%             lasStats = regionprops(obj, 'Area','BoundingBox', 'Centroid');
%             [~,lasObject]=max([lasStats.Area]);
%         end
        imshow(im1)
        
        hold on
        %This is a loop to bound the red objects in a rectangular box.
        if ~isempty(tarStats)
            tarC = tarStats(tarObject).Centroid;
            plot(tarC(1),tarC(2), '-m+')
            a=text(tarC(1)+15,tarC(2), strcat('X: ', num2str(round(tarC(1))), '    Y: ', num2str(round(tarC(2)))));
            set(a, 'FontName', 'Arial', 'FontWeight', 'bold', 'FontSize', 12, 'Color', 'green');
        end
        if ~isempty(lasStats)
            lasC = lasStats(lasObject).Centroid;
            plot(lasC(1),lasC(2), '-m+')
            b=text(lasC(1)+15,lasC(2), strcat('X: ', num2str(round(lasC(1))), '    Y: ', num2str(round(lasC(2)))));
            set(b, 'FontName', 'Arial', 'FontWeight', 'bold', 'FontSize', 12, 'Color', 'red');
        end
        hold off
        
        mData(i)=getframe;
        i=i+1;
        flushdata(vid)
    end
    t=toc;
    procTime=(i)/t;
    stop(vid)
    disp(['Total Time = ',num2str(floor((procTime)))])
catch err
    flushdata(vid)
    stop(vid)
    rethrow(err)
end

