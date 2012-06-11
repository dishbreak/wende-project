function varargout = wirelessController(varargin)
% WIRELESSCONTROLLER MATLAB code for wirelessController.fig
%      WIRELESSCONTROLLER, by itself, creates a new WIRELESSCONTROLLER or raises the existing
%      singleton*.
%
%      H = WIRELESSCONTROLLER returns the handle to a new WIRELESSCONTROLLER or the handle to
%      the existing singleton*.
%
%      WIRELESSCONTROLLER('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in WIRELESSCONTROLLER.M with the given input arguments.
%
%      WIRELESSCONTROLLER('Property','Value',...) creates a new WIRELESSCONTROLLER or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before wirelessController_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to wirelessController_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help wirelessController

% Last Modified by GUIDE v2.5 03-Jun-2012 22:57:30

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
    'gui_Singleton',  gui_Singleton, ...
    'gui_OpeningFcn', @wirelessController_OpeningFcn, ...
    'gui_OutputFcn',  @wirelessController_OutputFcn, ...
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


% --- Executes just before wirelessController is made visible.
function wirelessController_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to wirelessController (see VARARGIN)

% Choose default command line output for wirelessController
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes wirelessController wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = wirelessController_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function rover1xInput_Callback(hObject, eventdata, handles)
% hObject    handle to rover1xInput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of rover1xInput as text
%        str2double(get(hObject,'String')) returns contents of rover1xInput as a double


% --- Executes during object creation, after setting all properties.
function rover1xInput_CreateFcn(hObject, eventdata, handles)
% hObject    handle to rover1xInput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function rover1yInput_Callback(hObject, eventdata, handles)
% hObject    handle to rover1yInput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of rover1yInput as text
%        str2double(get(hObject,'String')) returns contents of rover1yInput as a double


% --- Executes during object creation, after setting all properties.
function rover1yInput_CreateFcn(hObject, eventdata, handles)
% hObject    handle to rover1yInput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function laserxInput_Callback(hObject, eventdata, handles)
% hObject    handle to laserxInput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of laserxInput as text
%        str2double(get(hObject,'String')) returns contents of laserxInput as a double


% --- Executes during object creation, after setting all properties.
function laserxInput_CreateFcn(hObject, eventdata, handles)
% hObject    handle to laserxInput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function laseryInput_Callback(hObject, eventdata, handles)
% hObject    handle to laseryInput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of laseryInput as text
%        str2double(get(hObject,'String')) returns contents of laseryInput as a double


% --- Executes during object creation, after setting all properties.
function laseryInput_CreateFcn(hObject, eventdata, handles)
% hObject    handle to laseryInput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function rover2xInput_Callback(hObject, eventdata, handles)
% hObject    handle to rover2xInput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of rover2xInput as text
%        str2double(get(hObject,'String')) returns contents of rover2xInput as a double


% --- Executes during object creation, after setting all properties.
function rover2xInput_CreateFcn(hObject, eventdata, handles)
% hObject    handle to rover2xInput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function rover2yInput_Callback(hObject, eventdata, handles)
% hObject    handle to rover2yInput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of rover2yInput as text
%        str2double(get(hObject,'String')) returns contents of rover2yInput as a double


% --- Executes during object creation, after setting all properties.
function rover2yInput_CreateFcn(hObject, eventdata, handles)
% hObject    handle to rover2yInput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in laserOnCheckbox.
function laserOnCheckbox_Callback(hObject, eventdata, handles)
% hObject    handle to laserOnCheckbox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of laserOnCheckbox
laserOn = get(handles.laserOnCheckbox,'Value');
if laserOn
    set(handles.laserxInput,'Enable','on')
    set(handles.laseryInput,'Enable','on')
else
    set(handles.laserxInput,'Enable','off')
    set(handles.laseryInput,'Enable','off')
end



function picEdit_Callback(hObject, eventdata, handles)
% hObject    handle to picEdit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of picEdit as text
%        str2double(get(hObject,'String')) returns contents of picEdit as a double


% --- Executes during object creation, after setting all properties.
function picEdit_CreateFcn(hObject, eventdata, handles)
% hObject    handle to picEdit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in browseDir.
function browseDir_Callback(hObject, eventdata, handles)
% hObject    handle to browseDir (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[fileName, pathName] = uigetfile('*.png','Select Picture');
picLocation = [pathName,fileName];
set(handles.picEdit,'String',picLocation);
GUI.picture = imread(picLocation);
GUI.picSize = size(GUI.picture);
set(handles.mainPanel,'UserData',GUI);


% --- Executes on button press in applyButton.
function applyButton_Callback(hObject, eventdata, handles)
% hObject    handle to applyButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
GUI = get(handles.mainPanel,'UserData');
rover1x = str2num(get(handles.rover1xInput,'String'));
rover1y = str2num(get(handles.rover1yInput,'String'));
rover2x = str2num(get(handles.rover2xInput,'String'));
rover2y = str2num(get(handles.rover2yInput,'String'));
if get(handles.laserOnCheckbox,'Value')
    laserx = get(handles.laserxInput);
    lasery = get(handles.laseryInput);
else
    laserx = [];
    lasery = [];
end



% --- Executes on button press in showImage.
function showImage_Callback(hObject, eventdata, handles)
% hObject    handle to showImage (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of showImage
GUI = get(handles.mainPanel,'UserData');
showImage = get(handles.showImage,'Value');
if showImage
    set(handles.mainPanel,'Position',[0.8,0.769,129.2,24.692])
    set(handles.figure1,'Position',[103.8,36.154,134.6,25.462])
    set(handles.axes1,'Visible','on')
    if ~isempty(GUI) && ~isempty(GUI.picture)
        imshow(GUI.picture)
    end
else
    cla
    set(handles.axes1,'Visible','off')
    set(handles.mainPanel,'Position',[0.8,0.769,66.8,24.692])
    set(handles.figure1,'Position',[103.8,36.154,69.6,25.462])
end
    
