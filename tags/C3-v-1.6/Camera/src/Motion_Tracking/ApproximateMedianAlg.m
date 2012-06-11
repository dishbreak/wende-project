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
    thresh = 20;
    bg_bw = double(rgb2gray(bg));     % convert background to grayscale
    
    % ----------------------- set frame size variables -----------------------
    fr_size = size(bg);
    width = fr_size(2);
    height = fr_size(1);
    fg = zeros(height, width);
    
    % --------------------- process frames -----------------------------------
    
    while(videoInfo.vid.FramesAcquired<=numframes)
        
        fr = getdata(vid);
        fr_bw = rgb2gray(fr);       % convert frame to grayscale
        fr_diff = abs(double(fr_bw) - double(bg_bw));  % cast operands as double to avoid negative overflow
        
        for j=1:width                 % if fr_diff > thresh pixel in foreground
            for k=1:height
                
                if ((fr_diff(k,j) > thresh))
                    fg(k,j) = fr_bw(k,j);
                else
                    fg(k,j) = 0;
                end
                
                if (fr_bw(k,j) > bg_bw(k,j))
                    bg_bw(k,j) = bg_bw(k,j) + 1;
                elseif (fr_bw(k,j) < bg_bw(k,j))
                    bg_bw(k,j) = bg_bw(k,j) - 1;
                end
                
            end
        end
        
        %figure(1),subplot(3,1,1),imshow(fr)
        %subplot(3,1,2),imshow(uint8(bg_bw))
        %subplot(3,1,3),imshow(uint8(fg))
        imshow(im2bw(fg))
        videoInfo.FlushCameraData
        
    end
    videoInfo.StopCamera;
catch err
    videoInfo.StopCamera;
    rethrow(err)
end


% movie2avi(M,'approximate_median_background','fps',30);           % save movie as avi
