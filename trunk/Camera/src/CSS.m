classdef CSS
    properties
        outerDiameter = .705;           % 70.5 cm
        LaserColor = 1;                 % [R G B] => [1 2 3]
        BoundColor = 3;                 % [R G B] => [1 2 3]
        RoverColor = 2;                 % [R G B] => [1 2 3]
        vid = [];
        FR = [];
        row = [];
        col = [];
        transformMtx = [];
        timeStamp = [];
        CalibPointsMtx = [];
        CalibPointsMtxTrans = [];
        TestPointsMtx = [];
        currFrame = [];
        rowObj1 = [];
        colObj1 = [];
        rowObj2 = [];
        colObj2 = [];
        transCoordLaser =[];
        transCoordRover1 =[];
        transCoordRover2 =[];
        roverSep = [];
        rmSmallAreaPlanePF = [];
        outputPoints = [];
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
        end
        function FlushCameraData(obj)
            flushdata(obj.vid) 
        end
        function obj = GetFrame(obj)
            [obj.FR,obj.timeStamp] = getdata(obj.vid,1);       
        end
        function obj = CalibratePlayingField(obj)
            obj = obj.ProccesPlayingField(imread('CalibImg.png'));
            obj.TestPointsMtx = obj.outputPoints;
            obj = obj.ProccesPlayingField(obj.FR);
            obj.CalibPointsMtx = obj.outputPoints;
            obj.transformMtx = cp2tform(obj.CalibPointsMtx,obj.TestPointsMtx,'projective');
            for i = 1:length(obj.CalibPointsMtx)
                obj.CalibPointsMtxTrans = [obj.CalibPointsMtxTrans ; tformfwd(obj.transformMtx,obj.CalibPointsMtx(i,1),obj.CalibPointsMtx(i,2))];
            end
        end
        function obj = ProccesPlayingField(obj,frame)
            %playingField = imabsdiff(frame(:,:,2),frame(:,:,3));
            conFrame = rgb2ycbcr(frame);
            playingField = im2bw(conFrame(:,:,2),graythresh(im2double(conFrame(:,:,2))));
            obj.rmSmallAreaPlanePF = bwareaopen(playingField,300);
            pfFilt = medfilt2(obj.rmSmallAreaPlanePF, [3 3]);
            edgePlane = edge(pfFilt,'canny',.99);
            SouthPoint = max(find(sum(edgePlane,2) ~= 0));
            NorthPoint = min(find(sum(edgePlane,2) ~= 0));
            NSMid = median(find(sum(edgePlane,1) ~= 0));
            WestPoint = min(find(sum(edgePlane,1) ~= 0));
            EastPoint =  max(find(sum(edgePlane,1) ~= 0));
            EWMid = median(find(sum(edgePlane,2) ~= 0));
            obj.outputPoints = [NSMid NorthPoint;EastPoint EWMid ;NSMid SouthPoint;WestPoint EWMid ];
        end
        function obj = ProcessLaserFrame(obj)
            obj.currFrame = obj.FR;
            colorPlane = rgb2ycbcr(obj.FR);
            %colorPlane = obj.currFrame(:, :, obj.LaserColor);
            detectionPlane = im2bw(colorPlane,.6);
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
            obj.transCoordLaser = [obj.col,obj.row,1]*obj.transformMtx.tdata.T;
        end
        function obj = ProcessRoverFrame(obj)
            obj.currFrame =  rgb2ycbcr(obj.FR);
            compFram = imcomplement(obj.currFrame(:,:,3));
            detectionPlane = im2bw(compFram,graythresh(im2double(compFram)));
            subPF = imabsdiff(detectionPlane,obj.rmSmallAreaPlanePF);
            rmSmallAreaPlane = bwareaopen(subPF,1100);
            %pfFilt = medfilt2(rmSmallAreaPlane, [8 8]);
            props = regionprops(logical(rmSmallAreaPlane), 'Area', 'Centroid');
            [area, index] = sort([props.Area],'descend');
            try
                obj.rowObj1 = props(index(1)).Centroid(2);
                obj.colObj1 = props(index(1)).Centroid(1);
                obj.transCoordRover1 = [obj.colObj1,obj.rowObj1,1]*obj.transformMtx.tdata.T;
            catch
                obj.rowObj1 = NaN;
                obj.colObj1 = NaN;
                obj.transCoordRover1 = [obj.colObj1,obj.rowObj1];
            end
            try
                obj.rowObj2 = props(index(2)).Centroid(2);
                obj.colObj2 = props(index(2)).Centroid(1);
                obj.transCoordRover2 = [obj.colObj2,obj.rowObj2,1]*obj.transformMtx.tdata.T;
            catch
                obj.rowObj2 = NaN;
                obj.colObj2 = NaN;
                obj.transCoordRover2 = [obj.colObj2,obj.rowObj2];
            end
        end
        function PlotDetection(obj,pCol,pRow,Color)
            hold on
            plot(pCol, pRow, [Color 'o'])
            plot([1 size(obj.currFrame, 2)], [pRow pRow], [Color '-'])
            plot([pCol pCol], [1 size(obj.currFrame, 1)], [Color '-'])
        end
        function obj = findRoverSep(obj)
            pixPerMeter = (obj.CalibPointsMtxTrans(2,1)-obj.CalibPointsMtxTrans(4,1))/obj.outerDiameter;
            pixDistance = sqrt((obj.transCoordRover1(1)-obj.transCoordRover2(1))^2+(obj.transCoordRover1(2)-obj.transCoordRover2(2))^2);
            obj.roverSep = pixDistance/pixPerMeter;
        end
    end
end
