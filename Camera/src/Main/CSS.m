classdef CSS
    properties
        outerDiameter = .705;           % 70.5 cm or .58166 m
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
        rowObj1 = [];
        colObj1 = [];
        rowObj2 = [];
        colObj2 = [];
        transCoordLaser =[];
        transCoordRover1 =[];
        transCoordRover2 =[];
        roverSep = [];
        roverSepNT = [];
        rmSmallAreaPlanePF = [];
        outputPoints = [];
        tolImage = [];
        bgImage = [];
        MTFrame = [];
        laserProps = [];
        targetProps = [];
        bb = [];
        bbObj1 = [];
        bbObj2 = [];
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
            obj.transformMtx = cp2tform(obj.CalibPointsMtx,obj.TestPointsMtx,'similarity');
            for i = 1:length(obj.CalibPointsMtx)
                obj.CalibPointsMtxTrans = [obj.CalibPointsMtxTrans ; tformfwd(obj.transformMtx,obj.CalibPointsMtx(i,1),obj.CalibPointsMtx(i,2))];
            end
        end
         function obj = getBackgroundData(obj,samples)
%             bg = uint8(mean(samples,4));
%             dSamps = double(samples);
%             dSampsR = dSamps(:,:,1,:);
%             dSampsG = dSamps(:,:,2,:);
%             dSampsB = dSamps(:,:,3,:);
%             stdImagR = std(dSampsR,1,4);
%             stdImagG = std(dSampsG,1,4);
%             stdImagB = std(dSampsB,1,4);
%             stdImag(:,:,1) = stdImagR;
%             stdImag(:,:,2) = stdImagG;
%             stdImag(:,:,3) = stdImagB;
            obj.tolImage = uint8(6.*rgb2gray(stdImag));
            
            obj.bgImage = wiener2(rgb2gray(bg),[10 10]);
%         end


        function obj = ProccesPlayingField(obj,frame)
            imFrame = rgb2ycbcr(frame);
            bwFrame = im2bw(imFrame(:,:,2),.55);
            bwRemAr = bwareaopen(bwFrame,3000);
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
                whichCirc(k) = 4*pi*areaObj/perObj^2;
            end
            [valCirc pCirc] = max(whichCirc);
            circPF = imBounds{pCirc};
            [Sval SouthPointFind] = max(circPF(:,1));
            [Nval NorthPointFind] = min(circPF(:,1));
            [Eval EastPointFind] = max(circPF(:,2));
            [Wval WestPointFind] = min(circPF(:,2));
            obj.outputPoints = [ fliplr(circPF(NorthPointFind,:)); fliplr(circPF(EastPointFind,:)); fliplr(circPF(SouthPointFind,:)); fliplr(circPF(WestPointFind,:))];
        end
        function obj = TrackMotion(obj)
            grFR = wiener2(rgb2gray(obj.FR),[10 10]);
            im = wiener2(rgb2gray(im1),[10 10]);
            brighterThanBg = (grFR > obj.bgImage + obj.tolImage);
            darkerThanBg = (grFR < obj.bgImage - obj.tolImage);
            ImThr = (brighterThanBg | darkerThanBg);
            Im = bwareaopen(ImThr,50);
            modFrame(:,:,1) = double(obj.FR(:,:,1)).*Im;
            modFrame(:,:,2) = double(obj.FR(:,:,2)).*Im;
            modFrame(:,:,3) = double(obj.FR(:,:,3)).*Im;
            obj.MTFrame = uint8(modFrame);
        end
        function obj = ProcessLaserFrame(obj)
            redCh = rgb2gray(obj.MTFrame);
            detectionPlane = im2bw(redCh,.98);
            obj.laserProps = regionprops(logical(detectionPlane), 'Area', 'Centroid','BoundingBox');
            area = [obj.laserProps.Area];
            [num_pixels, index] = max(area);
            threshold = 5;
            if (num_pixels > threshold)
                obj.row = obj.laserProps(index).Centroid(2);
                obj.col = obj.laserProps(index).Centroid(1);
                obj.bb  = obj.laserProps(index).BoundingBox;
            else 
                obj.row = NaN;
                obj.col = NaN;
                obj.bb = [];
            end
            try
                obj.transCoordLaser = tformfwd(obj.transformMtx,obj.col,obj.row);
            catch
                obj.transCoordLaser = [obj.row,obj.col];
            end
        end
        function obj = ProcessRoverFrame(obj)
            greenCh = imsubtract(obj.MTFrame(:,:,2), rgb2gray(obj.MTFrame));
            detectionPlane = im2bw(greenCh,0.05);
            rmSmallAreaPlane = bwareaopen(detectionPlane,20);
            obj.targetProps = regionprops(rmSmallAreaPlane, 'Area', 'Centroid','BoundingBox');
            [~, index] = sort([obj.targetProps.Area],'descend');
            
            if isempty(stats)
                obj.targetProps = regionprops(obj.MTFrame,'Area','Centroid','BoundingBox');
                [~, index] = max([obj.targetProps.Area]);
            end
            
            try
                obj.rowObj1 = obj.targetProps(index(1)).Centroid(2);
                obj.colObj1 = obj.targetProps(index(1)).Centroid(1);
                obj.bbObj1  = obj.targetProps(index(1)).BoundingBox;
                obj.transCoordRover1 = tformfwd(obj.transformMtx,obj.colObj1,obj.rowObj1);
            catch
                obj.rowObj1 = NaN;
                obj.colObj1 = NaN;
                obj.bbObj1  = [];
                obj.transCoordRover1 = [obj.colObj1,obj.rowObj1];
            end
            try
                obj.rowObj2 = obj.targetProps(index(2)).Centroid(2);
                obj.colObj2 = obj.targetProps(index(2)).Centroid(1);
                obj.bbObj2  = obj.targetProps(index(2)).BoundingBox;
                obj.transCoordRover2 = tformfwd(obj.transformMtx,obj.colObj2,obj.rowObj2);
            catch
                obj.rowObj2 = NaN;
                obj.colObj2 = NaN;
                obj.bbObj2  = [];
                obj.transCoordRover2 = [obj.colObj2,obj.rowObj2];
            end
        end
        function PlotDetection(obj,bb,Color)
            rectangle('Position',bb,'EdgeColor',Color,'LineWidth',2)

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
