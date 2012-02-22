clear all
%Inputs
roverMode = 2; %0:Crawl&Stop  1:Crawl  2:Sprint
maxTime = 35; %s
targetRadius = .005; %m

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
global cameraOrigin; cameraOrigin = [1,-2]; %m
global laserOrigin; laserOrigin = [-2,3]; %m
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
global globalCommands;



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
        
        
        
        %C3
        [commandedAzEl] = c3sim(cameraRoverPosition,cameraLaserPosition);
    end
    
    %LASER
    lasersim(commandedAzEl);
    %**********************************************************************
    
    %Compute Miss Distance
    missDistance = ((laserPosition(1)-roverPosition(1))^2 + (laserPosition(2)-roverPosition(2))^2)^(1/2);


    %Generate Plot
    plot(playingFieldX,playingFieldY,'k',failureLineX,failureLineY,'r');
    hold on
    plot(roverPosition(1),roverPosition(2),'bs','MarkerSize',15,'MarkerFaceColor','b');
    plot([laserPosition(1),laserPosition(1)+globalCommands(1)],[laserPosition(2),laserPosition(2)+globalCommands(2)],'m','MarkerSize',2);
    plot(laserPosition(1),laserPosition(2),'g.','MarkerSize',20);
    plot(cameraOrigin(1),cameraOrigin(2),'mo','MarkerSize',20,'LineWidth',3);
    plot(laserOrigin(1),laserOrigin(2),'go','MarkerSize',20,'LineWidth',3);
    hold off
    xlim([-3 3]);
    ylim([-3 3]);
    axis equal
    set(gcf,'Position',[50,50,800,750]) %[left,bottom,width,height]
    
    roverRange = (roverPosition(1)^2 + roverPosition(2)^2)^(1/2);
    if missDistance < targetRadius && roverRange > playingFieldRadius
        title(['Time = ',num2str(time,'%3.2f'),'s. Miss Distance = ',num2str(missDistance*100,'%05.1f'),'cm. ****SUCCESS****']);
    elseif roverRange > failureLineRadius || time >= maxTime
        title(['Time = ',num2str(time,'%3.2f'),'s. Miss Distance = ',num2str(missDistance*100,'%05.1f'),'cm. ****FAILURE****']);
    else
        title(['Time = ',num2str(time,'%3.2f'),'s. Miss Distance = ',num2str(missDistance*100,'%05.1f'),'cm.']);
    end
    
    
    %Save Frame
    f = getframe(gcf);
    i = i+1;
    im(:,:,1,i) = rgb2ind(f.cdata,colorcube,'nodither');
    
    if (missDistance < targetRadius && roverRange > playingFieldRadius) || roverRange > failureLineRadius || time >= maxTime
        %Generate Last Frame Pause
        for j = 1:20
            i = i+1;
            im(:,:,1,i) = rgb2ind(f.cdata,colorcube,'nodither');
        end
        break
    end
end %End Simulation

%Generate Video
imwrite(im,colorcube,'wendesimOut.gif','DelayTime',dTime,'Loopcount',65535);