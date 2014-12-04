% Elijah J. Morgan
% Dec. 4 2014
% This program was written to plot the prism of clear space that the 
% sensor module reports in a visual way.
% 
% How to use
% Step 1. Place this file in the directory where you wish the video to be
% created.Change your matlab directory to that directory.
% 2. Place the file you wish to graph into the same directory.
% run the program, when prompted enter the name of the file you wish to
% open. 
% 3. The name of the .avi file is hardcoded to be "prism1". If you wish
% to create videos of multiple file you will need to rename each .avi as
% soon as you make it, otherwise it will be overwritten by the next file
% you plot.





close all
clc
clear all



prompt = 'Enter Name of FIle:' ;
 x = input(prompt,'s') ;

fileID = fopen(x,'r');

A = readtable(x,'Delimiter','tab');

fclose(fileID);
 
n = height(A) ;
centerX = 0 ; centerY = 0; centerZ = 0 ;

writerObj = VideoWriter('prism1') ;

open(writerObj); 




figure('Position',[150 10 1000 1000])
x = 1 ;
for c=1:n 
    
    plot3dprism(centerX,centerY,centerZ,A.U(x),A.D(x),A.L(x),A.R(x),A.F(x),A.B(x))
    frame = getframe(gcf);
    writeVideo(writerObj,frame);
    x = x + 1 ;
    clf
end

close(writerObj);