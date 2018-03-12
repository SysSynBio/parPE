function [ output_args ] = createExampleData(  )
    % Create data for steadystate-model parameter estimation example
    
    cd(fileparts(mfilename('fullpath')));
    hdfFile = 'data.h5';
    
    % add example directory to path
    exdir = fullfile(fileparts(which('amiwrap.m')), 'examples', 'example_steadystate');
    addpath(exdir);
    
    % compile the model
    amiwrap('model_steadystate', 'model_steadystate_syms', exdir);
    
    %% model & data dimensions
    model = model_steadystate_syms;
    numConditions = 12;
    sigmaY = 0.1; % measurement standard deviation
    t = linspace(0,100,20); % timepoints
    t = [t, inf];
    p = [1;0.5;0.4;2;0.1]; % optimization parameters
    assert(numel(p) == numel(model.sym.p));
    k_original = [0.1,0.4,0.7,1]; % fixed parameters
    
    options = amioption('sensi', 0, ...
                        'maxsteps', 1e4, ...
                        'pscale', 'log10');

    numK = numel(model.sym.k); % number of constant parameters
    numP = numel(model.sym.p); % number of variable parameters
    numY = numel(model.sym.y); % number of observables
    numT = numel(t); % number of timepoints
    
    rng(0);
    kAll = repmat(k_original,numConditions,1);
    
    %% write data
    delete(hdfFile);
    try
        h5create(hdfFile, '/fixedParameters/k', size(kAll));
    end
    try
        h5create(hdfFile, '/measurements/ytrue', [numT, numY, numConditions]);
    end
    try
        h5create(hdfFile, '/measurements/llhtrue', [numConditions]);
    end
    try
        h5create(hdfFile, '/measurements/y', [numT, numY, numConditions]);
    end
    try
        h5create(hdfFile, '/measurements/ysigma', [numT, numY, numConditions]);
    end
    
    fid = H5F.open(hdfFile, 'H5F_ACC_RDWR', 'H5P_DEFAULT');
    gid = H5G.create(fid, '/parameters', 'H5P_DEFAULT', 'H5P_DEFAULT', 'H5P_DEFAULT');
    H5G.close(gid);
    H5F.close(fid);
    
    h5create(hdfFile, '/parameters/parameterNames', numel(model.sym.p)); 
    h5create(hdfFile, '/parameters/simulationParameterNames', numel(model.sym.p)); 
    %h5rwrite(hdfFile, '/parameters/parameterNames', model.sym.p); TODO: to
    %string
    
    h5write(hdfFile, '/fixedParameters/k', kAll);
    h5writeatt(hdfFile, '/parameters/', 'ptrue', log10(p));
    h5writeatt(hdfFile, '/parameters/', 't', t);
    h5write(hdfFile, '/measurements/ysigma', sigmaY * ones(numT, numY, numConditions));

    for i = 1:numConditions
        k = kAll(i, :);
        sol = simulate_model_steadystate(t, p, k, [], options);
        h5write(hdfFile, '/measurements/ytrue', sol.y, [1, 1, i], [numT, numY, 1]);
        h5write(hdfFile, '/measurements/y', sol.y, [1, 1, i], [numT, numY, 1]);
        data = amidata(numT, numY, 0, 0, 0);
        data.Y = sol.y;
        sol = simulate_model_steadystate(t, p, k, data, options);
        h5write(hdfFile, '/measurements/llhtrue', sol.llh, [i], [1]);
        %h5write(hdfFile, '/measurements/y', sol.y * (1 + sigmaY * randn()), [1, 1, i], [numT, numY, 1]);
    end
    
    llh = h5read(hdfFile, '/measurements/llhtrue');
    h5writeatt(hdfFile, '/measurements/llhtrue', 'total', sum(llh));
    
    %% set bounds
    h5create(hdfFile, '/parameters/lowerBound', [numP])
    h5write(hdfFile, '/parameters/lowerBound', -4 * ones(numP, 1))
    h5create(hdfFile, '/parameters/upperBound', [numel(p)])
    h5write(hdfFile, '/parameters/upperBound', 4 * ones(numP, 1))
    
    %% set optimization options
    % create group
    fid = H5F.open(hdfFile, 'H5F_ACC_RDWR', 'H5P_DEFAULT');
    gid = H5G.create(fid, '/optimizationOptions', 'H5P_DEFAULT', 'H5P_DEFAULT', 'H5P_DEFAULT');
    H5G.close(gid);
    H5F.close(fid);

    h5writeatt(hdfFile, '/optimizationOptions/', 'optimizer', 0);
    h5writeatt(hdfFile, '/optimizationOptions/', 'maxIter', 20);

end

