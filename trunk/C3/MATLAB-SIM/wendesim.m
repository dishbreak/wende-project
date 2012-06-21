function [outC3Tracks, time, pf, Res] = wendesim(rover_mode, ...
                                                           tracker_mode, ...
                                                           constants, ...
                                                           budgetErrors, ...
                                                           c3Filters, ...
                                                           plotSingleTrackItems)

%figure(figureID)clear all
%Inputs
roverMode    = rover_mode-1; %0:Crawl&Stop  1:Crawl  2:Sprint
maxTime      = 120; %s
targetRadius = constants.targetRadius; %m

clear global
clear persistent

%Error Budgets
global cameraMajorError;    cameraMajorError    = budgetErrors.cameraMajorError;   %m (Camera Major Axis Random Error: 1 standard deviation)
global cameraMinorError;    cameraMinorError    = budgetErrors.cameraMinorError;   %m (Camera Minor Axis Random Error: 1 standard deviation)
global laserAzError;        laserAzError        = budgetErrors.laserAzError;       %deg (Laser Azimuth Random Error: 1 standard deviation)
global laserElError;        laserElError        = budgetErrors.laserElError;       %deg (Laser Elevation Random Error: 1 standard deviation)
global laserAzBias;         laserAzBias         = budgetErrors.laserAzBias;        %deg/deg (Laser Azimuth Bias: Degrees of bias per degree moved)
global laserElBias;         laserElBias         = budgetErrors.laserElBias;        %deg/deg (Laser Elevation Bias: Degrees of bias per degree moved)
global gridlockError;       gridlockError       = budgetErrors.gridlockError;      %deg (Calibration Error: Angle between Laser and Camera)
global laserOriginError;    laserOriginError    = budgetErrors.laserOriginError;   %m (Calibration Error: Distance from Center of Playing Field to Laser Origin)


%Constants
global updatePeriod;            updatePeriod            = constants.updatePeriod; %s
global dTime;                   dTime                   = constants.dTime; %s
global cameraOrigin;            cameraOrigin            = constants.cameraOrigin; %m
global laserOrigin;             laserOrigin             = constants.laserOrigin; %m
global laserPosition;           laserPosition           = constants.laserPosition;%[(rand-0.5),(rand-0.5)];
global localLaserVelocity;      localLaserVelocity      = constants.localLaserVelocity;
global playingFieldRadius;      playingFieldRadius      = constants.playingFieldRadius; %m
global failureLineRadius;       failureLineRadius       = constants.failureLineRadius; %m
clear roverPosition
global roverPosition;           roverPosition           = constants.roverPosition; %m
if (roverMode == 4)
    numRovers = 2;
else
    numRovers = 1;
end
global roverBearing;            roverBearing            = round(rand(numRovers,1).*360); %deg
global playingFieldOrigin;      playingFieldOrigin      = constants.playingFieldOrigin; %m
global systemLatency;           systemLatency           = constants.systemLatency; %s (System latency. Must be a multiple of "dTime")
global laserHeight;             laserHeight             = constants.laserHeight; %m
global laserMaxSpeed;           laserMaxSpeed           = constants.laserMaxSpeed; %deg/s
global laserMaxAcceleration;    laserMaxAcceleration    = constants.laserMaxAcceleration; %deg/s^2
global laserMinSpeed;           laserMinSpeed           = constants.laserMinSpeed; %deg/s
global laserMinStep;            laserMinStep            = constants.laserMinStep; %deg
global processNoise;            processNoise            = 5;
global updateRate;              updateRate              = constants.updatePeriod; %s

%Initialize
global globalCommands;
commandedAzEl           = [0,0];
globalCommands          = [0,0];
    
intilizeFilter = 1;
commandedLaser = 0;
isFound = 0
%Define Playing Field
playingFieldX = playingFieldRadius*cos([0:2*pi/36:2*pi]);
playingFieldY = playingFieldRadius*sin([0:2*pi/36:2*pi]);
failureLineX = failureLineRadius*cos([0:2*pi/36:2*pi]);
failureLineY = failureLineRadius*sin([0:2*pi/36:2*pi]);

