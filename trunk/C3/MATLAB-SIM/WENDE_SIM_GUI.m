function varargout = WENDE_SIM_GUI(varargin)
% WENDE_SIM_GUI MATLAB code for WENDE_SIM_GUI.fig
%      WENDE_SIM_GUI, by itself, creates a new WENDE_SIM_GUI or raises the existing
%      singleton*.
%
%      H = WENDE_SIM_GUI returns the handle to a new WENDE_SIM_GUI or the handle to
%      the existing singleton*.
%
%      WENDE_SIM_GUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in WENDE_SIM_GUI.M with the given input arguments.
%
%      WENDE_SIM_GUI('Property','Value',...) creates a new WENDE_SIM_GUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before WENDE_SIM_GUI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to WENDE_SIM_GUI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help WENDE_SIM_GUI

% Last Modified by GUIDE v2.5 05-Mar-2012 21:21:54

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @WENDE_SIM_GUI_OpeningFcn, ...
                   'gui_OutputFcn',  @WENDE_SIM_GUI_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT
global constants;
global budgetErrors;
global c3Filters;

% --- Executes just before WENDE_SIM_GUI is made visible.
function WENDE_SIM_GUI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to WENDE_SIM_GUI (see VARARGIN)

% Choose default command line output for WENDE_SIM_GUI
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes WENDE_SIM_GUI wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = WENDE_SIM_GUI_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in puRoverMode.
function puRoverMode_Callback(hObject, eventdata, handles)
% hObject    handle to puRoverMode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns puRoverMode contents as cell array
%        contents{get(hObject,'Value')} returns selected item from
%        popupmenu


% --- Executes during object creation, after setting all properties.
function puRoverMode_CreateFcn(hObject, eventdata, handles)
% hObject    handle to puRoverMode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in cmdStartStop.
function cmdStartStop_Callback(hObject, eventdata, handles)
% hObject    handle to cmdStartStop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global constants;
global budgetErrors;
global c3Filters;

set(handles.cmdStartStop,'enable','off')
% Get chosen demo mode
demo_val = get(handles.puDemoMode, 'Value');
% demo_list = get(handles.puDemoMode, 'String');
% demo_mode = demo_list{demo_val};

% Get chosen rover movement mode
rover_val = get(handles.puRoverMode, 'Value');
%rover_list = get(handles.puRoverMode, 'String');
%rover_mode = rover_list{rover_val};

% Get tracker filter choice
tracker_val = get(handles.puTrackerMode, 'Value');
%tracker_list = get(handles.puTrackerMode, 'String');
%tracker_mode = tracker_list{tracker_val};

clc;
if (demo_val == 1)
    postion = [0.053110773899848,0.085365853658537,0.864946889226100,0.879790940766551];
    outerPosition = [-0.159770339197866,-0.024433533933677,1.233394969587529,1.064453547171638];
    set(handles.axes3,'Position',postion);
    set(handles.axes3,'OuterPosition',outerPosition);
    
    axes(handles.axes3) 
    
    [TTI, DTI, time, TravelRange, pf] = wendesim(rover_val, ...
                                                 tracker_val, ...
                                                 constants, ...
                                                 budgetErrors, ...
                                                 c3Filters, ...
                                                 1);
    
    plotDTI(handles,DTI,TTI,pf,time,TravelRange);
