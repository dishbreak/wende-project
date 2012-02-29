classdef ObjectDetection < CSS
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        laserPos = [];
        targetPos = [];
        lasFR = [];
        tarFR = [];
        
    end
    
    methods
        function obj = DetectLaser(obj)
            disp('Functionality Not Yet Designed');
            %obj.lasFR = getdata(obj.vid,1);
            %obj.laserPos = [5,7];
            flushdata(obj.vid);
        end
        
        function obj = DetectTarget(obj)
            %disp('checking target position')
            obj.tarFR = getdata(obj.vid,1);
            
            % Now to track red objects in real time
            % we have to subtract the red component
            % from the grayscale image to extract the red components in the image.
            diff_im = imsubtract(obj.tarFR(:,:,1), rgb2gray(obj.tarFR));
            %Use a median filter to filter out noise
            diff_im = medfilt2(diff_im, [3 3]);
            % Convert the resulting grayscale image into a binary image.
            diff_im = im2bw(diff_im,0.18);
            
            % Remove all those pixels less than 300px
            diff_im = bwareaopen(diff_im,500);
            
            % Label all the connected components in the image.
            
            bw = bwlabel(diff_im, 8); %takes longer
            
            % Here we do the image blob analysis.
            % We get a set of properties for each labeled region.
            props = regionprops(bw, 'Area', 'Centroid');
            if ~isempty(props)
                obj.targetPos = ComputeAreaCentroid(obj,props);
            else
                obj.targetPos = [];
            end
            flushdata(obj.vid);
            
        end
        %         function obj = DetectTarget(obj)
        %             %             obj.currFrame = imtransform(obj.FR, obj.transformMtx,'XData',[1 obj.Width],'YData',[1 obj.Height]);
        %             obj.tarFR = getdata(obj.vid,1);
        %             detectionPlane = im2bw(obj.tarFR(:,:,1),0.17);
        %             rmSmallAreaPlane = bwareaopen(detectionPlane,100);
        %             edgePlane = edge(rmSmallAreaPlane,'canny',.999);
        %             props = regionprops(logical(edgePlane), 'Area', 'Centroid');
        %             targetPos = ComputeAreaCentroid(obj,props);
        %             obj.targetPos = targetPos;
        %             flushdata(obj.vid);
        %
        %         end
        
        
    end
    methods (Access = private)
        function centroid = ComputeAreaCentroid(obj,properties)
            totArea = 0;
            sumAreaPosX=0;
            sumAreaPosY=0;
            for i = 1:size(properties,1)
                totArea = properties(i).Area+totArea;
                sumAreaPosX = sumAreaPosX+(properties(i).Area*properties(i).Centroid(1));
                sumAreaPosY = sumAreaPosY+(properties(i).Area*properties(i).Centroid(2));
            end
            x = sumAreaPosX/totArea;
            y = sumAreaPosY/totArea;
            centroid = [x,y];
            
        end
        
    end
    
    
end

