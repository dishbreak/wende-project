function wtVid(f,name,frameRate,nTimes)
% Use this function to create a .avi video file from a matlab movie.
% f = matlab movie file
% name = name of file passed in as char (i.e. 'coolVideo')
% frameRate = speed at which you want to replay
% nTimes = number of times you want to loop the video for the .avi file
vidObj = VideoWriter([name,'.avi']);
vidObj.FrameRate = frameRate;
open(vidObj);
for i = 1:nTimes
    for k = 1:length(f)
        writeVideo(vidObj,f(k).cdata);
    end
end
close(vidObj);