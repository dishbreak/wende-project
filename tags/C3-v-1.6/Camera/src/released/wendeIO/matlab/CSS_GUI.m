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

% Last Modified by GUIDE v2.5 25-Mar-2012 15:33:20

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
    CameraSystem = CameraSystem.getCameraInfo;
    CameraSystem.ConfigCamera
    set(handles.Log,'String',{get(handles.Log,'String'); '-------------------------------------------------------'});
    set(handles.Log,'String',[get(handles.Log,'String'); 'Initializing Camera...'])
    set(handles.Log,'String',[get(handles.Log,'String'); ['Using camera ' char(get(CameraSystem.vid,'Name'))]])
    set(handles.Log,'String',[get(handles.Log,'String'); 'Complete!'])
    set(handles.Log,'String',[get(handles.Log,'String'); '-------------------------------------------------------']);
catch
    helpdlg('Camera Initialization Failed!','Failure')
end

% --- Executes on button press in PreviewButton.
function PreviewButton_Callback(hObject, eventdata, handles)
% hObject    handle to PreviewButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global CameraSystem 
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
set(handles.Log,'String',[get(handles.Log,'String'); 'Displaying Transformed Image']);
imageSize = get(CameraSystem.vid,'VideoResolution');
TransImage = imtransform(CameraSystem.FR, CameraSystem.transformMtx,'XData',[1 imageSize(1)],'YData',[1 imageSize(2)]);
imshow(TransImage)
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
CameraSystem.StartCamera
while(n~=51)
    CameraSystem = GetFrame(CameraSystem);
    CameraSystem = TrackMotion(CameraSystem);
    CameraSystem = ProcessLaserFrame(CameraSystem);
    imshow(CameraSystem.FR)
    hold on
    CameraSystem.PlotDetection(CameraSystem.bb,CameraSystem.col,CameraSystem.row,'r','o')
    CameraSystem = ProcessRoverFrame(CameraSystem);
    CameraSystem.PlotDetection(CameraSystem.bbObj1,CameraSystem.colObj1,CameraSystem.rowObj1,'g','o')
    CameraSystem.PlotDetection(CameraSystem.bbObj2,CameraSystem.colObj2,CameraSystem.rowObj2,'g','o')
    CameraSystem = findRoverSep(CameraSystem);
    set(handles.Log,'String',[get(handles.Log,'String'); '************************************']);
    set(handles.Log,'String',[get(handles.Log,'String'); ['Frame ' num2str(n)]]);
    set(handles.Log,'String',[get(handles.Log,'String'); ['Laser : (' num2str(CameraSystem.transCoordLaser(1)) ',' num2str(CameraSystem.transCoordLaser(2)) ')']])
    set(handles.Log,'String',[get(handles.Log,'String'); ['Rover 1: (' num2str(CameraSystem.transCoordRover1(1)) ',' num2str(CameraSystem.transCoordRover1(2)) ')']])
    set(handles.Log,'String',[get(handles.Log,'String'); ['Rover 2: (' num2str(CameraSystem.transCoordRover2(1)) ',' num2str(CameraSystem.transCoordRover2(2)) ')']])
    set(handles.Log,'String',[get(handles.Log,'String'); ['Rover Separation TForm: ' num2str(CameraSystem.roverSep) ' m']]);
        set(handles.Log,'String',[get(handles.Log,'String'); ['Rover Separation Non TForm: ' num2str(CameraSystem.roverSepNT) ' m']]);
    hold off
    n = n + 1;
   CameraSystem.FlushCameraData
end
CameraSystem.StopCamera
set(handles.Log,'String',[get(handles.Log,'String'); '************************************']);
set(handles.Log,'String',[get(handles.Log,'String'); 'Simulation Complete!!!']);
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


% --- Executes on button press in LaserPoint1.
function LaserPoint1_Callback(hObject, eventdata, handles)
% hObject    handle to LaserPoint1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global CameraSystem
set(handles.Log,'String',[get(handles.Log,'String'); 'Getting 1st Laser Point...']);
CameraSystem.StartCamera
CameraSystem = GetFrame(CameraSystem);
CameraSystem = TrackMotion(CameraSystem);
CameraSystem = ProcessLaserFrame(CameraSystem);
imshow(CameraSystem.FR)
hold on
CameraSystem.PlotDetection(CameraSystem.bb,CameraSystem.col,CameraSystem.row,'r','o')
CameraSystem.FlushCameraData
CameraSystem.StopCamera
set(handles.Log,'String',[get(handles.Log,'String'); ['Laser Point 1 : (' num2str(CameraSystem.transCoordLaser(1)) ',' num2str(CameraSystem.transCoordLaser(2)) ')']])
set(hObject,'UserData',CameraSystem.transCoordLaser);
set(handles.Log,'String',[get(handles.Log,'String'); 'Done!']);
set(handles.Log,'String',[get(handles.Log,'String'); '-------------------------------------------------------']);
% --- Executes on button press in LaserPoint2.
function LaserPoint2_Callback(hObject, eventdata, handles)
% hObject    handle to LaserPoint2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global CameraSystem
set(handles.Log,'String',[get(handles.Log,'String'); 'Getting 2nd Laser Point...']);
CameraSystem.StartCamera
CameraSystem = GetFrame(CameraSystem);
CameraSystem = TrackMotion(CameraSystem);
CameraSystem = ProcessLaserFrame(CameraSystem);
imshow(CameraSystem.FR)
hold on
CameraSystem.PlotDetection(CameraSystem.bb,CameraSystem.col,CameraSystem.row,'r','o')
CameraSystem.FlushCameraData
CameraSystem.StopCamera
set(handles.Log,'String',[get(handles.Log,'String'); ['Laser Point 2 : (' num2str(CameraSystem.transCoordLaser(1)) ',' num2str(CameraSystem.transCoordLaser(2)) ')']])
set(handles.Log,'String',[get(handles.Log,'String'); 'Done!']);
set(handles.Log,'String',[get(handles.Log,'String'); '-------------------------------------------------------']);
set(handles.Log,'String',[get(handles.Log,'String'); 'Calculating Distance Between Points...']);
fLaserPoint = get(handles.LaserPoint1,'UserData');
pixPerMeter = sqrt((CameraSystem.CalibPointsMtxTrans(2,1)-CameraSystem.CalibPointsMtxTrans(4,1))^2+(CameraSystem.CalibPointsMtxTrans(2,2)-CameraSystem.CalibPointsMtxTrans(2,2))^2)/CameraSystem.outerDiameter;
pixDistance = sqrt((fLaserPoint(1)-CameraSystem.transCoordLaser(1))^2+(fLaserPoint(2)-CameraSystem.transCoordLaser(2))^2);
LaserSep = pixDistance/pixPerMeter;
set(handles.Log,'String',[get(handles.Log,'String'); 'Distance Between Points = ' num2str(LaserSep) ' m']);
set(handles.Log,'String',[get(handles.Log,'String'); 'Done!']);
set(handles.Log,'String',[get(handles.Log,'String'); '-------------------------------------------------------']);