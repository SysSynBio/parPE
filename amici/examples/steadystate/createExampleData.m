function [ output_args ] = createExampleData(  )
    % Create data for steady_state parameter estimation example
    
    exdir = fullfile(fileparts(which('amiwrap.m')), 'examples', 'example_steadystate');
    addpath(exdir);
    % compile the model
    amiwrap('model_steadystate','model_steadystate_syms',exdir)
    
    t = linspace(0,300,20); % time
    p = [1;0.5;0.4;2;0.1]; % optimization parameters
    % k_original = [0.1,0.4,0.7,1]; % fixed parameters
    
    options = amioption('sensi',0,...
                        'maxsteps',1e4);

	numConditions = 12;
    numK = 4;
    numY = 3;
    numT = numel(t);
    sigmaY = 0.1;
    rng(0);
    hdfFile = '/home/dweindl/src/parPE/amici/examples/steadystate/data.h5';
    kAll = rand(numConditions, numK);
    
    delete(hdfFile);
    try
        h5create(hdfFile, '/data/k', size(kAll));
    end
    try
        h5create(hdfFile, '/data/ytrue', [numT, numY, numConditions]);
    end
    try
        h5create(hdfFile, '/data/ymeasured', [numT, numY, numConditions]);
    end
    
    h5write(hdfFile, '/data/k', kAll);
    h5writeatt(hdfFile, '/data/', 'ptrue', log10(p));
    h5writeatt(hdfFile, '/data/', 't', t);
    h5writeatt(hdfFile, '/data/', 'sigmay', sigmaY);

    for i = 1:numConditions
        k = kAll(i, :);
        sol = simulate_model_steadystate(t,log10(p),k,[],options);
        h5write(hdfFile, '/data/ytrue', sol.y, [1, 1, i], [numT, numY, 1]);
        h5write(hdfFile, '/data/ymeasured', sol.y * (1 + sigmaY * randn()), [1, 1, i], [numT, numY, 1]);
    end
end