if (roverMode == 4)
    %roverBearing(1) = roverBearing(2);
    roverPosition(1,1) = 0;
    roverPosition(1,2) = 0;
    for oo = 1:1:numRovers
        xdir = sind(mod((roverBearing(oo)-180),360));
        ydir = cosd(mod((roverBearing(oo)-180),360));
        roverDirection(1) = xdir/(xdir^2 + ydir^2)^(1/2);
        roverDirection(2) = ydir/(xdir^2 + ydir^2)^(1/2);
        roverPosition(2,1) = roverDirection(1)*(failureLineRadius-.5);
        roverPosition(2,2) = roverDirection(2)*(failureLineRadius-.5);
    end
    startOther = 0;
else
    startOther = 1;
    roverPosition(1,1) = 0;
    roverPosition(1,2) = 0;
end
outC3Tracks = [];

%For writing track data to a file
%fileID = fopen('TestTrack_1.txt','w');

%WENDE Simulation
for time = 0:dTime:maxTime
    %SIMULATION PROCESSING
    %**********************************************************************
    %ROVER
    roversim(roverMode,startOther);
    
    if mod(time,updatePeriod) == 0
        %CAMERA
        [cameraRoverPosition,cameraLaserPosition] = camerasim(roverMode);
     end
     if (mod(time - systemLatency,updatePeriod) == 0)
        %C3
%         index = mod(floor(rand*100),2);
%         disp('******')
%         if (index == 0)
%             disp('index 0')
%             tempC(1,:) = cameraRoverPosition(1,:);
%             tempC(2,:) = cameraRoverPosition(2,:);
%         else
%             disp('index 1')
%             tempC(1,:) = cameraRoverPosition(2,:);
%             tempC(2,:) = cameraRoverPosition(1,:);
%         end
%         disp('******')
%        cameraRoverPosition = tempC;
%  if roverMode == 4
%        Print rover and laser position data to a file opend above for test purposes
%        fprintf(fileID,'%12.12f %12.12f %12.12f %12.12f %12.12f %12.12f %12.12f\n', ...
%                cameraRoverPosition(1,1),cameraRoverPosition(1,2), ...
%                cameraRoverPosition(2,1),cameraRoverPosition(2,2), ...
%                cameraLaserPosition(1,1),cameraLaserPosition(1,2), ...
%                time);
%  else
%  	 Print rover and laser position data to a file opend above for test purposes
%        fprintf(fileID,'%12.12f %12.12f %12.12f %12.12f %12.12f\n', ...
%                cameraRoverPosition(1,1),cameraRoverPosition(1,2), ...
%                cameraLaserPosition(1,1),cameraLaserPosition(1,2), ...
%                time);
%  end            
        [outC3Tracks] = c3sim(cameraRoverPosition,               ...
                              cameraLaserPosition, ...
                              tracker_mode,        ...
                              playingFieldRadius,  ...
                              time,                ... 
                              intilizeFilter,      ...
                              c3Filters);
% Print the azimuth and elevation data to the file opened above for test purposes                          
%        fprintf(fileID,'%12.12f %12.12f\n', ...
%                        outC3Tracks(1).commandedAzEl(1,1),outC3Tracks(1).commandedAzEl(1,2));
                    
