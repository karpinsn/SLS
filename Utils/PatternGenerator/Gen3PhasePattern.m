function [ fringe ] = Gen3PhasePattern( pitch, width, height, direction)
%GEN3PHASEPATTERN Generates a packed set of 3 phase shifted patterns
%   
%   Arguments:
%       pitch - Pitch of the fringe. Number of pixels per period
%       width - Width of the fringe
%       height - Height of the fringe
%       direction - Direction of the fringe
%           0 - Corresponds to horizontal
%           1 - Corresponds to vertical

if (0 == direction)
    x = 1 : width;
else
    x = 1 : height;
end

r = (1.0 - cos((2.0 * pi) * (x / pitch) - (2.0*pi/3.0))) * .5;
g = (1.0 - cos((2.0 * pi) * (x / pitch))) * .5;
b = (1.0 - cos((2.0 * pi) * (x / pitch) + (2.0*pi/3.0))) * .5;

fringe = zeros(height, width, 3);

if(0 == direction)
    fringe(:,:,1) = ones(height, 1) * r;
    fringe(:,:,2) = ones(height, 1) * g;
    fringe(:,:,3) = ones(height, 1) * b;
else
    fringe(:,:,1) = r' * ones(1, width);
    fringe(:,:,2) = g' * ones(1, width);
    fringe(:,:,3) = b' * ones(1, width);
end

end

