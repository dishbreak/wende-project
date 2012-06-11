function varargout = videoCalGUI(varargin)
% VIDEOCALGUI MATLAB code for videoCalGUI.fig
%      VIDEOCALGUI, by itself, creates a new VIDEOCALGUI or raises the existing
%      singleton*.
%
%      H = VIDEOCALGUI returns the handle to a new VIDEOCALGUI or the handle to
%      the existing singleton*.
%
%      VIDEOCALGUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in VIDEOCALGUI.M with the given input arguments.
%
%      VIDEOCALGUI('Property','Value',...) creates a new VIDEOCALGUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before videoCalGUI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to videoCalGUI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help videoCalGUI

% Last Modified by GUIDE v2.5 21-Mar-2012 20:31:32

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @videoCalGUI_OpeningFcn, ...
                   'gui_OutputFcn',  @videoCalGUI_OutputFcn, ...
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


% --- Executes just before videoCalGUI is made visible.
function videoCalGUI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to videoCalGUI (see VARARGIN)

% Choose default command line output for videoCalGUI
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);
obj.vid = videoinput('winvideo', 1, 'YUY2_1280x720');
obj.src = getselectedsource(obj.vid);
set(obj.vid,'ReturnedColorSpace','rgb')
set(obj.vid, 'FramesPerTrigger', 1);
set(obj.vid, 'TriggerRepeat', Inf);
set(obj.src,'WhiteBalanceMode','manual')
set(obj.src,'ExposureMode','manual')
set(obj.src,'BacklightCompensation','off')
%set(obj.src,'FocusMode','manual')
obj.brightness = get(obj.src,'Brightness');
brightnessRange = getfield(propinfo(obj.src,'Brightness'),'ConstraintValue');
set(handles.brightnessText,'string',['Brightness = ',num2str(obj.brightness)])
set(handles.brightnessSlider,'min',brightnessRange(1));
set(handles.brightnessSlider,'max',brightnessRange(2));
set(handles.brightnessSlider,'value',obj.brightness);

obj.contrast = get(obj.src,'Contrast');
contrastRange = getfield(propinfo(obj.src,'Contrast'),'ConstraintValue');
set(handles.contrastText,'string',['Contrast = ',num2str(obj.contrast)])
set(handles.contrastSlider,'min',contrastRange(1));
set(handles.contrastSlider,'max',contrastRange(2));
set(handles.contrastSlider,'value',obj.contrast);

obj.exposure = get(obj.src,'Exposure');
exposureRange = getfield(propinfo(obj.src,'Exposure'),'ConstraintValue');
set(handles.exposureText,'string',['Exposure = ',num2str(obj.exposure)])
set(handles.exposureSlider,'min',exposureRange(1));
set(handles.exposureSlider,'max',exposureRange(2));
set(handles.exposureSlider,'value',obj.exposure);

obj.focus = get(obj.src,'Focus');
focusRange = getfield(propinfo(obj.src,'Focus'),'ConstraintValue');
set(handles.focusText,'string',['Focus = ',num2str(obj.focus)])
set(handles.focusSlider,'min',focusRange(1));
set(handles.focusSlider,'max',focusRange(2));
set(handles.focusSlider,'value',obj.focus);

obj.saturation = get(obj.src,'Saturation');
saturationRange = getfield(propinfo(obj.src,'Saturation'),'ConstraintValue');
set(handles.saturationText,'string',['Saturation = ',num2str(obj.saturation)])
set(handles.saturationSlider,'min',saturationRange(1));
set(handles.saturationSlider,'max',saturationRange(2));
set(handles.saturationSlider,'value',obj.saturation);

obj.sharpness = get(obj.src,'Sharpness');
sharpnessRange = getfield(propinfo(obj.src,'Sharpness'),'ConstraintValue');
set(handles.sharpnessText,'string',['Sharpness = ',num2str(obj.sharpness)])
set(handles.sharpnessSlider,'min',sharpnessRange(1));
set(handles.sharpnessSlider,'max',sharpnessRange(2));
set(handles.sharpnessSlider,'value',obj.sharpness);

obj.balance = get(obj.src,'WhiteBalance');
whitebalanceRange = getfield(propinfo(obj.src,'Whitebalance'),'ConstraintValue');
set(handles.whitebalanceText,'string',['White Balance = ',num2str(obj.balance)])
set(handles.whitebalanceSlider,'min',whitebalanceRange(1));
set(handles.whitebalanceSlider,'max',whitebalanceRange(2));
set(handles.whitebalanceSlider,'value',obj.balance);

obj.zoom = get(obj.src,'Zoom');
zoomRange = getfield(propinfo(obj.src,'Zoom'),'ConstraintValue');
set(handles.zoomText,'string',['Zoom = ',num2str(obj.zoom)])
set(handles.zoomSlider,'min',zoomRange(1));
set(handles.zoomSlider,'max',zoomRange(2));
set(handles.zoomSlider,'value',obj.zoom);

set(handles.vidCalGUI,'UserData',obj)
% UIWAIT makes videoCalGUI wait for user response (see UIRESUME)
% uiwait(handles.vidCalGUI);


