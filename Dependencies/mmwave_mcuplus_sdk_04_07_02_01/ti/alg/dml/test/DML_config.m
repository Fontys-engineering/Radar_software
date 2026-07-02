% Inputs needed to run the file
% DMLtest_numAntennas - Number of samples before adding zeros
% DMLVirtualAntennaSize - Number of samples in signal after adding zeros in case of sparse array
%                       (Aperture size)
% DMLAzimFFTBins      - Number of samples after zeropadding, to perform FFT 
%                       (This decides the resolution of DML, this is the total available steering 
%                        vectors to choose from (using 'FOV' argument), which are uniformly placed 
%                        between [-pi, pi) in 'w'(omega of FFT) space)
% sparse_mask   -       Mask that says where zeros are inserted to fill sparse array
% ROI - [n1,n2] -       {n1, n2} ∈ [0, Nzeropad-1], represent the range of steering vectors 
%                        chosen to perform DML. ('0' corresponds to steering vector at w = 0 and 
%                        'Nzeropad-1' corresponds to steering vector at w = 2*pi-pi/Nzeropad, which 
%                        means that the if n1 > n2, steeing vectors of interest are n1, n1+1,...,
%                        Nzeropad-1, 0, 1,..., n2-1, n2 and if n1 < n2, then steeing vectors of 
%                        interest are n1, n1+1, ..., n2-1, n2. The steering vector '0' corresponds 
%                        to direction normal to Radar(0 deg azimuth). Steering vectors '0' - 'Nzeropad/2-2'
%                        correspond to angles '0' - 'pi/2' in radial coordinates and 'Nzeropad/2' - 'Nzeropad-1'
%                        correspond to angles '-pi/2' - '0' in radial coordinates)
% n1 -> DMLtest_n1 and n2 -> DMLtest_n2
% DML_peak          - Simulated tones are present at these bins (in range [0, DMLVirtualAntennaSize))
DMLtest_numAntennas = 12;
DMLVirtualAntennaSize = 17;
DMLAzimFFTBins        = 128;
DMLtest_n1              = 0;
DMLtest_n2              = 127;       % ROI for DML is steering vector DMLtest_n1 -> steering vector DMLtest_n2
if DMLtest_n2 > DMLtest_n1
    num_steervecs = DMLtest_n2 - DMLtest_n1 + 1;
else
    num_steervecs = DMLAzimFFTBins - DMLtest_n1 + DMLtest_n2 + 1;
end
ROI             = [DMLtest_n1, DMLtest_n2];
Amp             = [0.3 0.3];
DMLPeakLoc  = [4 5];  % Simulated tones are present at these FFT bins (in range [0, DMLVirtualAntennaSize))
if DMLtest_n2 > DMLtest_n1
    assert((DMLPeakLoc(1) > DMLtest_n1/DMLAzimFFTBins*DMLVirtualAntennaSize) && (DMLPeakLoc(1) < DMLtest_n2/DMLAzimFFTBins*DMLVirtualAntennaSize), "DMLPeakLoc 1 out of FOV")
    assert((DMLPeakLoc(2) > DMLtest_n1/DMLAzimFFTBins*DMLVirtualAntennaSize) && (DMLPeakLoc(2) < DMLtest_n2/DMLAzimFFTBins*DMLVirtualAntennaSize), "DMLPeakLoc 2 out of FOV")
else
    assert((DMLPeakLoc(1) > DMLtest_n1/DMLAzimFFTBins*DMLVirtualAntennaSize) || (DMLPeakLoc(1) < DMLtest_n2/DMLAzimFFTBins*DMLVirtualAntennaSize), "DMLPeakLoc 1 out of FOV")
    assert((DMLPeakLoc(2) > DMLtest_n1/DMLAzimFFTBins*DMLVirtualAntennaSize) || (DMLPeakLoc(2) < DMLtest_n2/DMLAzimFFTBins*DMLVirtualAntennaSize), "DMLPeakLoc 2 out of FOV")
