data = readmatrix('IOData.csv'); % or csvread if older MATLAB
u = data(:, 1); % first column is input
y = data(:, 2); % second column is output
Ts = 0.1;  
ident;
