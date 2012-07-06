classdef CSS
    properties
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % CSS Variable List
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        outerDiameter = 3.943;      % Circle Outer Diamter
        brightness = 80;            % Camera Brightness
        contrast = 10;              % Camera Contrast
        exposure = -10;             % Camera Exposure
        saturation = 200;           % Camera Saturation
        sharpness = 50;             % Camera Sharpness
        whitebalance =2500;         % Camera WhiteBalance
        vid = [];                   % Video Object
        FR = [];                    % Current Frame
        transformMtx = [];          % Transformation Matrix
        CalibPointsMtx = [];        % Calibration Points Matrix
        CalibPointsMtxTrans = [];   % Calibration Points Matrix (Transformed)
        TestPointsMtx = [];         % Test Image Points Matrix
        row = [];                   % Laser Row (Skewed)
        col = [];                   % Laser Column (Skewed)
        rowObj1 = [];               % Rover 1 Row (Skewed)
        colObj1 = [];               % Rover 1 Column (Skewed)
        rowObj2 = [];               % Rover 2 Row (Skewed)
        colObj2 = [];               % Rover 2 Column (Skewed)
        transCoordLaser =[];        % Laser Coords Transformed
        transCoordRover1 =[];       % Rover 1 Coords Transformed
        transCoordRover2 =[];       % Rover 2 Coords Transformed
        transCoordLaserPPI =[];     % Laser Coords Transformed + Scaled to PPI
        transCoordRover1PPI =[];    % Rover 1 Coords Transformed + Scaled to PPI
        transCoordRover2PPI =[];    % Rover 2 Coords Transformed + Scaled to PPI
        outputPoints = [];          % Temporary Calbiration Points Matrix
        bgImage = [];               % Background Image Used for Motion Tracker
        MTFrame = [];               % Motion Tracker Frame
        RemMT = [];                 % Remove Outside the Playing Field
        laserProps = [];            % Laser Properties
        targetProps = [];           % Target Properties
        accCenter = [];             % Center of Playing Field
        accCenterTrans = [];        % Center of Playing Field (Transformed)
        MPerPx = [];                % Meter Per Pixel
        Network = [];               % Network Object
        LaserStatus = 0;            % Laser On/Off
        indexCent = [];             % Center Index
        RoverFlag1 = [];            % Rover Flag 1
        RoverFlag2 = [];            % Rover Flag 2
        rCamera = 3.13*1000;        % Camera Distance from PF (mm)
        htRover = .096*1000;        % Height of Rover (mm) .096
        htCamera = 1.465*1000;      % Height of Camera (mm)
        startTime = clock;          % initial time for delta in messages.
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    end
    
    methods
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Camera Functions
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
            elseif strcmp('HD Pro Webcam C920',deviceName)
                try
                    obj.vid = videoinput('winvideo', 1, 'YUY2_1280x720');
                catch
                    obj.vid = videoinput('winvideo', 2, 'YUY2_1280x720');
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
            obj.FR = getdata(obj.vid,1);       
        end
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Calibration Functions
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        function obj = CalibratePlayingField(obj)
            % Proccess Test Calibration Image and Set Calibration Points
            obj = obj.ProccesPlayingField(imread('CalibImg.png'),0);
            obj.TestPointsMtx = obj.outputPoints;
            % Proccess Playing Field and Set Calibration Points
            obj = obj.ProccesPlayingField(obj.FR,1);
            obj.CalibPointsMtx = obj.outputPoints;
            % Calculate Transformation Matrix
            obj.transformMtx = cp2tform(obj.CalibPointsMtx,obj.TestPointsMtx,'projective'); %Projective the best?
            % Transform Calibration Matrix to New Perspective
            for i = 1:length(obj.CalibPointsMtx)
                obj.CalibPointsMtxTrans = [obj.CalibPointsMtxTrans ; tformfwd(obj.transformMtx,obj.CalibPointsMtx(i,1),obj.CalibPointsMtx(i,2))];
            end
            % Transform Center Point of Circle to New Perspective
            obj.accCenterTrans = tformfwd(obj.transformMtx,obj.accCenter(obj.indexCent).Centroid(1),obj.accCenter(obj.indexCent).Centroid(2));
        end
        function obj = getBackgroundData(obj,bg)
            obj.bgImage = wiener2(rgb2gray(bg),[5 5]); % commented out
            obj.bgImage = rgb2gray(bg);
         end
        function obj = ProccesPlayingField(obj,frame,mtSave)
            imFrame = rgb2ycbcr(frame);
            bwFrame = im2bw(imFrame(:,:,2),.55);
            bwRemAr = bwareaopen(bwFrame,1000);
            findDisk = strel('disk',1);
            imDisk = imclose(bwRemAr,findDisk);
            [imBounds,imLev] = bwboundaries(imDisk,'noholes');
            for k = 1:length(imBounds)
                imBoundsSize(k) = size(imBounds{k},1);
            end
            [~,pCirc] = min(imBoundsSize);
            [~,wRmCirc] = max(imBoundsSize);
            circPF = imBounds{wRmCirc};
            innerPF = imBounds{pCirc};
            if mtSave == 1
                obj.RemMT = logical(zeros(size(bwFrame)));
                for pts = 1:length(circPF)
                    obj.RemMT(circPF(pts,1),circPF(pts,2)) = true;
                end
                obj.RemMT = imfill(obj.RemMT,'holes');
            end
            % Find Outter Circle Bounds
            [Sval SouthPointFind] = max(circPF(:,1));
            [Nval NorthPointFind] = min(circPF(:,1));
            [Eval EastPointFind] = max(circPF(:,2));
            [Wval WestPointFind] = min(circPF(:,2));
            calImN = min(find(circPF(:,2) == (Eval+Wval)/2));
            calImS = max(find(circPF(:,2) == (Eval+Wval)/2));
            if mtSave == 1
                % Find Inner Circle Bounds
                [inSval inSouthPointFind] = max(innerPF(:,1));
                [inNval inNorthPointFind] = min(innerPF(:,1));
                [inEval inEastPointFind] = max(innerPF(:,2));
                [inWval inWestPointFind] = min(innerPF(:,2));
                % Find Outter Circle Vals for Inner Circle Points
                setPointsInE = innerPF(inEastPointFind,:);
                setPointsInW = innerPF(inWestPointFind,:);
                setPointsInN = innerPF(inNorthPointFind,:);
                setPointsInS = innerPF(inSouthPointFind,:);
                nptOutE = max(find(circPF(:,1) == setPointsInE(1)));
                nptOutW = min(find(circPF(:,1) == setPointsInW(1)));
                nptOutN = min(find(circPF(:,2) == setPointsInN(2)));
                nptOutS = max(find(circPF(:,2) == setPointsInS(2)));
                % Actual Center Point
                obj.accCenter = regionprops(~bwRemAr & bwFrame,'Area','Centroid');
                [midSiz, obj.indexCent] = max([obj.accCenter.Area]);
            end
            
            if mtSave == 0
                obj.outputPoints = [ fliplr(circPF(calImN,:)); fliplr(circPF(EastPointFind,:)); fliplr(circPF(calImS,:)); fliplr(circPF(WestPointFind,:)); (circPF(calImN,2)+circPF(calImS,2))/2 (circPF(WestPointFind,1)+circPF(EastPointFind,1))/2];
            else
                obj.outputPoints = [ fliplr(circPF(nptOutN(1),:)); fliplr(circPF(nptOutE(1),:)); fliplr(circPF(nptOutS(1),:)); fliplr(circPF(nptOutW(1),:)); obj.accCenter(obj.indexCent).Centroid(1) obj.accCenter(obj.indexCent).Centroid(2)];
            end
        end
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Motion Tracking Functions
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        function obj = TrackMotion(obj)
            grFR  = rgb2gray(obj.FR);
            brighterThanBg = (grFR > obj.bgImage + 30);
            darkerThanBg = (grFR < obj.bgImage - 30); 
            %ImThr = uint8((brighterThanBg | darkerThanBg) & obj.RemMT);
            ImThr = uint8((brighterThanBg | darkerThanBg));
            obj.MTFrame = obj.FR.*repmat(ImThr,[1 1 3]);
        end
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Detection Functions
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        function obj = ProcessLaserFrame(obj)
            redCh = obj.FR(:,:,1); 
            detectionPlane = im2bw(redCh,.97);
            obj.laserProps = regionprops(logical(detectionPlane), 'Area', 'Centroid');
            area = [obj.laserProps.Area];
            [~, index] = max(area);
            try 
                obj.row = obj.laserProps(index).Centroid(2);
                obj.col = obj.laserProps(index).Centroid(1);
                obj.transCoordLaser = tformfwd(obj.transformMtx,obj.col,obj.row);
                obj.transCoordLaserPPI =[obj.transCoordLaser(1)-obj.accCenterTrans(1),-(obj.transCoordLaser(2)-obj.accCenterTrans(2))].*1000.*obj.MPerPx;
                obj.LaserStatus = 1;
            catch 
                obj.row = NaN;
                obj.col = NaN;
                obj.transCoordLaser = [obj.row,obj.col];
                obj.transCoordLaserPPI = [obj.row,obj.col];
                obj.LaserStatus = 0;
            end
        end
        function obj = ProcessRoverFrame(obj)
