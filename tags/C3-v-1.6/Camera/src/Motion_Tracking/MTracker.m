classdef MTracker
    properties
    end
    methods    
        function T = RosinThreshold(MTracker,imhist, picknonempty)
            if (nargin < 2)
                picknonempty = 0;
            end
            % find best threshold
            [mmax2, mpos] = max(imhist);
            p1 = [mpos, mmax2];
            % find last non-empty bin
            L = length(imhist);
            lastbin = mpos;
            for i = mpos:L
                if (imhist(i) > 0)
                    lastbin=i;
                end
            end
            p2 = [lastbin, imhist(lastbin)];
            DD = sqrt((p2(1)-p1(1))^2 + (p2(2)-p1(2))^2);
            if (DD ~= 0)
                best = -1;
                found = -1;
                for i = mpos:lastbin
                    p0 = [i,  imhist(i)];
                    d = abs((p2(1)-p1(1))*(p1(2)-p0(2)) - (p1(1)-p0(1))*(p2(2)-p1(2)));
                    d = d / DD;

                    if ((d > best) && ((imhist(i)>0) || (picknonempty==0)))
                        best=d;
                        found = i;
                    end
                end

                if (found == -1)
                    found = lastbin+1;
                end
            else
                found = lastbin+1;
            end
            T = min(found, L);
        end
        function d = shownormimage(MTracker,data)
            d = data - min(min(min(data)));
            if (max(max(max(d))) ~= 0)
                d = d / max(max(max(d)));
            end
            if (size(d,3) > 1)
                imshow(d);
            else
                imshow(d*64);
            end
        end
        function data = makelinear(MTracker,im)
            data = zeros(numel(im),1);
            data(:) = im(:);
        end
        function T = otsuThreshold(MTracker,h)
            L = length(h);
            i = 1:L;
            A = cumsum(h);
            B = cumsum(h.*i);
            u = B ./ A;
            tmp = (A(L) - A);
            v = (B(L) - B) ./ (tmp + (tmp==0));
            F = A .* (A(L) - A) .* (u-v).^2;
            [mF, T] = max(F);
        end
        function [T,F,E,A] = kapurthreshold(MTracker,imhist)
            if (size(imhist,1) ~= 1)
                imhist = imhist';
            end
            if (size(imhist,1) ~= 1)
                error('imhist should be a 1xL vector');
            end
            L = length(imhist);
            i = 0:(L-1);
            A = cumsum(imhist);
            E = cumsum(imhist .* log(imhist+(imhist==0)));
            A = A + (A==0);
            L2 = L-1;
            F = zeros(1,L);
            tmp = (A(L) - A(1:L2));
            tmp=tmp + (tmp==0);
            F(2:L) = (E(1:L2) ./ A(1:L2)) - log(A(1:L2)) + (E(L)-E(1:L2)) ./ tmp - log(tmp);
            F(1) = 0;
            [tmp, T] = min(F);
        end
    end
end