else
    %Monte Carlo Params
    cameraOriginRadius = 3.6; %m
    laserOriginRadius = 3.6; %m

    angleResolution = 30; %deg
    numTrials = 1;
    constantsBackup = constants;
    i = 0;
    position = get(handles.axes3,'Position');
    outerPosition = get(handles.axes3,'OuterPosition');
    
    axes(handles.axes3) 
    set(handles.axes3,'Position',[0.05311077389984826,0.08536585365853659,0.8649468892261002,0.872822299651568]);
    set(handles.axes3,'OuterPosition',[0.019726858877086494,0.05574912891986064,0.9059180576631259,0.9163763066202091]);
    
    for cameraBearing = 360:angleResolution:360
        for laserBearing = 0:angleResolution:360
            for roverBearing = 0:angleResolution:360
                for trial = 1:numTrials
                    i = i+1;
                    %SETUP MONTE CARLO PARAMS
                    %**********************************************************  
                    %Rover
                    constants.roverPosition      = [0,0];
                    %Camera
                    constants.cameraOrigin       = [cameraOriginRadius*sind(cameraBearing),cameraOriginRadius*cosd(cameraBearing)];
                    %Laser
                    constants.laserOrigin        = [laserOriginRadius*sind(laserBearing),laserOriginRadius*cosd(laserBearing)];
                    constants.laserPosition      = [0,0];
                    constants.localLaserVelocity = [0,0];
    
                    axes(handles.axes3) 
                    [TTI, DTI, time, TravelRange, pf, res] = wendesim(rover_val,tracker_val, constants, budgetErrors, c3Filters, 0);
                    
                    Results.cameraBearing(i) = cameraBearing;
                    Results.laserBearing(i)  = laserBearing;
                    Results.roverBearing(i)  = roverBearing;
                    Results.tgtAcquired(i)   = res.tgtAcquired;
                    Results.tAcquisition(i)  = res.tAcquisition;
                    
                    plotDTI(handles,DTI,TTI,pf,time,TravelRange);
                    
                    %Results Analysis
                    axes(handles.axes3) 
                    misses.trial = find(Results.tgtAcquired==0);
                    misses.cameraBearing = Results.cameraBearing(misses.trial);
                    misses.laserBearing = Results.laserBearing(misses.trial);
                    misses.roverBearing = Results.roverBearing(misses.trial);
                    misses.cameraToLaser = misses.laserBearing - misses.cameraBearing;
                    misses.laserToRover = misses.roverBearing - misses.laserBearing;

                    %Create Histogram
                    if (isempty(misses.laserToRover) == 1)
                        rose(zeros(1,0),[0:angleResolution:359.9]*pi/180);
                    else
                        rose(misses.laserToRover*pi/180,[0:angleResolution:359.9]*pi/180);
                    end
                    set(gca,'View',[90 90],'YDir','reverse');
                    hline = findobj(gca,'Type','line');
                    set(hline,'LineWidth',3);
                    title({'Histogram of Number of Failures vs. Rover Direction','(Rover Direction is Relative to Laser)',...
                        ['Overall Success Rate = ',num2str(sum(Results.tgtAcquired)/length(Results.tgtAcquired)*100,3),'%'],'',''});                    
%                     if (isempty(misses.laserToRover) == 1)
%                         legend(['Succes Rate 100%%' ])
%                     else
%                         legend(['Succes Rate ' num2str((1-length(misses)/i)*100) '%' ])
%                     end
                end
            end
        end
    end
    saveas(gcf,['Failures_vs_RoverDirection_(',num2str(length(Results.tgtAcquired)),'_Trials).fig'])
    constants = constantsBackup;
end
set(handles.cmdStartStop,'enable','on')

function plotDTI(handles,DTI,TTI,pf,time,TravelRange)
axes(handles.axesDTI)
data = get(handles.tbDTI, 'Data');

if (isempty(data) ~= 1)
    temp = {time, DTI, TTI, TravelRange, pf};
    data = [temp ; data];
else
    data = {time, DTI, TTI, TravelRange, pf};
end

set(handles.tbDTI, 'Data', data);

[x,y] = size(data);
for ii=1:1:x
    t(ii) = data{ii,2};
end

plot(t);
% hold on;
% m = mean(t);
% m = repmat(m,length(t));
% 
% s = std(t);
% s1 = repmat(m+s,length(t));
% s2 = repmat(m-s,length(t));
% plot(m,'r');
% plot(s1,'-g');
% plot(s2,'-g');
% hold off
grid on;
xlabel('DTI Test Iteration');
ylabel('Distance');
axis([0,length(t),0,2]);