%        disp(['Output' num2str(index)] );
        %for zz = 1:1:length(outC3Tracks)
        %    disp(['track ' num2str(zz) ' ' num2str([outC3Tracks(zz).cameraPipX outC3Tracks(zz).cameraPipY]) ]);
        %end
        intilizeFilter = 0;
        commandedLaser = 1;
        for hh=1:1:length(outC3Tracks)
            commandedAzEl = outC3Tracks(hh).commandedAzEl;
            cameraPipX    = outC3Tracks(hh).cameraPipX;
            cameraPipY    = outC3Tracks(hh).cameraPipY;
            cameraTheta   = outC3Tracks(hh).cameraTheta;
            if (commandedAzEl(1) ~= 0 && commandedAzEl(2) ~= 0 && plotSingleTrackItems == 1)

                %*******Calculate Commands in Global (For Display Purposes Only)*******
                globalCameraPipX = cameraPipX*cosd(-cameraTheta) - cameraPipY*sind(-cameraTheta);
                globalCameraPipY = cameraPipX*sind(-cameraTheta) + cameraPipY*cosd(-cameraTheta);
                globalCameraLaserPosition(1) = cameraLaserPosition(1)*cosd(-cameraTheta) - cameraLaserPosition(2)*sind(-cameraTheta);
                globalCameraLaserPosition(2) = cameraLaserPosition(1)*sind(-cameraTheta) + cameraLaserPosition(2)*cosd(-cameraTheta);
                gCommands(1) = globalCameraPipX - globalCameraLaserPosition(1);
                gCommands(2) = globalCameraPipY - globalCameraLaserPosition(2);
                %*************************************************************************        
            else
                gCommands = [0,0];
            end
            globalCommands(hh,:) = gCommands;
        end
        %
    end
    %LASER
    if (isempty(outC3Tracks) == 0)
        commandedAzEl = outC3Tracks(1).commandedAzEl;
        for hh=2:1:length(outC3Tracks)
            commandedAzEl = [commandedAzEl; outC3Tracks(hh).commandedAzEl];
        end
    end
    
    % TODO take multiple az el
    lasersim(commandedAzEl);
    %**********************************************************************
    
    %Generate Plot
    if (plotSingleTrackItems == 1)
        plot(playingFieldX,playingFieldY,'b',failureLineX,failureLineY,'r');
        hold on
        text(playingFieldX(3),playingFieldY(3),' \leftarrow Playing Field','FontSize',10)
        text(failureLineX(3),failureLineY(3),' \leftarrow Failure Line','FontSize',10)
        [rr,cc] = size(roverPosition);
        theta = atan2(playingFieldOrigin(1)-cameraOrigin(1),playingFieldOrigin(2)-cameraOrigin(2))*180/pi;
        for hh = 1:1:rr
            plot(roverPosition(hh,1),roverPosition(hh,2),'gs','MarkerSize',15,'MarkerFaceColor','g');
            text(roverPosition(hh,1),roverPosition(hh,2),[ ' \leftarrow ' num2str(hh) ],'FontSize',10);
            
            temp(1) = cameraRoverPosition(hh,1)*cosd(-theta) - cameraRoverPosition(hh,2)*sind(-theta);
            temp(2) = cameraRoverPosition(hh,1)*sind(-theta) + cameraRoverPosition(hh,2)*cosd(-theta);
            plot(temp(1),temp(2),'bs');
            
            if (~isempty(outC3Tracks) )
                cameraPipX    = outC3Tracks(hh).cameraPipX;
                cameraPipY    = outC3Tracks(hh).cameraPipY; 
                temp(1) = cameraPipX*cosd(-theta) - cameraPipY*sind(-theta);
                temp(2) = cameraPipX*sind(-theta) + cameraPipY*cosd(-theta);
                plot(temp(1),temp(2),'rs');
            end
        end
        % TODO FIX
        plot([laserPosition(1),laserPosition(1)+globalCommands(1)],[laserPosition(2),laserPosition(2)+globalCommands(2)],'m','MarkerSize',2);
        plot(laserPosition(1),laserPosition(2),'r.','MarkerSize',20);
        plot(cameraOrigin(1),cameraOrigin(2),'mo','MarkerSize',20,'LineWidth',3);
        plot(laserOrigin(1),laserOrigin(2),'go','MarkerSize',20,'LineWidth',3);
        hold off
        xlim([-3 3]);
        ylim([-3 3]);
        ylabel('Distance (m)');
        xlabel('Distance (m)');
        grid on;
        %%
        axis equal
    end
    
    if( startOther == 1 )
        tempRange = 10000;
    else
        tempRange = 0;
    end
    
    [rr,cc]=size(roverPosition);
    for ii = 1:1:rr
        range = (roverPosition(ii,1)^2 + roverPosition(ii,2)^2)^(1/2);
        if( startOther == 1 )
            if(range < tempRange)
               tempRange = range;
            end
        else
            if(range > tempRange)
               tempRange = range;
            end
        end
    end

    roverRange = tempRange;

    %Compute Miss Distance
    missDistance = ((laserPosition(1)-roverPosition(1,1))^2 + (laserPosition(2)-roverPosition(1,2))^2)^(1/2);

    if (commandedLaser == 1 && roverRange > playingFieldRadius && startOther == 1)        
        S(1).P.x = laserPosition(1)+0.005*cos([0:2*pi/36:2*pi]);
        S(1).P.y = laserPosition(2)+0.005*sin([0:2*pi/36:2*pi]);
        S(1).P.hole = 0;
        position=[0 0];
        x1  = [position(1)-targetRadius position(1)-targetRadius position(1)+targetRadius position(1)+targetRadius position(1)-targetRadius];
        y1  = [position(2)-targetRadius position(2)+targetRadius position(2)+targetRadius position(2)-targetRadius position(2)-targetRadius];
        % todo fix
        x2  = x1.*cos(roverBearing(1)*pi/180)-y1.*sin(roverBearing(1)*pi/180);
        y2  = x1.*sin(roverBearing(1)*pi/180)+y1.*cos(roverBearing(1)*pi/180);
        S(2).P.x = roverPosition(1,1)+x2;
        S(2).P.y = roverPosition(1,2)+y2;
        S(2).P.hole = 0;
        Display_result = 0;

        G = Polygons_intersection(S,Display_result);
        for lii = 1:1:length(G)
            if (length(G(lii).index)>1)
                if (length(G)>2)
                    if (G(3).area/G(1).area > .25 && G(3).area/G(1).area < 1 || ...
                        G(3).area/G(2).area > .25 && G(3).area/G(2).area < 1 )
                        isFound = 1; 
                    end
                else
                    isFound = 1; 
                end
            end
        end
    end
    %disp(['startOther = ' num2str(startOther) '   commandedLaser = ' num2str(commandedLaser) '   Range = ' num2str(roverRange) '   isFound = ' num2str(isFound)]);
    if isFound == 1 && roverRange > playingFieldRadius%if there is any overlap
        if ((roverMode == 0 && roverRange <= 0.7 + 0.65) || roverMode ~= 0) % if there is overlap and not at stop crawl range
            if (plotSingleTrackItems == 1)
                title(['Time = ',num2str(time,'%3.2f'),'s. Miss Distance = ',num2str(missDistance*100,'%05.1f'),'cm. ****SUCCESS****']);
            end
            pf = '          P';
            Res.tgtAcquired    = 1; %Target Acquired
        else
            if (plotSingleTrackItems == 1)
                title(['Time = ',num2str(time,'%3.2f'),'s. Miss Distance = ',num2str(missDistance*100,'%05.1f'),'cm. ****FAILURE****']);
            end
            pf = '          F';
            Res.tgtAcquired = 0; %Target Not Acquired
        end
    elseif roverRange > failureLineRadius || time >= maxTime % max time or range reached
        if (plotSingleTrackItems == 1)
            title(['Time = ',num2str(time,'%3.2f'),'s. Miss Distance = ',num2str(missDistance*100,'%05.1f'),'cm. ****FAILURE****']);
        end
        pf = '          F';
        Res.tgtAcquired   = 0; %Target Not Acquired
    else
        if (plotSingleTrackItems == 1)
            title(['Time = ',num2str(time,'%3.2f'),'s. Miss Distance = ',num2str(missDistance*100,'%05.1f'),'cm.']);
        end
    end
    
    %EXIT
    if (plotSingleTrackItems == 1)
        pause(dTime/10);
    end
    %TODO fix this
    if ((isFound == 1 && roverRange > playingFieldRadius) || ...  % Exit criteria for modes 0-3
        roverRange > failureLineRadius || ...                     % this should fail in any case
        time >= maxTime)                                          % this should fail in anyt case
        if (roverMode == 4 && startOther == 0)
            startOther = 1;
        else
            Res.tAcquisition   = time;
            break
        end;
    end
end %End Simulation

%Close the TestTrack file opened above 
%fclose(fileID);