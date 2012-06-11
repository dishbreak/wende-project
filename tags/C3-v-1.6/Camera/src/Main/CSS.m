classdef CSS
    properties
        outerDiameter = .705;           % 70.5 cm or .58166 m
        LaserColor = 1;                 % [R G B] => [1 2 3]
        BoundColor = 3;                 % [R G B] => [1 2 3]
        RoverColor = 2;                 % [R G B] => [1 2 3]
        brightness = 80;
        contrast = 10;
        exposure = -10;
        saturation = 200;
        sharpness = 50;
        whitebalance =2500;
        vid = [];
        FR = [];
        row = [];
        col = [];
        transformMtx = [];
        timeStamp = [];
        CalibPointsMtx = [];
        CalibPointsMtxTrans = [];
        TestPointsMtx = [];
        rowObj1 = [];
        colObj1 = [];
        rowObj2 = [];
        colObj2 = [];
        transCoordLaser =[];
        transCoordRover1 =[];
        transCoordRover2 =[];
        roverSep = [];
        roverSepNT = [];
        outputPoints = [];
        bgImage = [];
        MTFrame = [];
        RemMT = [];
        laserProps = [];
        targetProps = [];
        centerPoint = [];
    end
    
    methods
        function obj = getCameraInfo(obj)
            a = imaqhwinfo;
            camera_name = char(a.InstalledAdaptors(end));
            camera_info = imaqhwinfo(camera_name);
            deviceName = camera_info.DeviceInfo.DeviceName;
            if strcmp('Microsoft LifeCam Studio',deviceName)
                try
                    obj.vid = videoinput('winvideo', 1, 'YUY2_1280x720');
                catch
                    obj.vid = videoinput('winvideo', 2, 'YUY2_1280x720');
                end
            elseif strcmp('Logitech HD Pro Webcam C920',deviceName)
                try
                    obj.vid = videoinput('winvideo', 1, 'RGB24_1280x720');
                catch
                    obj.vid = videoinput('winvideo', 2, 'RGB24_1280x720');
                end
            else
                camera_id = camera_info.DeviceInfo(1).DeviceID;
                resolution = char(camera_info.DeviceInfo(1).SupportedFormats);
                obj.vid = videoinput(camera_name, camera_id, resolution);
                obj.vid.FrameGrabInterval = 1;
            end
        end
        function ConfigCamera(obj)
            src = getselectedsource(obj.vid);
            set(obj.vid,'ReturnedColorSpace','rgb')
            set(obj.vid, 'FramesPerTrigger', 1);
            set(obj.vid, 'TriggerRepeat', Inf);
%             set(src,'WhiteBalanceMode','manual')
%             set(src,'ExposureMode','manual')
%             set(src,'BacklightCompensation','off')
%             set(src,'Brightness',obj.brightness)
%             set(src,'Contrast',obj.contrast)
%             set(src,'Exposure',obj.exposure)
%             set(src,'Saturation',obj.saturation)
%             set(src,'Sharpness',obj.sharpness)
%             set(src,'WhiteBalance',obj.whitebalance)
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
            obj = obj.ProccesPlayingField(imread('CalibImg.png'),0);
            obj.TestPointsMtx = obj.outputPoints;
            try
                obj = obj.ProccesPlayingField(obj.FR,1);
            catch
                disp('NO CALIBRATION POINTS FOUND!!')
            end
            obj.CalibPointsMtx = obj.outputPoints;
            obj.transformMtx = cp2tform(obj.CalibPointsMtx,obj.TestPointsMtx,'projective');
            for i = 1:length(obj.CalibPointsMtx)
                obj.CalibPointsMtxTrans = [obj.CalibPointsMtxTrans ; tformfwd(obj.transformMtx,obj.CalibPointsMtx(i,1),obj.CalibPointsMtx(i,2))];
            end
        end
         function obj = getBackgroundData(obj,bg)
            obj.bgImage = wiener2(rgb2gray(bg),[5 5]);
         end
        function obj = ProccesPlayingField(obj,frame,mtSave)
            imFrame = rgb2ycbcr(frame);
            bwFrame = im2bw(imFrame(:,:,2),.55);
            bwRemAr = bwareaopen(bwFrame,1000);
            findDisk = strel('disk',1);
            imDisk = imclose(bwRemAr,findDisk);
            imfillDisk = imfill(imDisk,'holes');
            [imBounds,imLev] = bwboundaries(imfillDisk,'noholes');
            statsPF = regionprops(imLev,'Area','Centroid');
            for k = 1:length(imBounds)
                imBoundsObj = imBounds{k};
                deltaObj = diff(imBoundsObj).^2;
                perObj = sum(sqrt(sum(deltaObj,2)));
                areaObj = statsPF(k).Area;
                centPoints(k,:) = statsPF(k).Centroid;
                whichCirc(k) = 4*pi*areaObj/perObj^2;
            end
            [valCirc pCirc] = max(whichCirc);
            circPF = imBounds{pCirc};
            if mtSave == 1
                obj.RemMT = logical(zeros(size(bwFrame)));
                for pts = 1:length(circPF)
                    obj.RemMT(circPF(pts,1),circPF(pts,2)) = true;
                end
                obj.RemMT = imfill(obj.RemMT,'holes');
                obj.centerPoint = centPoints(pCirc,:);
            end
            [Sval SouthPointFind] = max(circPF(:,1));
            [Nval NorthPointFind] = min(circPF(:,1));
            [Eval EastPointFind] = max(circPF(:,2));
            [Wval WestPointFind] = min(circPF(:,2));
            obj.outputPoints = [ fliplr(circPF(NorthPointFind,:)); fliplr(circPF(EastPointFind,:)); fliplr(circPF(SouthPointFind,:)); fliplr(circPF(WestPointFind,:))];
        end
        function obj = TrackMotion(obj)
            grFR  = rgb2gray(obj.FR);
            brighterThanBg = (grFR > obj.bgImage + 20);
            darkerThanBg = (grFR < obj.bgImage - 20); 
            ImThr = uint8((brighterThanBg | darkerThanBg) & obj.RemMT);
