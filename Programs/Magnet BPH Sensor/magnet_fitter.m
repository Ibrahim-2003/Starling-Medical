function magnet_fitter
clc
close all
filename = 'test8.csv';
M = csvread(filename,1,0);
mass = M(:,6);
b_field = M(:,5);

a = fminsearch(@fSSR, [10000,7000], [], mass, b_field);

[p,S] = polyfit(b_field,mass,3);
x1 = linspace(7254,7274,1000);
y1 = polyval(p,x1);
figure
plot(b_field,mass,'.','LineWidth',100)
hold on
plot(x1,y1,'MarkerSize',20)
hold off
book_plot()

preds = polyval(p,b_field);
R_squared = 1 - (S.normr/norm(mass - mean(mass)))^2

end

function f = fSSR(a,m,H)
gamma = 12*6894.76/(1000^2);
z = 9180.*m./(gamma*25*pi*(1+2*1.25^2));
yp = a(1)*((z+2)./sqrt((z+2).^2+25) - (z)./sqrt(z.^2+25)) + a(2);
f = sum((H-yp).^2);
end

function book_plot()
grid on;
background = [212,238,252] / 256;
grid_color = [150,153,156] / 256;
set(gca, 'Color','w', 'XColor','r', 'YColor','r','GridColor',grid_color,...
    'GridAlpha',0.9)
set(gcf, 'Color',background)
end

