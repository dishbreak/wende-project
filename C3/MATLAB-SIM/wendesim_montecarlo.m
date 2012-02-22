clear all
%Inputs
roverMode = 2; %0:Crawl&Stop  1:Crawl  2:Sprint
maxTime = 35; %s
targetRadius = .005; %m
angleResolution = 30; %deg
numTrials = 1;

%Monte Carlo Params
cameraOriginRadius = 3.6; %m
laserOriginRadius = 3.6; %m


%Error Budgets
global cameraMajorError; cameraMajorError = 0.01;   %m (Camera Major Axis Random Error: 1 standard deviation)
global cameraMinorError; cameraMinorError = 0.01;   %m (Camera Minor Axis Random Error: 1 standard deviation)
global laserAzError; laserAzError = 0.06;           %deg (Laser Azimuth Random Error: 1 standard deviation)
global laserElError; laserElError = 0.06;           %deg (Laser Elevation Random Error: 1 standard deviation)
global laserAzBias; laserAzBias = 0.05;             %deg/deg (Laser Azimuth Bias: Degrees of bias per degree moved)
global laserElBias; laserElBias = 0.05;             %deg/deg (Laser Elevation Bias: Degrees of bias per degree moved)
global gridlockError; gridlockError = 0;            %deg (Calibration Error: Angle between Laser and Camera)
global laserOriginError; laserOriginError = 0.10;   %m (Calibration Error: Distance from Center of Playing Field to Laser Origin)


%Constants
global updatePeriod; updatePeriod = .25; %s
global dTime; dTime = .05; %s
global cameraOrigin; cameraOrigin = [0,0]; %m
global laserOrigin; laserOrigin = [0,0]; %m
global laserPosition; laserPosition = [0,0];%[(rand-0.5),(rand-0.5)];
global playingFieldRadius; playingFieldRadius = 0.7; %m
global failureLineRadius; failureLineRadius = 2; %m
global roverPosition; roverPosition = [0,0]; %m
global roverBearing; roverBearing = rand*360; %deg
global playingFieldOrigin; playingFieldOrigin = [0,0]; %m
global laserHeight; laserHeight = 2; %m
global laserMaxSpeed; laserMaxSpeed = 20; %deg/s
global laserMinSpeed; laserMinSpeed = 0; %deg/s

%Initialize
i = 0;
global c3RoverFilter; c3RoverFilter.initialized = 0;


%WENDE Simulation
for cameraBearing = 0:angleResolution:360
    for laserBearing = 0:angleResolution:360
        for roverBearing = 0:angleResolution:360
            for trial = 1:numTrials
                %SETUP MONTE CARLO PARAMS
                %**********************************************************
                i = i+1;
                %Rover
                roverPosition = [0,0];
                %Camera
                cameraOrigin = [cameraOriginRadius*sind(cameraBearing),cameraOriginRadius*cosd(cameraBearing)];
                %C3
                c3RoverFilter.initialized = 0;
                %Laser
                laserOrigin = [laserOriginRadius*sind(laserBearing),laserOriginRadius*cosd(laserBearing)];
                laserPosition = [0,0];
                %**********************************************************
                display(['Trial Number: ',num2str(i)]);
                for time = 0:dTime:maxTime
                    %SIMULATION PROCESSING
                    %**********************************************************
                    %ROVER
                    roversim(roverMode);
                    
                    if mod(time,updatePeriod) == 0
                        %CAMERA
                        
                        [cameraRoverPosition,cameraLaserPosition] = camerasim;
                        
                        
                        %C3
                        [commandedAzEl] = c3sim(cameraRoverPosition,cameraLaserPosition);
                    end
                    
                    %LASER
                    lasersim(commandedAzEl);
                    %**********************************************************
                    
                    %Compute Miss Distance
                    missDistance = ((laserPosition(1)-roverPosition(1))^2 + (laserPosition(2)-roverPosition(2))^2)^(1/2);
                    
                    %TRIAL EXIT PROCESSING
                    roverRange = (roverPosition(1)^2 + roverPosition(2)^2)^(1/2);
                    if (missDistance < targetRadius && roverRange > playingFieldRadius) || roverRange > failureLineRadius || time >= maxTime
                        if missDistance < targetRadius
                            Results.cameraBearing(i) = cameraBearing;
                            Results.laserBearing(i) = laserBearing;
                            Results.roverBearing(i) = roverBearing;
                            Results.tgtAcquired(i) = 1; %Target Acquired
                            Results.tAcquisition(i) = time;
                        else
                            Results.cameraBearing(i) = cameraBearing;
                            Results.laserBearing(i) = laserBearing;
                            Results.roverBearing(i) = roverBearing;
                            Results.tgtAcquired(i) = 0; %Target Not Acquired
                            Results.tAcquisition(i) = time;
                        end
                        break
                    end
                end
            end
        end
    end
end %End Simulation


%Results Analysis
display(['Success Rate = ',num2str(sum(Results.tgtAcquired)/length(Results.tgtAcquired)*100,3),'%']);

misses.trial = find(Results.tgtAcquired==0);
misses.cameraBearing = Results.cameraBearing(misses.trial);
misses.laserBearing = Results.laserBearing(misses.trial);
misses.roverBearing = Results.roverBearing(misses.trial);
misses.cameraToLaser = misses.laserBearing - misses.cameraBearing;
misses.laserToRover = misses.roverBearing - misses.laserBearing;

%Create Histogram
rose(misses.laserToRover*pi/180,[0:angleResolution:359.9]*pi/180);
set(gca,'View',[90 90],'YDir','reverse');
hline = findobj(gca,'Type','line');
set(hline,'LineWidth',3);
title({'Histogram of Number of Failures vs. Rover Direction','(Rover Direction is Relative to Laser)',...
    ['Overall Success Rate = ',num2str(sum(Results.tgtAcquired)/length(Results.tgtAcquired)*100,3),'%'],'',''});
saveas(gcf,['Failures_vs_RoverDirection_(',num2str(length(Results.tgtAcquired)),'_Trials).fig'])