%             Im = uint8(bwareaopen(ImThr,50));
            obj.MTFrame = obj.FR.*repmat(ImThr,[1 1 3]);
        end
        function obj = ProcessLaserFrame(obj)
            redCh = obj.MTFrame(:,:,1);
            detectionPlane = im2bw(redCh,.9);
            obj.laserProps = regionprops(logical(detectionPlane), 'Area', 'Centroid');
            area = [obj.laserProps.Area];
            [num_pixels, index] = max(area);
            threshold = 5;
            if (num_pixels > threshold)
                obj.row = obj.laserProps(index).Centroid(2);
                obj.col = obj.laserProps(index).Centroid(1);
            else 
                obj.row = NaN;
                obj.col = NaN;
            end
            try
                obj.transCoordLaser = tformfwd(obj.transformMtx,obj.col,obj.row);
            catch
                obj.transCoordLaser = [obj.row,obj.col];
            end
        end
        function obj = ProcessRoverFrame(obj)
            greenCh = 2*obj.MTFrame(:,:,2)-obj.MTFrame(:,:,1)-obj.MTFrame(:,:,3);
            detectionPlane = im2bw(greenCh,0.12);
            %rmSmallAreaPlane = bwareaopen(detectionPlane,25);
            obj.targetProps = regionprops(detectionPlane, 'Area', 'Centroid');
            [~, index] = sort([obj.targetProps.Area],'descend');     
            if isempty(obj.targetProps)
                obj.targetProps = regionprops(obj.MTFrame,'Area','Centroid');
                [~, index] = max([obj.targetProps.Area]);
            end
            try
                obj.rowObj1 = obj.targetProps(index(1)).Centroid(2);
                obj.colObj1 = obj.targetProps(index(1)).Centroid(1);
                obj.transCoordRover1 = tformfwd(obj.transformMtx,obj.colObj1,obj.rowObj1);
            catch
                obj.rowObj1 = NaN;
                obj.colObj1 = NaN;
                obj.transCoordRover1 = [obj.colObj1,obj.rowObj1];
            end
            try
                obj.rowObj2 = obj.targetProps(index(2)).Centroid(2);
                obj.colObj2 = obj.targetProps(index(2)).Centroid(1);
                obj.transCoordRover2 = tformfwd(obj.transformMtx,obj.colObj2,obj.rowObj2);
            catch
                obj.rowObj2 = NaN;
                obj.colObj2 = NaN;
                obj.transCoordRover2 = [obj.colObj2,obj.rowObj2];
            end
        end
        function PlotDetection(obj,x,y,Color,Marker)
            try
                hold on 
                plot(x, y, [Color Marker],'MarkerSize',5,'LineWidth',5)
            end
        end
        function obj = findRoverSep(obj)
            pixPerMeterNT = sqrt((obj.CalibPointsMtx(2,1)-obj.CalibPointsMtx(4,1))^2+(obj.CalibPointsMtx(2,2)-obj.CalibPointsMtx(2,2))^2)/obj.outerDiameter;
            pixPerMeter = sqrt((obj.CalibPointsMtxTrans(2,1)-obj.CalibPointsMtxTrans(4,1))^2+(obj.CalibPointsMtxTrans(2,2)-obj.CalibPointsMtxTrans(2,2))^2)/obj.outerDiameter;
            pixDistanceNT = sqrt((obj.colObj1-obj.colObj2)^2+(obj.rowObj1-obj.rowObj2)^2);
            pixDistance = sqrt((obj.transCoordRover1(1)-obj.transCoordRover2(1))^2+(obj.transCoordRover1(2)-obj.transCoordRover2(2))^2);
            obj.roverSep = pixDistance/pixPerMeter;
            obj.roverSepNT = pixDistanceNT/pixPerMeterNT;
        end
    end
end
