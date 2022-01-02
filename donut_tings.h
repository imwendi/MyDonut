#ifndef DONUT_TINGS_H
#define DONUTE_TINGS_H

struct Donut {
    double theta_spacing;
    double phi_spacing;
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

Donut *make_donut(double theta_spacing,
                  double phi_spacing,
                  int R1,
                  int R2,
                  int K2,
                  int width,
                  int height);
void calc_donut(double A, double B, Donut *donut);
void render_frame(Donut *donut);


#endif