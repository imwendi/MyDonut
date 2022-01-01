#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "donut_tings.h"

Donut *make_donut(int A,
                  int B,
                  int R1,
                  int R2,
                  int K2,
                  int width,
                  int height) {
    Donut *donut = (Donut *) malloc(sizeof(Donut));
    donut->A = A;
    donut->B = B;
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

void calc_donut(Donut *donut, char **outbuff, double **zbuff) {
    // Precompute frequently used values
    double cosA = cos(donut->A);
    double cosB = cos(donut->B);
    double sinA = sin(donut->A);
    double sinB = sin(donut->B);

    // theta goes over circular cross-section of the torus
    for (double theta = 0; theta < 2 * M_PI; theta += THETA_SPACING) {
        // Precompute values
        double costheta = cos(theta);
        double sintheta = sin(theta);

        // phi goes around the center of revolution of a torus
        for (double phi = 0; phi < 2 * M_PI; phi += PHI_SPACING) {
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
                // x and y projection
                // Note that y is negative as "up" in 3D space is -ve on computer
                // displays
                int xp = (int) (donut->width / 2 + donut->K1 * ooz * x);
                int yp = (int) (donut->height / 2 - donut->K1 * ooz * y);

                // test against the z-buffer.  larger 1/z means the pixel is
                // closer to the viewer than what's already plotted.
                if(ooz > zbuff[xp][yp]) {
                    zbuff[xp][yp] = ooz;
                    int luminance_index = L*8;
                    // luminance_index is now in the range 0..11 (8*sqrt(2) = 11.3)
                    // now we lookup the character corresponding to the
                    // luminance and plot it in our output:
                    outbuff[xp][yp] = ".,-~:;=!*#$@"[luminance_index];
                }
            }
        }
    }
}

void render_frame(Donut *donut, char **outbuff) {
    for (int j = 0; j < donut->height; ++j) {
        for (int i = 0; i < donut->width; ++i) {
            putchar(outbuff[i][j]);
        }
        putchar('\n');
    }
}

double deg2rad(double x) {
    return x * M_PI / 180.0;
}

int main() {
    double A = deg2rad(20);
    double B = deg2rad(30);
    Donut *donut = make_donut(A, B, 1, 2, 5, 300, 300);

}