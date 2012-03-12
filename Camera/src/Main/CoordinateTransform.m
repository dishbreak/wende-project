classdef CoordinateTransform < ObjectDetection
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        xtrfm = 1280;%pixel to meter
        ytrfm = 720;%pixel to meter
        x = [];
        y = [];
        r = [];
        theta = [];
    end
    
    methods
        function [pos] = ConvertToStandard(obj,coordinates)
            %this function is used to convert the coordinates from Pixel
            %to a standard Cartesian Coordinate system where the origin is
            %at 0,0 on the bottom left of the image
            ySize = size(obj.tarFR,1);
            %shift to 0,0 instead of 1,1
            yPixel = ySize-coordinates(2);
            xPixel = coordinates(1)-1;
            %determine the true coordinate
            x = xPixel/obj.xtrfm;
            y = yPixel/obj.ytrfm;
            pos = [x,y];

        end

    end
    
end

