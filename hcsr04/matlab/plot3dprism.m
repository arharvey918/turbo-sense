function plot3dprism(centerX,centerY,centerZ,up,down,left,right,front,back)
% this function plots a 3d prism based of 6 points, it does not plot the
% bottom side of the prism since it is not visible and has same dimensions 
% as the top.
% Detailed explanation goes here

% centerX,centerY,centerZ are the drone position
% up,down, left, right, front, back are the six values you output to the control team
% now I need to define a rectangular prism using those six values
% I will find the 8 corners



p1 = [centerX-back centerY-left centerZ-down]  ; % back-left-down
p2 = [centerX-back centerY+right centerZ-down] ; % back+right-down
p3 = [centerX-back centerY+right centerZ+up] ; % back+right+up
p4 = [centerX-back centerY-left centerZ+up] ; % back-left+up
p5 = [centerX+front centerY-left centerZ+up]; % front-left+up
p6 = [centerX+front centerY-left centerZ-down]; % front-left-down
p7 = [centerX+front centerY+right centerZ-down]; % front+right-down
p8 = [centerX+front centerY+right centerZ+up] ; % front+right+up


% back
x  = [p1(1) p2(1) p3(1) p4(1)] ; 
y  = [p1(2) p2(2) p3(2) p4(2)] ;
z  = [p1(3) p2(3) p3(3) p4(3)] ;
%front
x1 = [p5(1) p6(1) p7(1) p8(1)] ;
y1 = [p5(2) p6(2) p7(2) p8(2)] ; 
z1 = [p5(3) p6(3) p7(3) p8(3)] ;
%left
x2 = [p1(1) p4(1) p5(1) p6(1)] ;
y2 = [p1(2) p4(2) p5(2) p6(2)] ; 
z2 = [p1(3) p4(3) p5(3) p6(3)] ;
% right
x3 = [p2(1) p3(1) p8(1) p7(1)] ;
y3 = [p2(2) p3(2) p8(2) p7(2)] ; 
z3 = [p2(3) p3(3) p8(3) p7(3)] ;
% up
x4 = [p3(1) p8(1) p5(1) p4(1)] ;
y4 = [p3(2) p8(2) p5(2) p4(2)] ; 
z4 = [p3(3) p8(3) p5(3) p4(3)] ;



subplot(2,2,1)
fill3(x,y,z,'b'), hold on %plot back
axis([-450 450 -450 450 -450 450])
grid on

subplot(2,2,1)
fill3(x1,y1,z1,'b') %plot front
axis([-450 450 -450 450 -450 450])
grid on

subplot(2,2,1)
fill3(x2,y2,z2,'g') %plot left
axis([-450 450 -450 450 -450 450])
grid on

subplot(2,2,1)
fill3(x3,y3,z3,'g') %plot right
axis([-450 450 -450 450 -450 450])
grid on

subplot(2,2,1)
fill3(x4,y4,z4,'r') %plot up
axis([-450 450 -450 450])
grid on


subplot(2,2,2)
fill(x4,y4,'r') %plot up 2d
xlabel('<----- BACK FRONT ------>')
ylabel('<----- LEFT   RIGHT ------>')
title('top')
axis([-450 450 -450 450])
grid on

subplot(2,2,3) %plot front 2d
fill(y1,z1,'b') 
ylabel('<----- DOWN   UP ------>')
xlabel('<----- LEFT   RIGHT ------>')
title('front')
axis([-450 450 -450 450])
grid on

subplot(2,2,4)
fill(x3,z3,'g') %plot right 2d
xlabel('<----- BACK FRONT ------>')
ylabel('<----- DOWN      UP ------>')
title('side')
axis([-450 450 -450 450])
grid on



% plot 
hold off



end