end
PhaseOffset     = [0 pi/6];
NoisePowerdB    = -40;
final_scale     = 1;
% sparse_mask     = reshape([1 1 1 1 1 1 1 0 1 1 1 0 1 1], [DMLVirtualAntennaSize, 1]);
% sparse_mask     = reshape([1 0 1 0 1 1 0 1 1 0 0 0 1 0 1 1 1 0 1 1], [DMLVirtualAntennaSize, 1]);
% sparse_mask     = reshape([1 1 1 1 1 1 1 1 1 1 1 1], [DMLVirtualAntennaSize, 1]);
% sparse_mask     = reshape([1 0 0 1 1 1 0 1 1 1 0 1 0 1 1 0 1 1], [DMLVirtualAntennaSize, 1]);
sparse_mask     = reshape([1 0 0 1 1 1 0 1 1 1 0 1 0 1 1 1 1], [DMLVirtualAntennaSize, 1]);
invZeroInsMask = num2str(sparse_mask(:)'); % Insertion mask used to generate bij is inverse of actual azimuth antenna zero insertion mask
invZeroInsMask = invZeroInsMask(invZeroInsMask ~= ' ');
invZeroInsMaskForBij = dec2hex(bin2dec(invZeroInsMask));

zeroInsertionMask = num2str(flip(sparse_mask(:))');
zeroInsertionMask = zeroInsertionMask(zeroInsertionMask ~= ' ');
DMLzeroInsertionMask = dec2hex(bin2dec(zeroInsertionMask));
rng(0)
azim_sig = final_scale*(Amp(1)*exp(1i*(2*pi*DMLPeakLoc(1)*((0:DMLVirtualAntennaSize-1).')/DMLVirtualAntennaSize + PhaseOffset(1))) + ...
                      Amp(2)*exp(1i*(2*pi*DMLPeakLoc(2)*((0:DMLVirtualAntennaSize-1).')/DMLVirtualAntennaSize + PhaseOffset(2))) + ...
                                 sqrt(10^(NoisePowerdB/10)/2)*(randn(DMLVirtualAntennaSize, 1)+1i*randn(DMLVirtualAntennaSize, 1)));
% Quantize to 24 bits
azim_sig = azim_sig.*sparse_mask;
azim_sig = floor(azim_sig*2^23 + (0.5+1i*0.5));
% Saturation on positive and negative side (for real and imaginary parts)
azim_sig = min(max(real(azim_sig), -2^23), 2^23-1) + 1i*min(max(imag(azim_sig), -2^23), 2^23-1);
azimSig  = azim_sig;

samp_ids = 0:(DMLVirtualAntennaSize-1);

NRx = sum(sparse_mask, 'all');

if DMLtest_n1 <= DMLtest_n2
    steervec = exp(1j*2*pi/DMLAzimFFTBins*samp_ids'*(DMLtest_n1:DMLtest_n2)).*sparse_mask/NRx.^0.5;
else
    steervec = exp(1j*2*pi/DMLAzimFFTBins*samp_ids'*((DMLtest_n1-DMLAzimFFTBins):DMLtest_n2)).*sparse_mask/NRx.^0.5;
end
bij_mat = steervec'*steervec; 
bij = bij_mat(1, 2:end);
bIJ = round(-2*bij*2^14);
bIJ = min(max(real(bIJ), -2^15), 2^15-1) + 1i*min(max(imag(bIJ), -2^15), 2^15-1);

% b1ij generation

b1ij = 1./(1 - abs(bij).^2);
b1ij(abs(abs(bij) - 1) < 1e-12) = 0;

k_b1ij = ceil(log2(max(b1ij, [], 'all')));
b1IJ = round((2^(-k_b1ij))*b1ij*2^20);
b1IJ = min(max(real(b1IJ), -2^20), 2^20-1) + 1i*min(max(imag(b1IJ), -2^20), 2^20-1);
b1IJ_final = cat(1, b1IJ, b1IJ);
b1IJ_final = b1IJ_final(:);

%% Create a header file to use in DML test
fid = fopen('dml_test_gen.h', 'w');
fileHeaders = "#include <stdint.h>\n" + ...
    "#include <math.h>\n";
fprintf(fid, fileHeaders);
fprintf(fid, "\n/*Global Variables*/\n" + ...
    "uint16_t DMLtest_numAntennas = %s;\n" + ...
    "uint32_t DMLtest_n1 = %s;\n" + ...
    "uint32_t DMLtest_n2 = %s;\n" + ...
    "uint64_t DMLzeroInsertionMask = 0x%s;\n" + ...
    "uint32_t DMLVirtualAntennaSize = %s;\n" + ...
    "uint32_t DMLwindowRamOffset = 0;\n" + ...
    "uint32_t DMLVectorMultRAMOffset = 0;\n" + ...
    "uint32_t DMLAzimFFTBins = %s;\n" + ...
    "uint32_t DMLPeakLoc[2] = {%s, %s};\n" + ...
    "uint32_t DML_scale_factor_b1ij = %s;\n\n", ...
    num2str(DMLtest_numAntennas), num2str(DMLtest_n1), num2str(DMLtest_n2), DMLzeroInsertionMask, ...
    num2str(DMLVirtualAntennaSize), num2str(DMLAzimFFTBins), num2str(DMLPeakLoc(1)), num2str(DMLPeakLoc(2)), num2str(k_b1ij));

% Writing azimuth samples array in the header file
azimuthSamples = zeros(DMLtest_numAntennas, 1);
counter = 1;
for i = 1:size(sparse_mask)
    if sparse_mask(i) == 1
        azimuthSamples(counter) = azimSig(i);
        counter = counter + 1;
    end
end
azimSamples = [imag(azimuthSamples)';real(azimuthSamples)'];
% azimSamples = [imag(azimSig)'; real(azimSig)'];
azimSamples = string(num2str(reshape(azimSamples, [], 1)));
fprintf(fid, "int32_t DMLtest_azimSamples[] = {");
for i = 1:(size(azimSamples)-1)
    fprintf(fid, '\n\t%s,', azimSamples(i));
end
fprintf(fid, "\n\t%s\n};\n\n", azimSamples(i+1));

% Writing -2*bij in the header file
bij_final = [real(bIJ); imag(bIJ)];
bij_final = string(num2str(reshape(bij_final, [], 1)));
fprintf(fid, "int16_t DMLtest_bij[] = {");
for i = 1:(size(bij_final)-1)
    fprintf(fid, '\n\t%s,', bij_final(i));
end
fprintf(fid, "\n\t%s\n};\n\n", bij_final(i+1));

% Writing b1ij in the header file
b1IJ_final = [real(b1IJ_final)'; imag(b1IJ_final)'];
b1IJ_final = string(num2str(reshape(b1IJ_final, [], 1)));
fprintf(fid, "int32_t DMLtest_b1ij[] = {");
for i = 1:(size(b1IJ_final)-1)
    fprintf(fid, '\n\t%s,', b1IJ_final(i));
end
fprintf(fid, "\n\t%s\n};\n\n\n", b1IJ_final(i+1));
fclose(fid);


