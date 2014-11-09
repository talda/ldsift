function [  ] = plot_detected( vertex, faces )
%UNTITLED Applies DOG detector and plots detected points
[detectedPts, scale] = dog(vertex,faces, 20);
figure(1);
plot_mesh(vertex,faces);
[x,y,z]=sphere();
hold on;
ms = 17;
%plot3(vertex(detectedPts{2},1), vertex(detectedPts{2},2), vertex(detectedPts{2},3),'g.','MarkerSize',ms);
for scale_ = 2:4
    for k=1:size(detectedPts{scale_},2)
        lscale = 2;%3*scale(detectedPts{2}(k));
        surf(lscale*x+vertex(detectedPts{scale_}(k),1),lscale*y+vertex(detectedPts{scale_}(k),2),lscale*z+vertex(detectedPts{scale_}(k),3),ones(size(x)),'FaceLighting','phong','FaceColor','interp','EdgeColor','none','FaceAlpha',1,'FaceColor','g')
    end
end

end

