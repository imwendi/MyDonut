#define _USE_MATH_DEFINES
#include <unistd.h>
#include <math.h>
#include "donut_tings.h"

#define EVER ;;
#define DEG2RAD(x) x*360.0/M_PI/2

void animate_donut(double A_rate, double B_rate, Donut *donut);

int main() {
    double A_rate = 0.0005;
    double B_rate = 0.0002;

    double theta_spacing = 0.07;
    double phi_spacing = 0.02;
    double R1 = 1;
    double R2 = 2;
    double K2 = 5;
    int width = 60;
    int height = 60;

    Donut *donut = make_donut(theta_spacing,
                              phi_spacing,
                              R1,
                              R2,
                              K2,
                              width,
                              height);

    animate_donut(A_rate, B_rate, donut);

    return -1;
}

void animate_donut(double A_rate, double B_rate, Donut *donut) {
    double A = 0;
    double B = 0;

    for (EVER) {
        calc_donut(A, B, donut);
        render_frame(donut);

        A = A > 2*M_PI ? 0 : (A + DEG2RAD(A_rate));
        B = B > 2*M_PI ? 0 : (B + DEG2RAD(B_rate));
        usleep(10000);
    }
}