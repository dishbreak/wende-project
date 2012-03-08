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
    numframes=200;
    i=1;
    bg=rgb2gray(bg);
    disp('Start Moving')
    tic
    while(videoInfo.vid.FramesAcquired<=numframes)
        im = getdata(vid);
        im=rgb2gray(im);
        %se = strel('square',5);
        %im = imfill(im,'holes');
        brighterThanBg = (im > bg + tol);
        darkerThanBg = (im < bg - tol);
        Im = (brighterThanBg | darkerThanBg);
        Im = bwareaopen(Im,2000);
        imshow(Im)
        %         mFrame(i)=im2frame(uint8(Im),colormap(gray(128)));
        %         i=i+1;
%         hold on
%         stats = regionprops(Im, 'BoundingBox', 'Centroid','Area');
%         [~,idx]=max([stats.Area]);
%         if ~isempty(stats)
%         bb = stats(idx).BoundingBox;
%         bc = stats(idx).Centroid;
%         rectangle('Position',bb,'EdgeColor','r','LineWidth',2)
%         plot(bc(1),bc(2), '-m+')
%         a=text(bc(1)+15,bc(2), strcat('X: ', num2str(round(bc(1))), '    Y: ', num2str(round(bc(2)))));
%         set(a, 'FontName', 'Arial', 'FontWeight', 'bold', 'FontSize', 12, 'Color', 'yellow');
%         end
%         
%         hold off
        mData(i)=getframe;
        i=i+1;
        videoInfo.FlushCameraData
    end
    t=toc;
    procTime=(numframes-10)/t;
    videoInfo.StopCamera;
    disp(['Total Time = ',num2str(floor((procTime)))])
catch err
    videoInfo.StopCamera;
    rethrow(err)
end
