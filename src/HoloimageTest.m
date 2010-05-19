
x = linspace(-1.0, 1.0, 1000);

stepHeight = .2;
P = 180;
T = P/4.5;

r = (1.0 - sin(pi * 2.0 * x)) * .5;
g = (1.0 - cos(pi * 2.0 * x)) * .5;
%b = floor(x * 1) .* stepHeight + (stepHeight/2-(stepHeight-2)/2 .* cos(2*pi*x/T));
b = ((floor(x .* 1) .* cos(2*pi*x/T)) * stepHeight) + stepHeight;

phi = atan2(r-.5, g-.5);
absPhi = phi + (2*pi*b*(1/stepHeight));

clf;
hold on;
plot(x, r, 'r');
plot(x, g, 'g');
plot(x, b, 'b');
plot(x, phi, 'black');
%plot(x, absPhi, 'magenta');
hold off;


% ------------ Decode holoimage -------------------------------------------

I = imread('Holoimage.bmp', 'bmp');
%I = imread('Holoimage.png', 'png');
imshow(I);

I = double(I);

P = 64; % Pixels per period of the projector
%P = 256; % Pixels per period of the projector
theta = pi/6;
x = 1 : 512;
y = 1 : 512;

stair = (2 * pi * floor(I(:,:,3) / 32));
imagesc(stair);

phaseA = atan2((I(:,:,1) - 255/2), (I(:,:,2) - 255/2)) + (2 * pi * floor(I(:,:,3) / 32));
%phaseA = atan2((I(:,:,1) - 255/2), (I(:,:,2) - 255/2)) + (2 * pi * floor(I(:,:,3) / 51));
imagesc(phaseA);

phaseR = x .* ((2*pi*cos(theta))/P);

z = zeros(512,512);

for row = 1 : 512
    z(row,:) = (phaseA(row,:) - phaseR) .* (P./(512*sin(theta)*2*pi)) * 255;
end

imagesc(z);
