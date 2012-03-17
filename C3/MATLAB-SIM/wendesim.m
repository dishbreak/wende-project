function [TTI, DTI, time, TravelRange, pf, Res] = wendesim(rover_mode, ...
                                                           tracker_mode, ...
                                                           constants, ...
                                                           budgetErrors, ...
                                                           c3Filters, ...
                                                           plotSingleTrackItems)

%figure(figureID)clear all
%Inputs
roverMode    = rover_mode-1; %0:Crawl&Stop  1:Crawl  2:Sprint
maxTime      = 35; %s
targetRadius = constants.targetRadius; %m

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
global roverPosition;           roverPosition           = constants.roverPosition; %m
global roverBearing;            roverBearing            = rand*360; %deg
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

intilizeFilter = 1;

%Define Playing Field
playingFieldX = playingFieldRadius*cos([0:2*pi/36:2*pi]);
playingFieldY = playingFieldRadius*sin([0:2*pi/36:2*pi]);
failureLineX = failureLineRadius*cos([0:2*pi/36:2*pi]);
failureLineY = failureLineRadius*sin([0:2*pi/36:2*pi]);

%WENDE Simulation
for time = 0:dTime:maxTime
    %SIMULATION PROCESSING
    %**********************************************************************
    %ROVER
    roversim(roverMode);
    
    if mod(time,updatePeriod) == 0
        %CAMERA
        [cameraRoverPosition,cameraLaserPosition] = camerasim;
    end
    if mod(time - systemLatency,updatePeriod) == 0
        %C3
        [DTI, TravelRange, TTI, commandedAzEl, cameraPipX, cameraPipY, cameraTheta] = c3sim(cameraRoverPosition, ...
                                                                                            cameraLaserPosition, ...
                                                                                            tracker_mode,        ...
                                                                                            playingFieldRadius,  ...
                                                                                            time,                ... 
                                                                                            intilizeFilter,      ...
                                                                                            c3Filters);
        intilizeFilter = 0;
        
        if (commandedAzEl(1) ~= 0 && commandedAzEl(2) ~= 0 && plotSingleTrackItems == 1)
            %*******Calculate Commands in Global (For Display Purposes Only)*******
            globalCameraPipX = cameraPipX*cosd(-cameraTheta) - cameraPipY*sind(-cameraTheta);
            globalCameraPipY = cameraPipX*sind(-cameraTheta) + cameraPipY*cosd(-cameraTheta);
            globalCameraLaserPosition(1) = cameraLaserPosition(1)*cosd(-cameraTheta) - cameraLaserPosition(2)*sind(-cameraTheta);
            globalCameraLaserPosition(2) = cameraLaserPosition(1)*sind(-cameraTheta) + cameraLaserPosition(2)*cosd(-cameraTheta);
            globalCommands(1) = globalCameraPipX - globalCameraLaserPosition(1);
            globalCommands(2) = globalCameraPipY - globalCameraLaserPosition(2);
            %*************************************************************************        
        end
        %
    end
    %LASER
    lasersim(commandedAzEl);
    %**********************************************************************
    
    %Generate Plot
    if (plotSingleTrackItems == 1)
        plot(playingFieldX,playingFieldY,'b',failureLineX,failureLineY,'r');
        hold on
        text(playingFieldX(3),playingFieldY(3),' \leftarrow Playing Field','FontSize',10)
        text(failureLineX(3),failureLineY(3),' \leftarrow Failure Line','FontSize',10)
        plot(roverPosition(1),roverPosition(2),'gs','MarkerSize',15,'MarkerFaceColor','g');
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
        axis equal
    end
    
    roverRange = (roverPosition(1)^2 + roverPosition(2)^2)^(1/2);
    
    %Compute Miss Distance
    missDistance = ((laserPosition(1)-roverPosition(1))^2 + (laserPosition(2)-roverPosition(2))^2)^(1/2);

    if   abs(laserPosition(1)-roverPosition(1)) < targetRadius &&  ...
         abs(laserPosition(2)-roverPosition(2)) < targetRadius &&  ...
         roverRange > playingFieldRadius
        if ((roverMode == 0 && roverRange <= 0.7 + 0.65) || roverMode ~= 0)
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
    elseif roverRange > failureLineRadius || time >= maxTime
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
    if (missDistance < targetRadius && roverRange > playingFieldRadius) || roverRange > failureLineRadius || time >= maxTime
        TTI = time - TTI; 
        Res.tAcquisition   = time;
        break
    end
end %End Simulation
