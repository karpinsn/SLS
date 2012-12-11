function OptimalTwoWaveFinder (fringeSize)

lambda1s = 1:200;
lambda2s = 1:200;

error = zeros(length(lambda1s), length(lambda2s));
x = 100 : (100 + fringeSize);

sprintf('       ');
fprintf('\n f1    \tf2   \terror')
for i = 1:length(lambda1s),
    for j = 1:length(lambda2s),
        
    lambda1 = lambda1s(i);
    lambda2 = lambda2s(j);
   
    noiseAmt = 0.04;
    % Add noise with normal dist, mean 0 and standard deviation noiseAmt
    % Negative and positive numbers are added
    phi1 = getPhi(x, lambda1) + normrnd(0, noiseAmt, [1 length(x)]);
    phi2 = getPhi(x, lambda2) + normrnd(0, noiseAmt, [1 length(x)]);

    phi12 = phiAbs3(phi1, phi2, lambda1, lambda2);
    error(i,j) = rms(phi12 - x)/(950 - 100);
    
    if (error(i,j) < 0.0006),
        fprintf('\n%3d \t%3d \t%1.5f', lambda1, lambda2, error(i,j))
    end 
    end
end

min(error(:))

fprintf('\n');
error(isnan(error)) = 1000;

figure; imagesc(error, [0 0.002]);
ylabel('\lambda_1');
xlabel('\lambda_2');
title('Two Wavelength Method');
end

function lambda = lambda_eq(lambda1, lambda2)

    lambda = lambda1*lambda2 / abs(lambda1 - lambda2);

end

function phi = delta_phi(phi1, phi2, lambdaeq)

    phi = mod(phi1 - phi2, 2*pi);
    %phi = lambdaeq*phi;

end

function phi = getPhi(xs, lambda)

phi = mod(xs * 2*pi / lambda, 2*pi);
phi(phi > pi) = phi(phi > pi) - 2*pi;

end

function phi12 = phiAbs(phi1, phi2, lambda1, lambda2)
    
    lambdaEq12 = lambda_eq(lambda1, lambda2);

    phi12= delta_phi(phi1, phi2, lambdaEq12) * lambdaEq12 / (2*pi);
end

function phase1 = phiAbs2(phi1, phi2, P1, P2)
    
    P12 = lambda_eq(P1, P2);

    phase12 = mod(phi2 - phi1, 2 * pi);
    
    k1 = round((phase12 * (P12/P1) - phi1)/(2*pi));
    phase1 = (k1 *2*pi) + phi1;

end

function phase4 = phiAbs3(phi1, phi2, P1, P2)
    P12 = lambda_eq(P1, P2);
    phase12 = delta_phi(phi1, phi2, P12)*P12;
    k = round(((phase12 - phi1*P1)/P12)*P12/P1 / (2*pi));
    phase4 = P1/(2*pi)*(k*2*pi + phi1);
end

function N = getStep(phi, lambda)

    N = mod(floor(phi/lambda), 2*pi);

end