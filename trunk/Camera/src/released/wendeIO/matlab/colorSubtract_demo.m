clear
warning off
%data = imread('photo4.JPG','jpeg');
data = imread('photo4a.bmp','bmp');
% figure;
% imshow(data(:,:,1)); %Red
% figure;
% imshow(data(:,:,2)); %Green
% figure;
% imshow(data(:,:,3)); %Blue
pixelRemoval = 10;
laplacianKernel = [1];  %********TWEAK THIS MATRIX******

%**************************************************************************
%ORIGINAL CHANNELS (NORMALIZED - EDGES (10 PIXELS) WERE ALSO REMOVED B/C THEY MESSED UP NORMALIZATION)
%Original Red Channel
redFilterOutput = conv2(data(:,:,1),laplacianKernel);
indices = size(redFilterOutput);
redFilterOutput = redFilterOutput(pixelRemoval:indices(1) - pixelRemoval,pixelRemoval:indices(2) - pixelRemoval);
normal = max(abs(max(max(redFilterOutput))),abs(min(min(redFilterOutput))));
redFilterOutput = (redFilterOutput./normal);


%Original Green Channel
greenFilterOutput = conv2(data(:,:,2),laplacianKernel);
indices = size(greenFilterOutput);
greenFilterOutput = greenFilterOutput(pixelRemoval:indices(1) - pixelRemoval,pixelRemoval:indices(2) - pixelRemoval);
normal = max(abs(max(max(greenFilterOutput))),abs(min(min(greenFilterOutput))));
greenFilterOutput = (greenFilterOutput/normal);


%Original Blue Channel
blueFilterOutput = conv2(data(:,:,3),laplacianKernel);
indices = size(blueFilterOutput);
blueFilterOutput = blueFilterOutput(pixelRemoval:indices(1) - pixelRemoval,pixelRemoval:indices(2) - pixelRemoval);
normal = max(abs(max(max(blueFilterOutput))),abs(min(min(blueFilterOutput))));
blueFilterOutput = (blueFilterOutput/normal);
%**************************************************************************


%**************************************************************************
%COLOR SUBTRACTED CHANNELS
%Color Subtracted Red
filterOutput = 2*redFilterOutput - greenFilterOutput - blueFilterOutput;
filterOutput = max(filterOutput,0);
normal = max(max(filterOutput));
filterOutput = (filterOutput/normal);
figure;
subplot(2,1,1);
imshow(redFilterOutput);
subplot(2,1,2);
imshow(filterOutput);
figure;
imshow(filterOutput);

%Color Subtracted Green
filterOutput = 2*greenFilterOutput - redFilterOutput - blueFilterOutput;
filterOutput = max(filterOutput,0);
normal = max(max(filterOutput));
filterOutput = (filterOutput/normal);
figure;
subplot(2,1,1);
imshow(greenFilterOutput);
subplot(2,1,2);
imshow(filterOutput);

%Color Subtracted Blue
filterOutput = 2*blueFilterOutput - greenFilterOutput - redFilterOutput;
filterOutput = max(filterOutput,0);
normal = max(max(filterOutput));
filterOutput = (filterOutput/normal);
figure;
subplot(2,1,1);
imshow(blueFilterOutput);
subplot(2,1,2);
imshow(filterOutput);
%**************************************************************************