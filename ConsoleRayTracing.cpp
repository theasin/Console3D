//Console raytracing
//Originally from ArtemOnigiri, adapted for Linux

//TODO: Rework things needing <windows.h> 
//      Automatically get size of virtual terminal/framebuffer console

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include "VecFunctions.h"
#include <unistd.h>
#define clrscr() printf("\e[1;1H\e[2J")

int main(int argc, char *argv[]) {
	if (argc != 3) 
	{
		std::cout << "Syntax: ./a.out [WIDTH] [HEIGHT]" << std::endl;		
		return(1); 
	}
	clrscr();
	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	float aspect = (float)width / height;
	float pixelAspect = 11.0f / 24.0f;
	char gradient[] = " .:!/r(l1Z4H9W8$@";
	int gradientSize = std::size(gradient) - 2;

	char* screen = new char[width * height];
	unsigned int linesWritten = 0;

	for (int t = 0; t < 10000; t++) {
		vec3 light = norm(vec3(-0.5, 0.5, -1.0));
		vec3 spherePos = vec3(0, 3, 0);
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				vec2 uv = vec2(i, j) / vec2(width, height) * 2.0f - 1.0f;
				uv.x *= aspect * pixelAspect;
				vec3 ro = vec3(-6, 0, 0);
				vec3 rd = norm(vec3(2, uv));
				ro = rotateY(ro, 0.25);
				rd = rotateY(rd, 0.25);
				ro = rotateZ(ro, t * 0.01);
				rd = rotateZ(rd, t * 0.01);
				float diff = 1;
				for (int k = 0; k < 5; k++) {
					float minIt = 99999;
					vec2 intersection = sphere(ro - spherePos, rd, 1);
					vec3 n = 0;
					float albedo = 1;
					if (intersection.x > 0) {
						vec3 itPoint = ro - spherePos + rd * intersection.x;
						minIt = intersection.x;
						n = norm(itPoint);
					}
					vec3 boxN = 0;
					intersection = box(ro, rd, 1, boxN);
					if (intersection.x > 0 && intersection.x < minIt) {
						minIt = intersection.x;
						n = boxN;
					}
					intersection = plane(ro, rd, vec3(0, 0, -1), 1);
					if (intersection.x > 0 && intersection.x < minIt) {
						minIt = intersection.x;
						n = vec3(0, 0, -1);
						albedo = 0.5;
					}
					if (minIt < 99999) {
						diff *= (dot(n, light) * 0.5 + 0.5) * albedo;
						ro = ro + rd * (minIt - 0.01);
						rd = reflect(rd, n);
					}
					else break;
				}
				int color = (int)(diff * 20);
				color = clamp(color, 0, gradientSize);
				char pixel = gradient[color];
				screen[i + j * width] = pixel;
			}
		}
		screen[width * height - 1] = '\n';
		std::cout << screen;
	}
}
