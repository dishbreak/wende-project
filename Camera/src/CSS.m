classdef CSS
    properties
        Diameter = .06;                 % 60 cm
        LaserColor = 1;                 % [R G B] => [1 2 3]
        BoundColor = 3;                 % [R G B] => [1 2 3]
        RoverColor = 2;                 % [R G B] => [1 2 3]
        numFrames = 2;                  % Frames to Capture
        vid = [];
        %vid = videoinput(camera_name, camera_id, format);
        %vid = videoinput('winvideo', 1, 'YUY2_1280x720');
        FR = [];
        row = [];
        col = [];
        transformMtx = [];
        Width = [];
        Height =  [];
        timeStamp = [];
        CalibPointsMtx = [];
        TestPointsMtx = [];
        currFrame = [];
    end
    
    methods
        function obj = getCameraInfo(obj)
            a = imaqhwinfo;
            camera_name = char(a.InstalledAdaptors(end));
            camera_info = imaqhwinfo(camera_name);
            deviceName = camera_info.DeviceInfo.DeviceName;
            if strcmp('Microsoft LifeCam Studio',deviceName)
                obj.vid = videoinput('winvideo', 1, 'YUY2_1280x720');
            else
                camera_id = camera_info.DeviceInfo.DeviceID(end);
                resolution = char(camera_info.DeviceInfo.SupportedFormats(end));
                obj.vid = videoinput(camera_name, camera_id, resolution);
                obj.vid.FrameGrabInterval = 1;
            end
        end
        function ConfigCamera(obj)
            set(obj.vid,'ReturnedColorSpace','rgb')
            set(obj.vid, 'FramesPerTrigger', 1);
            set(obj.vid, 'TriggerRepeat', Inf);
        end
        function StartCamera(obj)
            start(obj.vid)
        end
        function StopCamera(obj)
            stop(obj.vid)
            flushdata(obj.vid)
        end
        function obj = GetTestFrame(obj)
            obj.StartCamera
            obj.FR = getdata(obj.vid,1);
            obj.StopCamera
        end
        function obj = CalibratePlayingField(obj)
            obj.CalibPointsMtx = obj.ProccesPlayingField(obj.FR);
            obj.TestPointsMtx = obj.ProccesPlayingField(imread('CalibImg.png'));
            obj.transformMtx = cp2tform(obj.CalibPointsMtx,obj.TestPointsMtx,'projective');
            curr_info = imfinfo('CalibImg.png');
            obj.Width = curr_info.Width;
            obj.Height = curr_info.Height;
        end
        function outputPoints = ProccesPlayingField(obj,frame)
            playingField = imabsdiff(frame(:,:,1),frame(:,:,3));
            detectionPlane = (playingField <= (0.4 * double(max(playingField(:)))));
            edgePlane = edge(detectionPlane,'canny',.98);
            SouthPoint = max(find(sum(edgePlane,2) ~= 0));
            NorthPoint = min(find(sum(edgePlane,2) ~= 0));
            NSMid = median(find(sum(edgePlane,1) ~= 0));
            WestPoint = min(find(sum(edgePlane,1) ~= 0));
            EastPoint =  max(find(sum(edgePlane,1) ~= 0));
            EWMid = median(find(sum(edgePlane,2) ~= 0));
            outputPoints = [NSMid NorthPoint;EastPoint EWMid ;NSMid SouthPoint;WestPoint EWMid ];
        end
        function obj = ProcessLaserFrame(obj)
            [cFrame,obj.timeStamp] = getdata(obj.vid, 1);
            obj.currFrame = imtransform(cFrame, obj.transformMtx,'XData',[1 obj.Width],'YData',[1 obj.Height]);
            colorPlane = obj.currFrame(:, :, obj.LaserColor);
            detectionPlane = (colorPlane >= (0.95 * double(max(colorPlane(:)))));
            props = regionprops(logical(detectionPlane), 'Area', 'Centroid');
            area = [props.Area];
            [num_pixels, index] = max(area);
            threshold = 5;
            if (num_pixels > threshold)
                obj.row = props(index).Centroid(2);
                obj.col = props(index).Centroid(1);
            else
                obj.row = NaN;
                obj.col = NaN;
            end
        end
        function PlotDetection(obj)
            hold on
            plot(obj.col, obj.row, 'yo')
            plot([1 size(obj.currFrame, 2)], [obj.row obj.row], 'r-')
            plot([obj.col obj.col], [1 size(obj.currFrame, 1)], 'r-')
            hold off;
        end
    end
end