% --- Outputs from this function are returned to the command line.
function varargout = videoCalGUI_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on slider movement.
function brightnessSlider_Callback(hObject, eventdata, handles)
% hObject    handle to brightnessSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
obj = get(handles.vidCalGUI,'UserData');
obj.brightness = get(handles.brightnessSlider,'value');
set(handles.brightnessText,'string',['Brightness = ',num2str(obj.brightness)])
stop(obj.vid)
set(obj.src,'brightness',obj.brightness)
start(obj.vid)
set(handles.vidCalGUI,'UserData',obj)

% --- Executes during object creation, after setting all properties.
function brightnessSlider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to brightnessSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on slider movement.
function contrastSlider_Callback(hObject, eventdata, handles)
% hObject    handle to contrastSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
obj = get(handles.vidCalGUI,'UserData');
obj.contrast = get(handles.contrastSlider,'value');
set(handles.contrastText,'string',['Contrast = ',num2str(obj.contrast)])
stop(obj.vid)
set(obj.src,'contrast',obj.contrast)
start(obj.vid)
set(handles.vidCalGUI,'UserData',obj)

% --- Executes during object creation, after setting all properties.
function contrastSlider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to contrastSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on slider movement.
function exposureSlider_Callback(hObject, eventdata, handles)
% hObject    handle to exposureSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
obj = get(handles.vidCalGUI,'UserData');
obj.exposure = get(handles.exposureSlider,'value');
set(handles.exposureText,'string',['Exposure = ',num2str(obj.exposure)])
stop(obj.vid)
set(obj.src,'exposure',obj.exposure)
start(obj.vid)
set(handles.vidCalGUI,'UserData',obj)

% --- Executes during object creation, after setting all properties.
function exposureSlider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to exposureSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on slider movement.
function focusSlider_Callback(hObject, eventdata, handles)
% hObject    handle to focusSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
obj = get(handles.vidCalGUI,'UserData');
obj.focus = get(handles.focusSlider,'value');
set(handles.focusText,'string',['Focus = ',num2str(obj.focus)])
stop(obj.vid)
set(obj.src,'focus',obj.focus)
start(obj.vid)
set(handles.vidCalGUI,'UserData',obj)

% --- Executes during object creation, after setting all properties.
function focusSlider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to focusSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on slider movement.
function saturationSlider_Callback(hObject, eventdata, handles)
% hObject    handle to saturationSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
obj = get(handles.vidCalGUI,'UserData');
obj.saturation = get(handles.saturationSlider,'value');
set(handles.saturationText,'string',['Saturation = ',num2str(obj.saturation)])
stop(obj.vid)
set(obj.src,'saturation',obj.saturation)
start(obj.vid)
set(handles.vidCalGUI,'UserData',obj)

% --- Executes during object creation, after setting all properties.
function saturationSlider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to saturationSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on slider movement.
function sharpnessSlider_Callback(hObject, eventdata, handles)
% hObject    handle to sharpnessSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
obj = get(handles.vidCalGUI,'UserData');
obj.sharpness = get(handles.sharpnessSlider,'value');
set(handles.sharpnessText,'string',['Sharpness = ',num2str(obj.sharpness)])
stop(obj.vid)
set(obj.src,'sharpness',obj.sharpness)
start(obj.vid)
set(handles.vidCalGUI,'UserData',obj)

% --- Executes during object creation, after setting all properties.
function sharpnessSlider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to sharpnessSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on slider movement.
function whitebalanceSlider_Callback(hObject, eventdata, handles)
% hObject    handle to whitebalanceSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider

obj = get(handles.vidCalGUI,'UserData');
obj.whitebalance = get(handles.whitebalanceSlider,'value');
set(handles.whitebalanceText,'string',['White Balance = ',num2str(obj.whitebalance)])
stop(obj.vid)
set(obj.src,'whitebalance',obj.whitebalance)
start(obj.vid)
set(handles.vidCalGUI,'UserData',obj)

% --- Executes during object creation, after setting all properties.
function whitebalanceSlider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to whitebalanceSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on slider movement.
function zoomSlider_Callback(hObject, eventdata, handles)
% hObject    handle to zoomSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
obj = get(handles.vidCalGUI,'UserData');
obj.zoom = get(handles.zoomSlider,'value');
set(handles.zoomText,'string',['Zoom = ',num2str(obj.zoom)])
stop(obj.vid)
set(obj.src,'zoom',obj.zoom)
start(obj.vid)
set(handles.vidCalGUI,'UserData',obj)

% --- Executes during object creation, after setting all properties.
function zoomSlider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to zoomSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on button press in startButton.
function startButton_Callback(hObject, eventdata, handles)
% hObject    handle to startButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
obj = get(handles.vidCalGUI,'UserData');
start(obj.vid)
while 1
    try
    frame = getdata(obj.vid);
    imshow(frame)
    end
    flushdata(obj.vid)
end
stop(obj.vid)
set(handles.vidCalGUI,'UserData',obj)


% --- Executes on button press in stopButton.
function stopButton_Callback(hObject, eventdata, handles)
% hObject    handle to stopButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
obj = get(handles.vidCalGUI,'UserData');
flushdata(obj.vid)

set(handles.vidCalGUI,'UserData',obj)

stop(obj.vid)