%            greenCh = 2*obj.MTFrame(:,:,2)-obj.MTFrame(:,:,1)-obj.MTFrame(:,:,3);
            greenCh = imsubtract(obj.MTFrame(:,:,2),rgb2gray(obj.MTFrame));
            detectionPlane = im2bw(greenCh,0.008);
            obj.targetProps = regionprops(detectionPlane, 'Area', 'Centroid');
            [rovSize, index] = sort([obj.targetProps.Area],'descend');     
            if isempty(obj.targetProps)
                obj.targetProps = regionprops(obj.MTFrame,'Area','Centroid');
                [rovSize, index] = max([obj.targetProps.Area]);
            end
            try
                if rovSize(1) > 1
                    obj.rowObj1 = obj.targetProps(index(1)).Centroid(2);
                    obj.colObj1 = obj.targetProps(index(1)).Centroid(1);
                    obj.transCoordRover1 = tformfwd(obj.transformMtx,obj.colObj1,obj.rowObj1);
                    if  1;%sqrt((obj.transCoordRover1(2)-obj.transCoordLaser(2)).^2+(obj.transCoordRover1(1)-obj.transCoordLaser(1)).^2).*100.*obj.MPerPx < 20
                        obj.transCoordRover1PPI =[obj.transCoordRover1(1)-obj.accCenterTrans(1),-(obj.transCoordRover1(2)-obj.accCenterTrans(2))].*1000.*obj.MPerPx;
                    else
                        xm = [obj.transCoordRover1(1)-obj.accCenterTrans(1)].*1000.*obj.MPerPx;
                        ym = [-(obj.transCoordRover1(2)-obj.accCenterTrans(2))].*1000.*obj.MPerPx;
                        offX = xm.*obj.htRover./obj.htCamera;
                        offY = abs((ym+obj.rCamera).*obj.htRover./obj.htCamera);
                        obj.transCoordRover1PPI =[xm-offX,ym-offY];
                    end
                    obj.RoverFlag1 = 1;
                else
                    error('Not Enought Pixels to be a Rover')
                end
            catch
                obj.rowObj1 = NaN;
                obj.colObj1 = NaN;
                obj.transCoordRover1 = [obj.colObj1,obj.rowObj1];
                obj.transCoordRover1PPI = [obj.colObj1,obj.rowObj1];
                obj.RoverFlag1 = 0;
            end
            try
                if rovSize(2) > 5000
                    obj.rowObj2 = obj.targetProps(index(2)).Centroid(2);
                    obj.colObj2 = obj.targetProps(index(2)).Centroid(1);
                    obj.transCoordRover2 = tformfwd(obj.transformMtx,obj.colObj2,obj.rowObj2);
                    obj.transCoordRover2PPI =[obj.transCoordRover2(1)-obj.accCenterTrans(1),-(obj.transCoordRover2(2)-obj.accCenterTrans(2))].*1000.*obj.MPerPx;
                    obj.RoverFlag2 = 1;
                else
                    error('Not Enought Pixels to be a Rover')
                end
            catch
                obj.rowObj2 = NaN;
                obj.colObj2 = NaN;
                obj.transCoordRover2 = [obj.colObj2,obj.rowObj2];
                obj.transCoordRover2PPI = [obj.colObj2,obj.rowObj2];
                obj.RoverFlag2 = 0;
            end
        end
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % PPI Functions
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        function setPPI(obj)
            angCirc=0:0.01:2*pi; 
            xPlot=200*cos(angCirc);
            yPlot=200*sin(angCirc);
            plot(xPlot,yPlot,'k','LineWidth',5,'Color',[.12 .28 .4902]);
            hold on
            inxPlot=70*cos(angCirc);
            inyPlot=70*sin(angCirc);
            plot(inxPlot,inyPlot,'k','LineWidth',5,'Color',[.12 .28 .4902]);
            set(gca,'FontWeight','Bold');axis([-200.1 200.1 -200.1 200.1]);
            grid on
            hold off
        end
        function obj = pxlsToCM(obj)
            for tPts = 1:length(obj.CalibPointsMtxTrans)
                transPTSave(tPts,:) = [(obj.CalibPointsMtxTrans(tPts,1)-obj.accCenterTrans(1)),-(obj.CalibPointsMtxTrans(tPts,2)-obj.accCenterTrans(2))];
            end
            xSize = min([abs(transPTSave(2,1)) abs(transPTSave(4,1))]);
            ySize = min([abs(transPTSave(1,2)) abs(transPTSave(3,2))]);
            cirSize = max([xSize ySize]);
            obj.MPerPx = obj.outerDiameter/(cirSize*2);
        end
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % Network Functions
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        function obj = ConfigNetwork(obj)
            obj.Network = SendMsg;
        end
        function SendImage(obj)
            sendFrame = zeros(1,180*300*3);
            resizeIm = imresize(obj.FR,[180,300]);
            sendFrame(3:3:end) = reshape(resizeIm(:,:,1)',1,180*300);
            sendFrame(2:3:end) = reshape(resizeIm(:,:,2)',1,180*300);
            sendFrame(1:3:end) = reshape(resizeIm(:,:,3)',1,180*300);
            obj.Network.setImage(uint64(toc(CameraSystem.startTime)*10^3), 3, 300, 180,sendFrame);
            obj.Network.sendImage;
        end
        function SendSysStatus(obj)
            obj.Network.setStatus(uint64(toc(CameraSystem.startTime)*10^3), obj.Network.getSystemStatus , obj.LaserStatus, 'o');
            obj.Network.sendStatus;
        end
    end
end
