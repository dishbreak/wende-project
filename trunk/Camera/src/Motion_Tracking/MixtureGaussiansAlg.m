% This m-file implements the mixture of Gaussians algorithm for background
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
    videoInfo.StopCamera;
    bg = uint8(mean(samples,4));
    % -----------------------  frame size variables -----------------------
    
    fr = uint8(mean(samples,4));           % read in 1st frame as background frame
    fr_bw = rgb2gray(fr);     % convert background to greyscale
    fr_size = size(fr);
    width = fr_size(2);
    height = fr_size(1);
    fg = zeros(height, width);
    bg_bw = zeros(height, width);
    
    % --------------------- mog variables -----------------------------------
    
    C = 3;                                  % number of gaussian components (typically 3-5)
    M = 3;                                  % number of background components
    D = 2.5;                                % positive deviation threshold
    alpha = 0.01;                           % learning rate (between 0 and 1) (from paper 0.01)
    thresh = 0.25;                          % foreground threshold (0.25 or 0.75 in paper)
    sd_init = 6;                            % initial standard deviation (for new components) var = 36 in paper
    w = zeros(height,width,C);              % initialize weights array
    mean = zeros(height,width,C);           % pixel means
    sd = zeros(height,width,C);             % pixel standard deviations
    u_diff = zeros(height,width,C);         % difference of each pixel from mean
    p = alpha/(1/C);                        % initial p variable (used to update mean and sd)
    rank = zeros(1,C);                      % rank of components (w/sd)
    
    
    % --------------------- initialize component means and weights -----------
    
    pixel_depth = 8;                        % 8-bit resolution
    pixel_range = 2^pixel_depth -1;         % pixel range (# of possible values)
    
    for i=1:height
        for j=1:width
            for k=1:C
                
                mean(i,j,k) = rand*pixel_range;     % means random (0-255)
                w(i,j,k) = 1/C;                     % weights uniformly dist
                sd(i,j,k) = sd_init;                % initialize to sd_init
                
            end
        end
    end
    
    videoInfo.StartCamera;
    %--------------------- process frames -----------------------------------
    
    while(videoInfo.vid.FramesAcquired<=numframes)
        
        fr =  getdata(vid);    % read in frame
        fr_bw = rgb2gray(fr);       % convert frame to grayscale
        
        % calculate difference of pixel values from mean
        for m=1:C
            u_diff(:,:,m) = abs(double(fr_bw) - double(mean(:,:,m)));
        end
        
        % update gaussian components for each pixel
        for i=1:height
            for j=1:width
                
                match = 0;
                for k=1:C
                    if (abs(u_diff(i,j,k)) <= D*sd(i,j,k))       % pixel matches component
                        
                        match = 1;                          % variable to signal component match
                        
                        % update weights, mean, sd, p
                        w(i,j,k) = (1-alpha)*w(i,j,k) + alpha;
                        p = alpha/w(i,j,k);
                        mean(i,j,k) = (1-p)*mean(i,j,k) + p*double(fr_bw(i,j));
                        sd(i,j,k) =   sqrt((1-p)*(sd(i,j,k)^2) + p*((double(fr_bw(i,j)) - mean(i,j,k)))^2);
                    else                                    % pixel doesn't match component
                        w(i,j,k) = (1-alpha)*w(i,j,k);      % weight slighly decreases
                        
                    end
                end
                
                w(i,j,:) = w(i,j,:)./sum(w(i,j,:));
                
                bg_bw(i,j)=0;
                for k=1:C
                    bg_bw(i,j) = bg_bw(i,j)+ mean(i,j,k)*w(i,j,k);
                end
                
                % if no components match, create new component
                if (match == 0)
                    [min_w, min_w_index] = min(w(i,j,:));
                    mean(i,j,min_w_index) = double(fr_bw(i,j));
                    sd(i,j,min_w_index) = sd_init;
                end
                
                rank = w(i,j,:)./sd(i,j,:);             % calculate component rank
                rank_ind = [1:1:C];
                
                % sort rank values
                for k=2:C
                    for m=1:(k-1)
                        
                        if (rank(:,:,k) > rank(:,:,m))
                            % swap max values
                            rank_temp = rank(:,:,m);
                            rank(:,:,m) = rank(:,:,k);
                            rank(:,:,k) = rank_temp;
                            
                            % swap max index values
                            rank_ind_temp = rank_ind(m);
                            rank_ind(m) = rank_ind(k);
                            rank_ind(k) = rank_ind_temp;
                            
                        end
                    end
                end
                
                % calculate foreground
                match = 0;
                k=1;
                
                fg(i,j) = 0;
                while ((match == 0)&&(k<=M))
                    
                    if (w(i,j,rank_ind(k)) >= thresh)
                        if (abs(u_diff(i,j,rank_ind(k))) <= D*sd(i,j,rank_ind(k)))
                            fg(i,j) = 0;
                            match = 1;
                        else
                            fg(i,j) = fr_bw(i,j);
                        end
                    end
                    k = k+1;
                end
            end
        end
        
        %figure(1),subplot(3,1,1),imshow(fr)
        %subplot(3,1,2),imshow(uint8(bg_bw))
        %subplot(3,1,3),
        imshow(uint8(fg))
        
        videoInfo.FlushCameraData
    end
    videoInfo.StopCamera;
catch err
    videoInfo.StopCamera;
    rethrow(err)
end