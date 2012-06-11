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
    %% Background Determination
    %Capture 10 background images (no objects can be in frame);.
    %     pause(2)
    %     for i = 1:10
    %         samples(:,:,:,i) = getdata(vid);
    %         pause(0.05)
    %     end
    %     bg = uint8(mean(samples,4));
    %     std = uint8(std(double(samples),1,4));
    %     std = rgb2gray(std);
    %     tol = 6.*std;
    bg = getframe(vid);
    tol = uint8(15*ones(size(bg)));
    numframes=400;
    i=1;
    %bg=medfilt2(rgb2gray(bg),[5 5]); %with filtering
    bg=wiener2(rgb2gray(bg),[10 10]); %with filtering
    %bg = rgb2gray(bg); %without filtering
    disp('Start Moving')
    tic
    pause(2)
    while(videoInfo.vid.FramesAcquired<=numframes)
        %% Detect Motion in Frame
        im1 = getdata(vid);
        %im = medfilt2(rgb2gray(im1),[5 5]);% with filtering
        im = wiener2(rgb2gray(im1),[10 10]);
        %im = rgb2gray(im1); %without filtering
        brighterThanBg = (im > bg + tol);
        darkerThanBg = (im < bg - tol);
        Im = (brighterThanBg | darkerThanBg);
        Im = bwareaopen(Im,50);
        
        
        %% Extract Object from Color Frame
        obj(:,:,1) = double(im1(:,:,1)).*Im;
        obj(:,:,2) = double(im1(:,:,2)).*Im;
        obj(:,:,3) = double(im1(:,:,3)).*Im;
        obj = uint8(obj);
        blueCh = imsubtract(obj(:,:,3), rgb2gray(obj));
        %blueCh = medfilt2(blueCh, [10 10]);
        %blueCh = wiener2(blueCh, [5 5]);
        % Convert the resulting grayscale image into a binary image.
        blueCh = im2bw(blueCh,0.05);
        blueCh = bwareaopen(blueCh,20);
        stats = regionprops(blueCh, 'Area','BoundingBox', 'Centroid');
        [~,idx]=max([stats.Area]);
        imshow(im1)
        
        hold on
        %This is a loop to bound the red objects in a rectangular box.
        for object = idx
            bb = stats(object).BoundingBox;
            bc = stats(object).Centroid;
            rectangle('Position',bb,'EdgeColor','r','LineWidth',2)
            plot(bc(1),bc(2), '-m+')
            a=text(bc(1)+15,bc(2), strcat('X: ', num2str(round(bc(1))), '    Y: ', num2str(round(bc(2)))));
            set(a, 'FontName', 'Arial', 'FontWeight', 'bold', 'FontSize', 12, 'Color', 'yellow');
        end
        hold off
        
        mData(i)=getframe;
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

