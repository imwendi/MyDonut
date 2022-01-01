#ifndef DONUT_TINGS_H
#define DONUTE_TINGS_H


#define THETA_SPACING 0.07
#define PHI_SPACING 0.02

struct Donut {
    /* Angle step about x-axis */
    double A; 
    /* Angle step about z-axis */
    double B;
    int R1;
    int R2;
    int K1;
    int K2;
    int width;
    int height;
    char **outbuff;
    double **zbuff;
};
typedef struct Donut Donut;

#endif