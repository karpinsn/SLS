%% Use to find optimal pitch numbers
OptimalTwoWaveFinder(1024);

%% User defined variables
% Width and height of the fringes
width = 1024;
height = 768;

% 0 - Horizontal
% 1 - Vertical
direction = 0;

% Pitches of the patterns. Found using optimal finder above
pitch1 = 74;
pitch2 = 79;

%% Calculated numbers
pitch12 = (pitch1 * pitch2) / abs(pitch1 - pitch2);
additionalPhaseShift = 2.0 * pi * .1;
tolerance = .01;

%% Generate Ideal Patterns
fringe1 = Gen3PhasePattern(pitch1, width, height, direction);
fringe2 = Gen3PhasePattern(pitch2, width, height, direction);

%% Dither patterns
fringe1Dither = stuckiDithering(fringe1);
fringe2Dither = stuckiDithering(fringe2);

%% Output generated patterns
for channel = 1 : 3
    out = uint8(fringe1Dither(:,:,channel) * 255.0);
    imwrite(out, sprintf('sixfringe-patterns/%d-%d.png', pitch1, channel));

    out = uint8(fringe2Dither(:,:,channel) * 255.0);
    imwrite(out, sprintf('sixfringe-patterns/%d-%d.png', pitch2, channel));
end

%% Test phase unwrapping
phi1 = atan2((sqrt(3.0) .* (fringe1(:,:,1) - fringe1(:,:,3))), (2.0 .* fringe1(:,:,2)) - fringe1(:,:,1) - fringe1(:,:,3));
phi2 = atan2((sqrt(3.0) .* (fringe2(:,:,1) - fringe2(:,:,3))), (2.0 .* fringe2(:,:,2)) - fringe2(:,:,1) - fringe2(:,:,3));
phi12 = mod(phi1 - phi2, 2.0 * pi);

k = round(((phi12 * (pitch12/pitch1) - phi1) - tolerance) / (2.0 * pi));

% This phase should be unwrapped
phase = phi1 + k * 2.0 * pi;
