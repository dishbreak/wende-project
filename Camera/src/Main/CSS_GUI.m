function varargout = CSS_GUI(varargin)
% CSS_GUI MATLAB code for CSS_GUI.fig
%      CSS_GUI, by itself, creates a new CSS_GUI or raises the existing
%      singleton*.
%
%      H = CSS_GUI returns the handle to a new CSS_GUI or the handle to
%      the existing singleton*.
%
%      CSS_GUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in CSS_GUI.M with the given input arguments.
%
%      CSS_GUI('Property','Value',...) creates a new CSS_GUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before CSS_GUI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to CSS_GUI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help CSS_GUI

% Last Modified by GUIDE v2.5 20-Jun-2012 13:57:08

% Begin initialization code - DO NOT EDIT

gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @CSS_GUI_OpeningFcn, ...
                   'gui_OutputFcn',  @CSS_GUI_OutputFcn, ...
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


% --- Executes just before CSS_GUI is made visible.
function CSS_GUI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to CSS_GUI (see VARARGIN)

% Choose default command line output for CSS_GUI
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes CSS_GUI wait for user response (see UIRESUME)
% uiwait(handles.CameraIRAD);


% --- Outputs from this function are returned to the command line.
function varargout = CSS_GUI_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure

varargout{1} = handles.output;


