function [outC3Tracks] = c3sim( cameraRoverPosition,   ...
                             cameraLaserPosition,   ...
                             tracker_mode,          ...
                             playingFieldRadius,    ...
                             time,                  ...
                             intilizeFilter,        ...
                             c3Filters)
%% Persistent Global Data
global cameraOrigin;
global laserOrigin;
global playingFieldOrigin;
global globalCommands;
global laserHeight;
global updatePeriod;
global gridlockError;
global laserOriginError;

%% static function variables
persistent c3Tracks

[nRows,nCols] = size(cameraRoverPosition);
nContacts = nRows;

%clear the old data
if (intilizeFilter == 1)
    c3Tracks = [];
end

% if the static variables is not initilized
if isempty(c3Tracks) 
    for ii=1:1:nContacts
        % out put items
        c3Tracks(ii).DTI                         = 0;
        c3Tracks(ii).TravelRange                 = 0;
        c3Tracks(ii).TTI                         = 0;
        c3Tracks(ii).commandedAzEl               = [0, 0];
        c3Tracks(ii).cameraPipX                  = 0;
        c3Tracks(ii).cameraPipY                  = 0;
        c3Tracks(ii).cameraTheta                 = 0;
        
        %Kalman FILTERING
        c3Tracks(ii).c3RoverFilter.X             = zeros(6, 1);
        c3Tracks(ii).c3RoverFilter.P             = 9*ones(6,6);
        c3Tracks(ii).c3RoverFilter.X(1,1)        = cameraRoverPosition(ii,1);
        c3Tracks(ii).c3RoverFilter.X(2,1)        = cameraRoverPosition(ii,2);

        c3Tracks(ii).c3RoverFilter.a             = c3Filters.alpha;
        c3Tracks(ii).c3RoverFilter.b             = c3Filters.beta;

        c3Tracks(ii).c3RoverFilter.xk_1          = cameraRoverPosition(ii,:);
        c3Tracks(ii).c3RoverFilter.vk_1          = [0,0];
        c3Tracks(ii).c3RoverFilter.xk            = [0,0];
        c3Tracks(ii).c3RoverFilter.vk            = [0,0];
        c3Tracks(ii).c3RoverFilter.rk            = [0,0];

        % other
        c3Tracks(ii).historyInput                = cameraRoverPosition(ii,:);
        c3Tracks(ii).historyPredictions          = cameraRoverPosition(ii,:);
        c3Tracks(ii).timeHold                    = time;
        
        %Prosecute Flag
        if (EuclideanDistance(cameraRoverPosition(ii,:),[0,0]) <= playingFieldRadius )
            c3Tracks(ii).isProsecute = 1;
        else
            c3Tracks(ii).isProsecute = 0;
        end
    end
end

