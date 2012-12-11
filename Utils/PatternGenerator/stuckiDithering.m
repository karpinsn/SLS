function [ ditheredImage ] = stuckiDithering( image )
%STUCKIDITHERING Dithers the specified image with the stucki error
%diffusing technique
%   Arguments:
%       image - Image to dither
%
%   Returns:
%       ditheredImage - Resulting dithered image

[m, n, c] = size(image);

% Perform the actual dithering
ditheredImage = image;
for channel = 1 : c
    for row = 1 : m
        for col = 1 : n
            oldPixel = ditheredImage(row,col,channel);
            newPixel = round(oldPixel); % Will give us either 0 or 1
            ditheredImage(row,col,channel) = newPixel;
            error = oldPixel - newPixel;
            
            % Diffuse error
            if col < n
                ditheredImage(row,col+1,channel) = ditheredImage(row,col+1,channel) + (8/42 * error);
                if col + 1 < n
                    ditheredImage(row,col+2,channel) = ditheredImage(row,col+2,channel) + (4/42 * error);
                end
            end
            
            if row < m
                ditheredImage(row+1,col,channel) = ditheredImage(row+1,col,channel) + (8/42 * error);
                if col > 1
                    ditheredImage(row+1,col-1,channel) = ditheredImage(row+1,col-1,channel) + (4/42 * error);
                end
                if col > 2
                    ditheredImage(row+1,col-2,channel) = ditheredImage(row+1,col-2,channel) + (2/42 * error);
                end
                
                if col < n
                    ditheredImage(row+1,col+1,channel) = ditheredImage(row+1,col+1,channel) + (4/42 * error);
                end
                if col + 1 < n
                    ditheredImage(row+1,col+2,channel) = ditheredImage(row+1,col+2,channel) + (2/42 * error);
                end
            end
            
            if row + 1 < m
                ditheredImage(row+2,col,channel) = ditheredImage(row+2,col,channel) + (4/42 * error);
                if col > 1
                    ditheredImage(row+2,col-1,channel) = ditheredImage(row+2,col-1,channel) + (2/42 * error);
                end
                if col > 2
                    ditheredImage(row+2,col-2,channel) = ditheredImage(row+2,col-2,channel) + (1/42 * error);
                end
                
                if col < n
                    ditheredImage(row+2,col+1,channel) = ditheredImage(row+2,col+1,channel) + (2/42 * error);
                end
                if col + 1 < n
                    ditheredImage(row+2,col+2,channel) = ditheredImage(row+2,col+2,channel) + (1/42 * error);
                end
            end
        end
    end
end