% --- Executes on button press in InitCam.
function InitCam_Callback(hObject, eventdata, handles)
% hObject    handle to InitCam (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
clear CameraSystem
try
    global CameraSystem 
    CameraSystem = CSS;
    set(handles.Log,'String',{get(handles.Log,'String'); '-------------------------------------------------------'});
    set(handles.Log,'String',[get(handles.Log,'String'); 'Initializing Camera...'])
    CameraSystem = CameraSystem.getCameraInfo;
    CameraSystem.ConfigCamera
    set(handles.Log,'String',[get(handles.Log,'String'); ['Using camera ' char(get(CameraSystem.vid,'Name'))]])
    set(handles.Log,'String',[get(handles.Log,'String'); 'Complete!'])
    set(handles.Log,'String',[get(handles.Log,'String'); '-------------------------------------------------------']);
    set(handles.Log,'String',[get(handles.Log,'String'); 'Initializing Network...'])
    CameraSystem = CameraSystem.ConfigNetwork;
    CameraSystem.Network.initNet(0); CameraSystem.Network.initNet(1); CameraSystem.Network.initNet(2);
    CameraSystem.Network.setSystemStatus(3); 
    set(handles.Log,'String',[get(handles.Log,'String'); 'Complete!']) 
    set(handles.Log,'String',{get(handles.Log,'String'); '-------------------------------------------------------'});
catch
    helpdlg('Camera Initialization Failed!','Failure')
    set(handles.Log,'String',[get(handles.Log,'String'); 'FAILED!'])
    set(handles.Log,'String',[get(handles.Log,'String'); '-------------------------------------------------------']);
end

% --- Executes on button press in PreviewButton.
function PreviewButton_Callback(hObject, eventdata, handles)
% hObject    handle to PreviewButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global CameraSystem 
set(gcf,'CurrentAxes',findobj(gcf,'Tag','Video'))
if strcmp(get(hObject,'String'),'Preview')
    set(handles.Video,'Visible','On')
    vidRes = get(CameraSystem.vid, 'VideoResolution');
    nBands = get(CameraSystem.vid, 'NumberOfBands');
    hImage = image( zeros(vidRes(2), vidRes(1), nBands) );
    preview(CameraSystem.vid,hImage)
    set(handles.Log,'String',[get(handles.Log,'String'); '-------------------------------------------------------']);
    set(handles.Log,'String',[get(handles.Log,'String'); 'Previewing Playing Field']);
    set(handles.Log,'String',[get(handles.Log,'String'); '-------------------------------------------------------']);
    set(hObject,'String','Stop Preview')
else
    stoppreview(CameraSystem.vid)
    set(hObject,'String','Preview')
    set(handles.Log,'String',[get(handles.Log,'String'); '-------------------------------------------------------']);
    set(handles.Log,'String',[get(handles.Log,'String'); 'Preview Stopped']);
    set(handles.Log,'String',[get(handles.Log,'String'); '-------------------------------------------------------']);
end

% --- Executes on button press in CalibButton.
function CalibButton_Callback(hObject, eventdata, handles)
% hObject    handle to CalibButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global CameraSystem 
set(handles.Log,'String',[get(handles.Log,'String'); 'Calculating Background Statistics...']);
n=1;
CameraSystem.StartCamera
while(n~=11)
    CameraSystem = GetFrame(CameraSystem);
    CameraSystem.FlushCameraData
    n=n+1;
end
CameraSystem.StopCamera
bg = CameraSystem.FR;
CameraSystem = getBackgroundData(CameraSystem,bg);
set(handles.Log,'String',[get(handles.Log,'String'); 'Calculating Background Statistics Complete!']);
set(handles.Log,'String',[get(handles.Log,'String'); '-------------------------------------------------------']);

set(handles.Log,'String',[get(handles.Log,'String'); 'Starting Calibration...']);
CameraSystem = CalibratePlayingField(CameraSystem);
CameraSystem = CameraSystem.pxlsToCM;
set(handles.Log,'String',[get(handles.Log,'String'); 'Displaying Transformed Image']);
imageSize = get(CameraSystem.vid,'VideoResolution');
TransImage = imtransform(CameraSystem.FR, CameraSystem.transformMtx,'XData',[1 imageSize(1)],'YData',[1 imageSize(2)]);
imshow(TransImage,'Parent',handles.Video)
set(handles.Log,'String',[get(handles.Log,'String'); 'Calibration Complete!']);
set(handles.Log,'String',[get(handles.Log,'String'); '-------------------------------------------------------']);

% --- Executes on button press in RunButton.
function RunButton_Callback(hObject, eventdata, handles)
% hObject    handle to RunButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global CameraSystem
set(handles.Log,'String',[get(handles.Log,'String'); 'Running Simulation...']);
n=1;
set(gcf,'CurrentAxes',findobj(gcf,'Tag','PPIDisplay'))
CameraSystem.setPPI; grid minor
set(gcf,'CurrentAxes',findobj(gcf,'Tag','Video'))
nFrames = 50;
LaserDist = zeros(1,nFrames);
Rover1Dist = zeros(1,nFrames);
Rover2Dist = zeros(1,nFrames);
SendImageTime = timer('StartFcn','CameraSystem.SendImage','TimerFcn','CameraSystem.SendImage', 'Period', 2.0,'ExecutionMode','fixedRate');
SendSysTime = timer('StartFcn','CameraSystem.SendSysStatus','TimerFcn','CameraSystem.SendSysStatus', 'Period', .25,'ExecutionMode','fixedRate');
CameraSystem.StartCamera
start(SendImageTime);start(SendSysTime);
while(n~=nFrames)
    CameraSystem = GetFrame(CameraSystem);
    CameraSystem = TrackMotion(CameraSystem);
    CameraSystem = ProcessLaserFrame(CameraSystem);
    CameraSystem.Network.setTracks(now, CameraSystem.Network.getSystemStatus,[[CameraSystem.transCoordRover1PPI],[CameraSystem.transCoordRover2PPI]],[CameraSystem.transCoordLaserPPI], CameraSystem.LaserStatus);
    CameraSystem.Network.sendTracks;
    imshow(CameraSystem.FR,'Parent',handles.Video)
    hold(handles.Video)
    plot(handles.Video,CameraSystem.col,CameraSystem.row,'ro','MarkerSize',5,'LineWidth',5)
    CameraSystem = ProcessRoverFrame(CameraSystem);
    plot(handles.Video,CameraSystem.colObj1,CameraSystem.rowObj1,'go','MarkerSize',5,'LineWidth',5)
    plot(handles.Video,CameraSystem.colObj2,CameraSystem.rowObj2,'go','MarkerSize',5,'LineWidth',5)
    hold(handles.Video)
    set(gcf,'CurrentAxes',findobj(gcf,'Tag','PPIDisplay'))
    hold(handles.PPIDisplay)
    try
        plot(handles.PPIDisplay,CameraSystem.transCoordLaserPPI(1).*CameraSystem.MPerPx,CameraSystem.transCoordLaserPPI(2).*CameraSystem.MPerPx,'rO','MarkerSize',5,'LineWidth',5)
    end
    try  
        plot(handles.PPIDisplay,CameraSystem.transCoordRover1PPI(1).*CameraSystem.MPerPx,CameraSystem.transCoordRover1PPI(2).*CameraSystem.MPerPx,'gO','MarkerSize',5,'LineWidth',5)
    end
    try
        plot(handles.PPIDisplay,CameraSystem.transCoordRover2PPI(1).*CameraSystem.MPerPx,CameraSystem.transCoordRover2PPI(2).*CameraSystem.MPerPx,'gO','MarkerSize',5,'LineWidth',5)
    end
    hold( handles.PPIDisplay)
    TravelLaser = CameraSystem.transCoordLaserPPI.*CameraSystem.MPerPx;
    LaserDist(n) = sqrt(TravelLaser(1).^2+TravelLaser(2).^2);
    
    TravelRover1 = CameraSystem.transCoordRover1PPI.*CameraSystem.MPerPx;
    Rover1Dist(n) = sqrt(TravelRover1(1).^2+TravelRover1(2).^2);
    
    TravelRover2 = CameraSystem.transCoordRover2PPI.*CameraSystem.MPerPx;
    Rover2Dist(n) = sqrt(TravelRover2(1).^2+TravelRover2(2).^2);
    
    set(handles.Log,'String',[get(handles.Log,'String'); '************************************']);
    set(handles.Log,'String',[get(handles.Log,'String'); ['Frame ' num2str(n)]]);
    set(handles.Log,'String',[get(handles.Log,'String'); ['Laser : ' num2str(LaserDist(n)) ' m']])
    set(handles.Log,'String',[get(handles.Log,'String'); ['Rover 1: ' num2str(Rover1Dist(n)) ' m']])
    set(handles.Log,'String',[get(handles.Log,'String'); ['Rover 2: ' num2str(Rover2Dist(n)) ' m']])
    hold off
    n = n + 1;
   CameraSystem.FlushCameraData
end
CameraSystem.StopCamera
stop(SendImageTime);stop(SendSysTime);
set(handles.Log,'String',[get(handles.Log,'String'); '************************************']);
set(handles.Log,'String',[get(handles.Log,'String'); 'Simulation Complete!!!']);
for sumFs = 2:nFrames
    LaserError(sumFs-1) = LaserDist(sumFs-1)-LaserDist(sumFs);
    RovError1(sumFs-1) = Rover1Dist(sumFs-1)-Rover1Dist(sumFs);
    RovError2(sumFs-1) = Rover2Dist(sumFs-1)-Rover2Dist(sumFs);
end
set(handles.Log,'String',[get(handles.Log,'String'); '-------------------------------------------------------']);
set(handles.Log,'String',[get(handles.Log,'String'); ['Average Error Over ' num2str(nFrames) ' Frames:']]);
set(handles.Log,'String',[get(handles.Log,'String'); ['Laser Error: ' num2str(nanmean(LaserError)) ' m']])
set(handles.Log,'String',[get(handles.Log,'String'); ['Rover 1 Error: ' num2str(nanmean(RovError1)) ' m']])
set(handles.Log,'String',[get(handles.Log,'String'); ['Rover 2 Error: ' num2str(nanmean(RovError2)) ' m']])
set(handles.Log,'String',[get(handles.Log,'String'); '-------------------------------------------------------']);


function Log_Callback(hObject, eventdata, handles)
% hObject    handle to Log (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Log as text
%        str2double(get(hObject,'String')) returns contents of Log as a double


% --- Executes during object creation, after setting all properties.
function Log_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Log (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes when user attempts to close CameraIRAD.
function CameraIRAD_CloseRequestFcn(hObject, eventdata, handles)
% hObject    handle to CameraIRAD (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global CameraSystem 
try
    CameraSystem.Network.closeNet(0); CameraSystem.Network.closeNet(1); CameraSystem.Network.closeNet(2);
end
% Hint: delete(hObject) closes the figure
delete(hObject);
