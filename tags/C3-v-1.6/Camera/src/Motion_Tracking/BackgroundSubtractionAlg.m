% This m-file implements the frame difference algorithm for background
% subtraction.  It may be used free of charge for any purpose (commercial
% or otherwise), as long as the author (Seth Benton) is acknowledged.

clc; clear all; close all;
addpath(genpath('../'))
clear
close all
warning off all
clc
videoInfo = CSS;
videoInfo = videoInfo.getCameraInfo;
videoInfo.ConfigCamera;
videoInfo.StartCamera;
vid = videoInfo.vid;
numframes = 300;
try
    pause(2)
    for i = 1:10
        samples(:,:,:,i) = getdata(vid);
        pause(0.05)
    end
    
    bg = uint8(mean(samples,4));
    thresh = 10;
    bg_bw = double(rgb2gray(bg));     % convert background to grayscale      % convert background to greyscale
    
    
    % ----------------------- set frame size variables -----------------------
    fr_size = size(bg);
    width = fr_size(2);
    height = fr_size(1);
    fg = zeros(height, width);
    
    % --------------------- process frames -----------------------------------
    
    while(videoInfo.vid.FramesAcquired<=numframes)
        
        fr = getdata(vid);       % read in frame
        fr_bw = rgb2gray(fr);       % convert frame to grayscale
        
        fr_diff = abs(double(fr_bw) - double(bg_bw));  % cast operands as double to avoid negative overflow
        
        for j=1:width                 % if fr_diff > thresh pixel in foreground
            for k=1:height
                if ((fr_diff(k,j) > thresh))
                    fg(k,j) = fr_bw(k,j);
                else
                    fg(k,j) = 0;
                end
            end
        end
        
        bg_bw = fr_bw;
        
        %     figure(1),subplot(3,1,1),imshow(fr)
        %     subplot(3,1,2),imshow(fr_bw)
        %     subplot(3,1,3),imshow(uint8(fg))
        %
        %     M(i-1)  = im2frame(uint8(fg),gray);% put frames into movie
        imshow(im2bw(fg));
        videoInfo.FlushCameraData
        
    end
    videoInfo.StopCamera;
catch err
    videoInfo.StopCamera;
    rethrow(err)
end
% movie2avi(M,'frame_difference_output', 'fps', 30);           % save movie as avi

