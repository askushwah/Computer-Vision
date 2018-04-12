all: watermark 

watermark: watermark.cpp
	g++ watermark.cpp -O3 -o watermark -lpng -I.

debug: watermark.cpp
	g++ -g watermark.cpp -o watermark_d -O0 -lpng -I.

clang: watermark.cpp
	clang++ watermark.cpp -O3 -lpng -I.

clean:
	rm watermark; rm watermark_d; 