%legend(['DTI Values / Succes Rate ' num2str((1-sum(t>=2)/length(t))*100) '%' ]);

% --- Executes on selection change in puTrackerMode.
function puTrackerMode_Callback(hObject, eventdata, handles)
% hObject    handle to puTrackerMode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns puTrackerMode contents as cell array
%        contents{get(hObject,'Value')} returns selected item from puTrackerMode


% --- Executes during object creation, after setting all properties.
function puTrackerMode_CreateFcn(hObject, eventdata, handles)
% hObject    handle to puTrackerMode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in puDemoMode.
function puDemoMode_Callback(hObject, eventdata, handles)
% hObject    handle to puDemoMode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns puDemoMode contents as cell array
%        contents{get(hObject,'Value')} returns selected item from puDemoMode
% demo_val = get(handles.puDemoMode, 'Value');
% if demo_val == 1
%     plotArea(handles);    
% else
%     axes(handles.axes3);
%     %Create Histogram
%     angleResolution = 15;
%     rose(zeros(1,0),[0:angleResolution:359.9]*pi/180);
%     set(gca,'View',[90 90],'YDir','reverse');
%     hline = findobj(gca,'Type','line');
%     set(hline,'LineWidth',3);
%     title({'Histogram of Number of Failures vs. Rover Direction','(Rover Direction is Relative to Laser)',...
%           ['Overall Success Rate = ',num2str(sum(Results.tgtAcquired)/length(Results.tgtAcquired)*100,3),'%'],'',''});                    
% end

% --- Executes during object creation, after setting all properties.
function puDemoMode_CreateFcn(hObject, eventdata, handles)
% hObject    handle to puDemoMode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton5.
function pushbutton5_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes during object creation, after setting all properties.
function cmdStartStop_CreateFcn(hObject, eventdata, handles)
% hObject    handle to cmdStartStop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes on key press with focus on pushbutton5 and none of its controls.
function pushbutton5_KeyPressFcn(hObject, eventdata, handles)
% hObject    handle to pushbutton5 (see GCBO)
% eventdata  structure with the following fields (see UICONTROL)
%	Key: name of the key that was pressed, in lower case
%	Character: character interpretation of the key(s) that was pressed
%	Modifier: name(s) of the modifier key(s) (i.e., control, shift) pressed
% handles    structure with handles and user data (see GUIDATA)


% --- Executes during object creation, after setting all properties.
function tbDTI_CreateFcn(hObject, eventdata, handles)
% hObject    handle to tbDTI (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject, 'Data', []);

