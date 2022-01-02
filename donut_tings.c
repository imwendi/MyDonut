#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "donut_tings.h"

Donut *make_donut(double theta_spacing,
                  double phi_spacing,
                  int R1,
                  int R2,
                  int K2,
                  int width,
                  int height) {
    Donut *donut = (Donut *) malloc(sizeof(Donut));
    donut->theta_spacing = theta_spacing;
    donut->phi_spacing = phi_spacing;
    donut->R1 = R1;
    donut->R2 = R2;
    donut->K1 = width*K2*3/(8*(R1+R2));
    donut->K2 = K2;
    donut->width = width;
    donut->height = height;

    donut->outbuff = (char **) calloc(width, sizeof(char *));
    donut->zbuff = (double **) calloc(width, sizeof(double *));
    for (int i = 0; i < width; ++i) {
        donut->outbuff[i] = (char *) calloc(height, sizeof(char));
        donut->zbuff[i] = (double *) calloc(height, sizeof(double));
    }

    return donut;
}

void reset_buffs(Donut *donut) {
    for (int i = 0; i < donut->width; ++i) {
        memset(donut->outbuff[i], ' ', donut->height);
        memset(donut->zbuff[i], 0, donut->height * sizeof(double));
    }
}

void calc_donut(double A, double B, Donut *donut) {
    reset_buffs(donut);

    // Precompute frequently used values
    double cosA = cos(A);
    double cosB = cos(B);
    double sinA = sin(A);
    double sinB = sin(B);

    // theta goes over circular cross-section of the torus
    for (double theta = 0; theta < 2 * M_PI; theta += donut->theta_spacing) {
        // Precompute values
        double costheta = cos(theta);
        double sintheta = sin(theta);

        // phi goes around the center of revolution of a torus
        for (double phi = 0; phi < 2 * M_PI; phi += donut->phi_spacing) {
            double cosphi = cos(phi);
            double sinphi = sin(phi);

            // x, y coords of circle before revolving
            double circlex = donut->R2 + donut->R1 * costheta;
            double circley = donut->R1 * sintheta;

            // final 3D (x, y, z) coords after rotations, after multiplying by
            // a couple of fancy 3D rotation matrices
            double x = circlex*(cosB*cosphi + sinA*sinB*sinphi) -
                    circley*cosA*sinB; 
            double y = circlex*(sinB*cosphi - sinA*cosB*sinphi) +
                    circley*cosA*cosB;
            double z = donut->K2 + cosA*circlex*sinphi + circley*sinA;
            double ooz = 1/z;  // "one over z"

            // calculate luminance in a cooked way
            float L = cosphi*costheta*sinB -
                    cosA*costheta*sinphi -
                    sinA*sintheta +
                    cosB*(cosA*sintheta - costheta*sinA*sinphi);
            // L ranges from -sqrt(2) to +sqrt(2).  If it's < 0, the surface
            // is pointing away from us, so we won't bother trying to plot it.
            if (L > 0) {
                // x and y projection - y is negative as "up" in 3D space is
                // -ve on the display
                int xp = (int) (donut->width / 2 + donut->K1 * ooz * x);
                int yp = (int) (donut->height / 2 - donut->K1 * ooz * y);

                // test against the z-buffer.  larger 1/z means the pixel is
                // closer to the viewer than what's already plotted.
                if(ooz > donut->zbuff[xp][yp]) {
                    donut->zbuff[xp][yp] = ooz;
                    int luminance_index = L*8;
                    // Get luminance
                    donut->outbuff[xp][yp] = ".,-~:;=!*#$@"[luminance_index];
                }
            }
        }
    }
}

void render_frame(Donut *donut) {
    printf("\x1b[d");
    for (int j = 0; j < donut->height; ++j) {
        for (int i = 0; i < donut->width; ++i) {
            putchar(donut->outbuff[i][j]);
        }
        putchar('\n');
    }
    printf("\x1b[d");
}
