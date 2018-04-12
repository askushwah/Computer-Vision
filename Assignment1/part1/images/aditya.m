noisy =imread('pirate_3_new.png');
noisy_noise =imread('skype_mark_2.png');


noisy_grey = rgb2gray(noisy);
noisy_noise_grey = rgb2gray(noisy_noise);


% imshow(noisy);
F = fftshift(fft2(noisy_grey));
F1 = fftshift(fft2(noisy_noise_grey));

S = log(1+abs(F));
S1 = log(1+abs(F1));
fm = max(S(:));


real_F = real(F);
real_F1 = real(F1);

figure(1)
meshc(S)
figure(2)
plot(S)
figure(3)
meshc(real_F)
figure(4)
plot(real_F)
figure(5)
imshow(S/fm,[]);


% figure(5)
% meshc(S1)
% figure(6)
% plot(S1)
% figure(7)
% meshc(real_F1)
% figure(8)
% plot(real_F1)

% figure(3)
% meshc(S1)
% figure(4)
% plot(S1)

% fti1 = rgb2gray(fti);
% 
% 
% figure(1)
% imshow(fti1)
% 
% figure(2)
% meshrc(fti1)