%% Correlate
nTracks = length(c3Tracks);
% loop through contacts
for ii=1:1:nContacts  
    minTrackDist(ii)    = 100000000;
    minTrackDistNum(ii) = -1;
    minContactNum(ii)   = -1;
    
    %loop through tracks
    for jj=1:1:nTracks     
        % do we have a prediction that should be used for dist
        [rs,cl] = size(c3Tracks(jj).historyInput);
         if (rs >= 3)
             dist = EuclideanDistance(cameraRoverPosition(ii,:),c3Tracks(jj).historyPredictions);                 
         else
            dist = EuclideanDistance(cameraRoverPosition(ii,:),c3Tracks(jj).historyInput(rs,:));                 
         end

        % find track
        if (dist < minTrackDist(ii) && isempty(find(minTrackDistNum==jj))==1)
            minTrackDistNum(ii) = jj;
            minTrackDist(ii) = dist;
            minContactNum(ii) = ii;
        end

    end
    %if no track do ....
    if (minTrackDistNum(ii) == -1)

        zz = nTracks + 1;

        % out put items
        c3Tracks(zz).DTI                         = 0;
        c3Tracks(zz).TravelRange                 = 0;
        c3Tracks(zz).TTI                         = 0;
        c3Tracks(zz).commandedAzEl               = [0,0];
        c3Tracks(zz).cameraPipX                  = 0;
        c3Tracks(zz).cameraPipY                  = 0;
        c3Tracks(zz).cameraTheta                 = 0;

        %FILTERING
        c3Tracks(zz).c3RoverFilter.X             = zeros(6, 1);
        c3Tracks(zz).c3RoverFilter.P             = 9*ones(6,6);
        c3Tracks(zz).c3RoverFilter.X(1,1)        = cameraRoverPosition(ii,1);
        c3Tracks(zz).c3RoverFilter.X(2,1)        = cameraRoverPosition(ii,2);
 
        c3Tracks(zz).c3RoverFilter.a             = c3Filters.alpha;
        c3Tracks(zz).c3RoverFilter.b             = c3Filters.beta;

        c3Tracks(zz).c3RoverFilter.xk_1          = cameraRoverPosition(ii,:);
        c3Tracks(zz).c3RoverFilter.vk_1          = [0,0];
        c3Tracks(zz).c3RoverFilter.xk            = [0,0];
        c3Tracks(zz).c3RoverFilter.vk            = [0,0];
        c3Tracks(zz).c3RoverFilter.rk            = [0,0];


        % other
        c3Tracks(zz).historyInput(1,:)           = cameraRoverPosition(ii,:);
        c3Tracks(zz).historyPredictions          = cameraRoverPosition(ii,:);
        c3Tracks(zz).timeHold                    = time;

        %Prosecute Flag
        if (EuclideanDistance(cameraRoverPosition(ii,:),[0,0]) <= playingFieldRadius )
            c3Tracks(zz).isProsecute = 1;
        else
            c3Tracks(zz).isProsecute = 0;
        end

        minTrackDistNum(ii) = zz;
        minTrackDist(ii)    = 0.0;
        minContactNum(ii)   = ii;
    else
        % TODO ... sanity check to make sure that hist agrees
    end
