noisy =imread('h.png');
% noisyim = imread('oI.png');

F =fft2(noisy);

S = log(abs(fftshift(F)));
fm = max(S(:));

figure(2)
imshow(noisy);

figure(1)
imshow(S/fm,[]);

% figure(3)
% meshc(noisy)

figure(4)
surf(noisy)
% improfile(noisy)


% real1 = real(F);
% 
% figure(2);
% meshc(real1,[]);

% figure(1)
% imshow(log2(abs(fftshift(F))),[]);
% meshc(log2(abs(fftshift(F))),[])

% meshc(noisyim)
% meshc(noisy)

% [X,Y] = meshgrid(1:256,1:256)
% 
% figure(2)
% plot(X,Y)

% imag1 = imag(F);
% 
% figure(3)
% plot(real1)

% for r = 1:512
%     for c = 1:512
%         if real1(r,c) > 9.100e+05 && real1(r,c) < 9.500e+05
%            fprintf('%i\t%i\n', r, c);
%            F(r,c) = 0;
%         end
%     end
% end
% 
% figure(4)
% plot(real1)
% 
% imshow(log2(abs(fftshift(F))),[]);
% 
% B = ifft2(ifftshift(F));
% 
% figure(5)
% imshow(B,[])
% 
% figure(6)
% meshc(B)
% 
% S = log(1+abs(F));
% M = log(1+abs(B));
% fm = max(S(:));
% fb =  max(S(:));


% imshow(S/fm);

% imshow(B/fb);
% 
% % plot(S/fm)
% imshow(B)
% [x,y] = meshgrid(-128:127,-128:127);

