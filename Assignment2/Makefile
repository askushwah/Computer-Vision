all: CImg.h a2.cpp
	g++ a2.cpp -w -Wextra -o a2 -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -lpthread -I. -Isiftpp -O3 sift.cpp -l armadillo

debug: CImg.h a2.cpp
	g++ a2.cpp -w -Wextra -g -o a2 -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -lpthread -I. -Isiftpp -O3 sift.cpp -l armadillo

clean:
	rm a2

# all: watermark 

# watermark: watermark.cpp
# 	g++ watermark.cpp -O3 -o watermark -lpng -I.

# debug: watermark.cpp
# 	g++ -g watermark.cpp -o watermark_d -O0 -lpng -I.


# clang: watermark.cpp
# 	clang++ watermark.cpp -O3 -lpng -I.

# clean:
# 	rm watermark; rm watermark_d; 
