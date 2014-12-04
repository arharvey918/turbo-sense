% Elijah J. Morgan
% Dec. 4 2014
% This program was written to plot each of the 6 directions that the sensor
% module reports to the control module in their own graph.
% 
% How to use
% 1. Place this file in the directory where you wish to have the png graphs
% saved.
% 2. Change the matlab direcotry to the same directory where you have the
% files placed.
% 3. Run program, it will prompt you to enter the name of the file which
% has the data you wish to plot.
% 4. THe program will creat 6 plots of the data, it will automatically save
% them the working directory with the names U, D, L, R, F and B.
% 5. If you wish to plot multiple sets of data be sure to move or rename
% the plots from the previous set of data, otherwise you will overwrite
% that data.



close all
clc
clear all

prompt = 'Enter Name of FIle:' ;
 x = input(prompt,'s') ;



fileID = fopen(x,'r');

A = readtable(x,'Delimiter','tab','format','%d%d%d%d%d%d');

fclose(fileID);


xmax = height(A) ;
mins1 = min(A.U) ; maxs1 = max(A.U) ;
mins2 = min(A.D) ; maxs2 = max(A.D) ;
mins3 = min(A.L) ; maxs3 = max(A.L) ;
mins4 = min(A.R) ; maxs4 = max(A.R) ;
mins5 = min(A.F) ; maxs5 = max(A.F) ;
mins6 = min(A.B) ; maxs6 = max(A.B) ;



labely = 'Detected Distance (cm)';
labelx = 'Time (ms)' ;

figure 

scatter(1:xmax,A.U), grid on
axis([0 xmax mins1-50 maxs1+50])
xlabel(labelx);
ylabel(labely);
title('U')
set(gcf,'PaperPositionMode','auto')
print('-dpng','-r0','U')

figure

scatter(1:xmax,A.D), grid on
axis([0 xmax mins2-50 maxs2+50])
xlabel(labelx);
ylabel(labely);
title('Down')
set(gcf,'PaperPositionMode','auto')
print('-dpng','-r0','D')

figure

scatter(1:xmax,A.L), grid on
axis([0 xmax mins3-50 maxs3+50])
xlabel(labelx);
ylabel(labely);
title('Left')
set(gcf,'PaperPositionMode','auto')
print('-dpng','-r0','L')

figure
scatter(1:xmax,A.R), grid on
axis([0 xmax mins4-50 maxs4+50])
xlabel(labelx);
ylabel(labely);
title('Right')
set(gcf,'PaperPositionMode','auto')
print('-dpng','-r0','R')

figure
scatter(1:xmax,A.F), grid on
axis([0 xmax mins5-50 maxs5+50])
xlabel(labelx);
ylabel(labely);
title('Front')
set(gcf,'PaperPositionMode','auto')
print('-dpng','-r0','F')

figure 
scatter(1:xmax,A.B), grid on
axis([0 xmax mins6-50 maxs6+50])
xlabel(labelx);
ylabel(labely);
title('Back')
set(gcf,'PaperPositionMode','auto')
print('-dpng','-r0','B')