end
%disp (['track order ' num2str(minTrackDistNum)])
%disp (['Cont  order ' num2str(minContactNum)])
%disp (['track dist  ' num2str(minTrackDist)])
for ii=1:1:nContacts
    %// todo addto history
    [rs,cl] = size(c3Tracks(minTrackDistNum(ii)).historyInput);
    cameraRoverPositionUse  = cameraRoverPosition(ii,:);
    cameraRoverPositionPrev = c3Tracks(minTrackDistNum(ii)).historyInput(rs,:);
    c3RoverFilter           = c3Tracks(minTrackDistNum(ii)).c3RoverFilter;
    travelRange             = c3Tracks(minTrackDistNum(ii)).TravelRange;
    timeHold                = c3Tracks(minTrackDistNum(ii)).timeHold;
    %% CALCULATE PREDICTED INTERCEPT POINT (PIP)
    if (tracker_mode == 1) % Alpha/Beta Filter
        [cameraPipX, cameraPipY, c3RoverFilter] = C3_AlphaBetaFilter(cameraRoverPositionUse, updatePeriod,c3Filters,c3RoverFilter);
    else                   % Kalman Filter (1 State)
        [cameraPipX, cameraPipY, c3RoverFilter] = C3_KalmanFilter(cameraRoverPositionUse, updatePeriod,5,c3Filters,c3RoverFilter);
    end
    
    %% VALIDATE POSTION IN PLAYING FIELD
    %Calculate Rover Range From Center (and return if it is within playing field)
    [DTI] = EuclideanDistance(cameraRoverPositionUse,[0,0]);

    %% If Outside Playing Field Point Laser
    if DTI <= playingFieldRadius || c3Tracks(ii).isProsecute == 0
        commandedAzEl           = [0,0];
        globalCommands          = [0,0];
        cameraTheta             = 0;
        travelRange             = 0;
        timeHold                = time;
        TTI                     = 0;
    else
        %Calculate the travel Range past playing field
        travelRange                = travelRange + EuclideanDistance(cameraRoverPositionPrev,cameraRoverPositionUse);
        
        %
        TTI                        = time - timeHold;

        %FIND CAMERA TO LASER TRANSLATION AND ROTATION PARAMETERS
        %Camera to Laser Rotation Angle
        cameraTheta = atan2(playingFieldOrigin(1,1)-cameraOrigin(1,1), playingFieldOrigin(1,2)-cameraOrigin(1,2))*180/pi + gridlockError;
        laserTheta  = atan2(playingFieldOrigin(1,1)-laserOrigin(1,1) , playingFieldOrigin(1,2)-laserOrigin(1,2)) *180/pi;
        theta       = laserTheta - cameraTheta;

        %Find Laser Origin in Camera Space
        laserOriginCameraSpace(1,1) = laserOrigin(1,1)*cosd(cameraTheta) - laserOrigin(1,2)*sind(cameraTheta);
        laserOriginCameraSpace(1,2) = laserOrigin(1,1)*sind(cameraTheta) + laserOrigin(1,2)*cosd(cameraTheta);

        %Camera to Laser Translation Vector (Camera Coordinate system has origin in playing field center)
        cameraToLaserX = laserOriginCameraSpace(1,1) - playingFieldOrigin(1,1) + laserOriginError;
        cameraToLaserY = laserOriginCameraSpace(1,2) - playingFieldOrigin(1,2) + laserOriginError;

        %TRANSFORM LASER & ROVER POSITIONS INTO LASER SPACE
        %Translate Camera X/Y into Relative X/Y Coordinate System (relative to Laser Origin)
        relativeLaserX = cameraLaserPosition(1,1) - cameraToLaserX;
        relativeLaserY = cameraLaserPosition(1,2) - cameraToLaserY;
        relativePipX = cameraPipX - cameraToLaserX;
        relativePipY = cameraPipY - cameraToLaserY;

        %Rotate Relative X/Y Into Local X/Y Coordinate System (Y axis goes from laser origin to playing field origin)
        localLaserX = relativeLaserX*cosd(theta) - relativeLaserY*sind(theta);
        localLaserY = relativeLaserX*sind(theta) + relativeLaserY*cosd(theta);
        localLaserR = (relativeLaserX^2+relativeLaserY^2)^(1/2);
        localPipX = relativePipX*cosd(theta) - relativePipY*sind(theta);
        localPipY = relativePipX*sind(theta) + relativePipY*cosd(theta);
        localPipR = (relativePipX^2+relativePipY^2)^(1/2);

        %Transform Local X/Y into Local Az/El Coordinate System
        localLaserAz = atan2(localLaserX,localLaserY)*180/pi;
        localLaserEl = atan2(localLaserR,laserHeight)*180/pi;
        localPipAz = atan2(localPipX,localPipY)*180/pi;
        localPipEl = atan2(localPipR,laserHeight)*180/pi;

        %CALCULATE GUIDANCE COMMANDS
        %Calculate Az/El Delta
        dAz = localPipAz - localLaserAz;
        dEl = localPipEl - localLaserEl;

        %Calculate Guidance Commands (in laser's Az/El deg/s)
        commandedAzEl = [dAz/updatePeriod dEl/updatePeriod];
    end
    
    % out put items
    c3Tracks(minTrackDistNum(ii)).DTI                         = DTI;
    c3Tracks(minTrackDistNum(ii)).TravelRange                 = travelRange;
    c3Tracks(minTrackDistNum(ii)).TTI                         = TTI;
    c3Tracks(minTrackDistNum(ii)).commandedAzEl               = commandedAzEl;
    c3Tracks(minTrackDistNum(ii)).cameraPipX                  = cameraPipX;
    c3Tracks(minTrackDistNum(ii)).cameraPipY                  = cameraPipY;
    c3Tracks(minTrackDistNum(ii)).cameraTheta                 = cameraTheta;
    c3Tracks(minTrackDistNum(ii)).historyInput                = [c3Tracks(minTrackDistNum(ii)).historyInput ; cameraRoverPositionUse];
    c3Tracks(minTrackDistNum(ii)).historyPredictions          = [cameraPipX cameraPipY];
    c3Tracks(minTrackDistNum(ii)).timeHold                    = timeHold;
    c3Tracks(minTrackDistNum(ii)).c3RoverFilter               = c3RoverFilter;
end

%TODO.. loop through and create output.
outC3Tracks = c3Tracks;
