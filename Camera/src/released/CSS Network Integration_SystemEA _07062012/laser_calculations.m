
height = 1500;
toline = 300;
radiusSm = 700;
radiusLg = 2000;

angle(1) = atan(toline/height)
angle(2) = atan((toline+radiusLg-radiusSm)/height)
angle(3) = atan((toline+radiusLg)/height)
angle(4) = atan((toline+radiusLg+radiusSm)/height)
angle(5) = atan((toline+2*radiusLg)/height)

factor = 20.6
dAngle = rad2deg(angle)
pwm = factor .* dAngle


az = 3085;
el = 3340;
azMin = 2350;
azMax = 3850;
elMin = 2300;
elMax = 3625;

%(azMax-azMin)
(elMax-elMin)/(dAngle(5) - dAngle(1))