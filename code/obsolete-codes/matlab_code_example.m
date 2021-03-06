dt = 0.10;
t_min = 0;
t_max = 100;

n_max = floor(t_max/dt);
n_min = floor(t_min/dt) + 1;

time(1:n_max) = 0;

j_min = 1;
j_max = 61;
k_min = 1;
k_max = 61;

p = 31; % middle of the area

u(j_min:j_max,k_min:k_max,n_min:n_max) = 0; % preallocate memory for and initialize u

% make an array containing a circle in the middle filled with 1s
circle(j_min:j_max,k_min:k_max) = 0;
for j = j_min:j_max
    for k = k_min:k_max
        if sqrt((31-j)^2 + (31-k)^2) <= 10
            circle(j,k) = 1;
        end
    end
end


% CALCULATE VALUES N SHIT
for n = n_min:n_max
    t = n*dt ;
    for j = j_min+1:j_max-1
        for k = k_min+1:k_max-1
            u(j,k,n+1) = ( u(j+1,k,n) + u(j-1,k,n) + u(j,k+1,n) + u(j,k-1,n) )/4;
            if circle(j,k) == 1
                u(j,k,n+1) = 0;
            end
            time(n+1) = t;
            u(j_max,1:k_max+1,n+1) = 5; % potential at boundary = 5.
            u(j_min,1:k_max+1,n+1) = -5; % potential at boundary = -5.
            u(j_min:j_max,k_min,n+1) = -5:1/6:5; % potential at boundary is a linear graph
            u(j_min:j_max,k_max,n+1) = -5:1/6:5; % potential at boundary is a linear graph
        end
        
    end
    
    % PLOTTING THE SURFACE
    if mod(n,5) == 0
        surf(j_min:j_max,k_min:k_max,u(j_min:j_max,k_min:k_max,n)) % plot a surface
        axis([2 61 2 61]) % set the x and y axis so that surf plot fills up figure
        title(['Time (s) = ',num2str(t, '%1.3f')]) % plot the function u as a surface.
        view(2); % set the viewer to two dimensions for a top down grid.
        zlim([-10,10]); % set the limits to between 0.01 and 10
       % set(gca,'ZScale','log'); % set the z-axis to be logarithmic
        xlabel('Space (j)');
        ylabel('Space (k)');
        pause(0.01) % pause for a bit after each plot so its easier to follow
    end

end

colorbar % just so its clear at the end
