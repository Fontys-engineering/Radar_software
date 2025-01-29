%% Set parameters
amount_of_segments = 3;
chirp_duration_us = 10;

%% Determine X
x_center = amount_of_segments*chirp_duration_us

x_min = -x_center/2
x_max = x_center/2;
step_size = 1/amount_of_segments; 

% Generate x-values
x = x_min:step_size:x_max;

%% Determine Y
y = x.^3 %function to be evaluated
% Normalize values to range [0, 1]
y_min = min(y);
y_max = max(y);
y_normalized = (y - y_min) / (y_max - y_min);

y_mapped_min = 77;
y_mapped_max = 81;
y_mapped = y_normalized * (y_mapped_max - y_mapped_min) + y_mapped_min;

%% Determine piecewise approx.
num_segments = 1/step_size;
segment_edges = linspace(x_min, x_max, num_segments + 1);

piecewise_starts = zeros(1, num_segments);
piecewise_slopes = zeros(1, num_segments);

for i = 1:num_segments

    x_segment = x(x >= segment_edges(i) & x <= segment_edges(i+1));
    y_segment = y_mapped(x >= segment_edges(i) & x <= segment_edges(i+1));
    

    slope = (y_segment(end) - y_segment(1)) / (x_segment(end) - x_segment(1));
    intercept = y_segment(1) - slope * x_segment(1);

    piecewise_starts(i) = y_segment(1);
    piecewise_slopes(i) = slope;
end

y_range = y_mapped_max - y_mapped_min;
slopes_mhz_us = piecewise_slopes * (y_range) * 1e6;

%% Plot functions
figure;
hold on;

% Plot the remapped arctan(x)
plot(x, y_mapped, 'b-', 'LineWidth', 2, 'DisplayName', 'Remapped function');

% Plot the piecewise approximation
for i = 1:num_segments
    % Generate x-values for the segment
    x_segment = x(x >= segment_edges(i) & x <= segment_edges(i+1));
    
    % Calculate the piecewise line
    y_approx = piecewise_slopes(i) * (x_segment - segment_edges(i)) + piecewise_starts(i);
    
    % Plot the piecewise line
    plot(x_segment, y_approx, 'r--', 'LineWidth', 1.5, 'DisplayName', ['Piecewise Segment ' num2str(i)]);
end

% Add labels, legend, and title
xlabel('x', 'FontSize', 12);
ylabel('Value (Remapped to 77-81)', 'FontSize', 12);
title('Remapped function and Piecewise Approximation', 'FontSize', 14);
legend('show', 'Location', 'Best');
grid on;


ylim([76 81]); % Set the y-axis to span from 76 to 81

hold off;

% Display piecewise line parameters (start points and slopes)
disp('Piecewise Line Parameters:');
disp('Segment Start Points (Y-mapped to 77-81):');
disp(piecewise_starts); % Start points of each segment
disp('Segment Slopes (Ramps in MHz/μs):');
disp(slopes_mhz_us); % Slopes of each segment in MHz/μs


%% Export to csv
csvData = [piecewise_starts; piecewise_slopes; repmat( ((x_max-x_min)*step_size), 1, num_segments )];

writematrix(csvData, "nlinChirpData.csv");