% --- Executes during object creation, after setting all properties.
function tblConstants_CreateFcn(hObject, eventdata, handles)
% hObject    handle to tblConstants (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

global constants;

savefile = 'WENDE_SIM_CONSTANTS.mat'; 

if (exist(savefile, 'file'))
    %load from m File
    load(savefile, 'constants');
else
    constants.updatePeriod = .25; %s
    constants.dTime = .05; %s
    constants.cameraOrigin = [1,-2]; %m
    constants.laserOrigin = [-2,3]; %m
    constants.laserPosition = [0,0];%[(rand-0.5),(rand-0.5)];
    constants.localLaserVelocity = [0,0];
    constants.playingFieldRadius = 0.7; %m
    constants.failureLineRadius = 2; %m
    constants.roverPosition = [0,0]; %m
    constants.roverBearing = rand*360; %deg
    constants.playingFieldOrigin = [0,0]; %m
    constants.systemLatency = constants.dTime; %s (System latency. Must be a multiple of "dTime")
    constants.laserHeight = 1.5; %m
    constants.laserMaxSpeed = 20; %deg/s
    constants.laserMaxAcceleration = 500; %deg/s^2
    constants.laserMinSpeed = 0; %deg/s
    constants.laserMinStep = .25; %deg
    constants.targetRadius = 0.005; %deg
    %save to .m file
    save(savefile, 'constants');
end

% Put the data in a matrix form.
data = [ constants.updatePeriod 0; ...
         constants.dTime 0;...
         constants.cameraOrigin; ...
         constants.laserOrigin; ...
         constants.laserPosition;...
         constants.localLaserVelocity; ...
         constants.playingFieldRadius 0; ...
         constants.failureLineRadius 0; ...
         constants.roverPosition; ...
         constants.roverBearing 0; ...
         constants.playingFieldOrigin; ...
         constants.systemLatency 0; ...
         constants.laserHeight 0; ...
         constants.laserMaxSpeed 0; ...
         constants.laserMaxAcceleration 0; ...
         constants.laserMinSpeed 0; ...
         constants.laserMinStep 0; ...
         constants.targetRadius 0];

set(hObject, 'Data', data);


% --- Executes during object creation, after setting all properties.
function tblErrorBudget_CreateFcn(hObject, eventdata, handles)
% hObject    handle to tblErrorBudget (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
global budgetErrors;    

savefile = 'WENDE_SIM_BUDGET_ERRORS.mat'; 
    
if (exist(savefile, 'file'))
    %load from m File
    load(savefile, 'budgetErrors');
else
    budgetErrors.cameraMajorError = 0.01;   %m (Camera Major Axis Random Error: 1 standard deviation)
    budgetErrors.cameraMinorError = 0.01;   %m (Camera Minor Axis Random Error: 1 standard deviation)
    budgetErrors.laserAzError     = 0.06;   %deg (Laser Azimuth Position Random Error: 1 standard deviation)
    budgetErrors.laserElError     = 0.06;   %deg (Laser Elevation Position Random Error: 1 standard deviation)
    budgetErrors.laserAzBias      = 0.05;   %deg/deg (Laser Azimuth Bias: Degrees of bias per degree moved)
    budgetErrors.laserElBias      = 0.05;   %deg/deg (Laser Elevation Bias: Degrees of bias per degree moved)
    budgetErrors.gridlockError    = 0;     %deg (Calibration Error: Angle between Laser and Camera)
    budgetErrors.laserOriginError = 0.10;   %m (Calibration Error: Distance from Center of Playing Field to Laser Origin)

    %save to .m file
    save(savefile, 'budgetErrors');
end

% Put the data in a matrix form.
data = [ budgetErrors.cameraMajorError; ...
    budgetErrors.cameraMinorError; ...
    budgetErrors.laserAzError; ...
    budgetErrors.laserElError; ...
    budgetErrors.laserAzBias; ...
    budgetErrors.laserElBias; ...
    budgetErrors.gridlockError; ...
    budgetErrors.laserOriginError];

set(hObject, 'Data', data);


% --- Executes when entered data in editable cell(s) in tblConstants.
function tblConstants_CellEditCallback(hObject, eventdata, handles)
% hObject    handle to tblConstants (see GCBO)
% eventdata  structure with the following fields (see UITABLE)
%	Indices: row and column indices of the cell(s) edited
%	PreviousData: previous data for the cell(s) edited
%	EditData: string(s) entered by the user
%	NewData: EditData or its converted form set on the Data property. Empty if Data was not changed
%	Error: error string when failed to convert EditData to appropriate value for Data
% handles    structure with handles and user data (see GUIDATA)
global constants
savefile = 'WENDE_SIM_CONSTANTS.mat'; 

if     (eventdata.Indices(1) == 1)
    constants.updatePeriod = eventdata.NewData;
elseif (eventdata.Indices(1) == 2) 
    constants.dTime = eventdata.NewData;
elseif (eventdata.Indices(1) == 3)
    if (eventdata.Indices(2) == 1)
        constants.cameraOrigin(1) = eventdata.NewData;
    else
        constants.cameraOrigin(2) = eventdata.NewData;
    end
    plotArea(handles);        
elseif (eventdata.Indices(1) == 4)
    if (eventdata.Indices(2) == 1)
        constants.laserOrigin(1) = eventdata.NewData;
    else
        constants.laserOrigin(2) = eventdata.NewData;
    end
    plotArea(handles);        
elseif (eventdata.Indices(1) == 5)
    if (eventdata.Indices(2) == 1)
        constants.laserPosition(1) = eventdata.NewData;
    else
        constants.laserPosition(2) = eventdata.NewData;
    end
    plotArea(handles);    
elseif (eventdata.Indices(1) == 6)
    constants.localLaserVelocity = eventdata.NewData;
elseif (eventdata.Indices(1) == 7)
    constants.playingFieldRadius = eventdata.NewData;
elseif (eventdata.Indices(1) == 8)
    constants.failureLineRadius = eventdata.NewData;
elseif (eventdata.Indices(1) == 9)
    if (eventdata.Indices(2) == 1)
        constants.roverPosition(1) = eventdata.NewData;
    else
        constants.roverPosition(2) = eventdata.NewData;
    end
    plotArea(handles);        
elseif (eventdata.Indices(1) == 10)
    constants.roverBearing = eventdata.NewData;
elseif (eventdata.Indices(1) == 11)
    if (eventdata.Indices(2) == 1)
        constants.playingFieldOrigin(1) = eventdata.NewData;
    else
        constants.playingFieldOrigin(2) = eventdata.NewData;
    end
    plotArea(handles);    
elseif (eventdata.Indices(1) == 12)
    constants.systemLatency  = eventdata.NewData;
elseif (eventdata.Indices(1) == 13)
    constants.laserHeight = eventdata.NewData;
elseif (eventdata.Indices(1) == 14)
    constants.laserMaxSpeed = eventdata.NewData;
elseif (eventdata.Indices(1) == 15)
    constants.laserMaxAcceleration = eventdata.NewData;
elseif (eventdata.Indices(1) == 16)
    constants.laserMinSpeed = eventdata.NewData;
elseif (eventdata.Indices(1) == 17)
    constants.laserMinStep = eventdata.NewData;
end

save(savefile, 'constants');


% --- Executes when entered data in editable cell(s) in tblErrorBudget.
function tblErrorBudget_CellEditCallback(hObject, eventdata, handles)
% hObject    handle to tblErrorBudget (see GCBO)
% eventdata  structure with the following fields (see UITABLE)
%	Indices: row and column indices of the cell(s) edited
%	PreviousData: previous data for the cell(s) edited
%	EditData: string(s) entered by the user
%	NewData: EditData or its converted form set on the Data property. Empty if Data was not changed
%	Error: error string when failed to convert EditData to appropriate value for Data
% handles    structure with handles and user data (see GUIDATA)
global budgetErrors;    
savefile = 'WENDE_SIM_BUDGET_ERRORS.mat'; 

if     (eventdata.Indices(1) == 1)
    budgetErrors.cameraMajorError = eventdata.NewData;
elseif (eventdata.Indices(1) == 2) 
    budgetErrors.cameraMinorError = eventdata.NewData;
elseif (eventdata.Indices(1) == 3)
    budgetErrors.laserAzError = eventdata.NewData;
elseif (eventdata.Indices(1) == 4)
    budgetErrors.laserElError = eventdata.NewData;
elseif (eventdata.Indices(1) == 5)
    budgetErrors.laserAzBias = eventdata.NewData;
elseif (eventdata.Indices(1) == 6)
    budgetErrors.laserElBias = eventdata.NewData;
elseif (eventdata.Indices(1) == 7)
    budgetErrors.gridlockError = eventdata.NewData;
elseif (eventdata.Indices(1) == 8)
    budgetErrors.laserOriginError = eventdata.NewData;
end

save(savefile, 'budgetErrors');


% --- Executes on button press in pushbutton6.
function pushbutton6_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes during object creation, after setting all properties.
function tblKalman_CreateFcn(hObject, eventdata, handles)
% hObject    handle to tblKalman (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
global c3Filters;

savefile = 'WENDE_SIM_FILTER.mat'; 

if (exist(savefile, 'file'))
    %load from m File
    load(savefile, 'c3Filters');
else
    c3Filters.processNoise = 9;
    c3Filters.alpha        = .5;
    c3Filters.beta         = .3;
    %save to .m file
    save(savefile, 'c3Filters');
end

% Put the data in a matrix form.
data = [ c3Filters.processNoise; ...
         c3Filters.alpha;...
         c3Filters.beta ];

set(hObject, 'Data', data);


% --- Executes when entered data in editable cell(s) in tblKalman.
function tblKalman_CellEditCallback(hObject, eventdata, handles)
% hObject    handle to tblKalman (see GCBO)
% eventdata  structure with the following fields (see UITABLE)
%	Indices: row and column indices of the cell(s) edited
%	PreviousData: previous data for the cell(s) edited
%	EditData: string(s) entered by the user
%	NewData: EditData or its converted form set on the Data property. Empty if Data was not changed
%	Error: error string when failed to convert EditData to appropriate value for Data
% handles    structure with handles and user data (see GUIDATA)
global c3Filters;    
savefile = 'WENDE_SIM_FILTER.mat'; 

if     (eventdata.Indices(1) == 1)
    c3Filters.processNoise = eventdata.NewData;
elseif (eventdata.Indices(1) == 2) 
    c3Filters.alpha = eventdata.NewData;
elseif (eventdata.Indices(1) == 3)
    c3Filters.beta = eventdata.NewData;
end

save(savefile, 'c3Filters');


% --- Executes during object creation, after setting all properties.
function axes3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes3
%plotArea(hObject);


function plotArea(hObject)
global constants
playingFieldX = constants.playingFieldRadius*cos([0:2*pi/36:2*pi]);
playingFieldY = constants.playingFieldRadius*sin([0:2*pi/36:2*pi]);
failureLineX  = constants.failureLineRadius *cos([0:2*pi/36:2*pi]);
failureLineY  = constants.failureLineRadius *sin([0:2*pi/36:2*pi]);

plot(playingFieldX,playingFieldY,'b',failureLineX,failureLineY,'r');
hold on
text(playingFieldX(3),playingFieldY(3),' \leftarrow Playing Field','FontSize',10)
text(failureLineX(3),failureLineY(3),' \leftarrow Failure Line','FontSize',10)
plot(constants.roverPosition(1),constants.roverPosition(2),'gs','MarkerSize',15,'MarkerFaceColor','g');
text(constants.roverPosition(1)+0.1,constants.roverPosition(2),[' \leftarrow Rover(' num2str(constants.roverPosition(1)) ',' num2str(constants.roverPosition(2)) ')'],'FontSize',10)
plot(constants.laserPosition(1),constants.laserPosition(2),'r.','MarkerSize',20);
text(constants.roverPosition(1)-1,constants.roverPosition(2),['Laser(' num2str(constants.roverPosition(1)) ',' num2str(constants.roverPosition(2)) ')\rightarrow'],'FontSize',10)
plot(constants.cameraOrigin(1),constants.cameraOrigin(2),'mo','MarkerSize',20,'LineWidth',3);
text(constants.cameraOrigin(1)+0.1,constants.cameraOrigin(2),[' \leftarrow Camera(' num2str(constants.cameraOrigin(1)) ',' num2str(constants.cameraOrigin(2)) ')'],'FontSize',10)
plot(constants.laserOrigin(1),constants.laserOrigin(2),'go','MarkerSize',20,'LineWidth',3);
text(constants.laserOrigin(1)+0.1,constants.laserOrigin(2),[' \leftarrow Laser(' num2str(constants.laserOrigin(1)) ',' num2str(constants.laserOrigin(2)) ')'],'FontSize',10)
hold off
xlim([-3 3]);
ylim([-3 3]);
ylabel('Distance (m)');
xlabel('Distance (m)');
axis equal