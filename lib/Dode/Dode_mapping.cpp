#include <iostream>
#include <math.h>
#include <stdlib.h>

# define my_pi           3.14159265358979323846

int main() {
    const double a = 1; // Kantenlänge
    const double d = a/2*(1+sqrt(5)); // virtuelle Kantenlänge für Zwischenebenen
    const double alpha = my_pi*2/5;
    const double beta = my_pi*3/10;
    //const double gamma = my_pi*3/5; // unused
    const double radius = a/4*sqrt(18+6*sqrt(5));

    const double y_b1 = a/(2*cos(beta));
    const double y_b2 = a*cos(alpha)/(2*cos(beta));
    const double y_b3 = tan(beta)*a/2;
    const double x_b2 = a*sin(alpha)/(2*cos(beta));

    const double theta_t = asin(y_b1/radius);
    const double z_t = radius*cos(theta_t); 
    const double z_b = -z_t;
    
    const double y_l1 = d/(2*cos(beta));
    const double y_l2 = y_l1*cos(alpha);
    const double y_l3 = d/2*tan(beta);
    const double x_l2 = y_l1*sin(alpha);
    //double x_l3 = d/2; // unused

    const double theta_h = asin(y_l1/radius); 
    const double z_h = radius*cos(theta_h);
    const double z_l = -z_h;

    //Eckkoordinaten Baseplate B
    const double B1[3] = {0, y_b1, z_b}; 
    const double B2[3] = {x_b2, y_b2, z_b};
    const double B2_[3] = {-x_b2, y_b2, z_b};
    const double B3[3] = {a/2, -y_b3, z_b};
    const double B3_[3] = {-a/2, -y_b3, z_b};

    //Eckkoordinaten Topplate T
    const double T1[3] = {0, -y_b1, z_t}; 
    const double T2[3] = {x_b2, -y_b2, z_t};
    const double T2_[3] = {-x_b2, -y_b2, z_t};
    const double T3[3] = {a/2, y_b3, z_t};
    const double T3_[3] = {-a/2, y_b3, z_t};

    //Zwischenebene H (Higher/Upper plane)
    const double H1[3] = {0, -y_l1, z_h}; 
    const double H2[3] = {x_l2, -y_l2, z_h};
    const double H2_[3] = {-x_l2, -y_l2, z_h};
    const double H3[3] = {d/2, y_l3, z_h};
    const double H3_[3] = {-d/2, y_l3, z_h};

    //Zwischenebene L (Lower plane)
    const double L1[3] = {0, y_l1, z_l}; 
    const double L2[3] = {x_l2, y_l2, z_l};
    const double L2_[3] = {-x_l2, y_l2, z_l};
    const double L3[3] = {d/2, -y_l3, z_l};
    const double L3_[3] = {-d/2, -y_l3, z_l};

    // Längen (Anzhal LEDs) der Kanten #1 bis #30
    const int edge_LEDs [30] = { 84 , 80 , 85, 84 , 77 , 82 , 81, 78, 82, 83, 
    78, 83, 79, 79, 81, 84, 82, 76, 83, 84, 
    79, 82, 84, 78, 81, 85, 79, 81, 83, 81 };
    
    // Einzel-Vektoren der LED-Abstände
    double Vektor[30][3]={
    // Kante1: B1 --> B2_
    {(B2_[0]-B1[0])/edge_LEDs[0], (B2_[1]-B1[1])/edge_LEDs[0], (B2_[2]-B1[2])/edge_LEDs[0]},
    // Kante2: B2_ --> L2_
    {(L2_[0]-B2_[0])/edge_LEDs[1], (L2_[1]-B2_[1])/edge_LEDs[1], (L2_[2]-B2_[2])/edge_LEDs[1]},
    // Kante3: L2_ --> H3_
    {(H3_[0]-L2_[0])/edge_LEDs[2], (H3_[1]-L2_[1])/edge_LEDs[2], (H3_[2]-L2_[2])/edge_LEDs[2]},
    // Kante4: B2_ --> B3_
    {(B3_[0]-B2_[0])/edge_LEDs[3], (B3_[1]-B2_[1])/edge_LEDs[3], (B3_[2]-B2_[2])/edge_LEDs[3]},
    // Kante5: B3_ --> L3_
    {(L3_[0]-B3_[0])/edge_LEDs[4], (L3_[1]-B3_[1])/edge_LEDs[4], (L3_[2]-B3_[2])/edge_LEDs[4]},
    // Kante6: L3_ --> H2_
    {(H2_[0]-L3_[0])/edge_LEDs[5], (H2_[1]-L3_[1])/edge_LEDs[5], (H2_[2]-L3_[2])/edge_LEDs[5]},
    // Kante7: B3_ --> B3
    {(B3[0]-B3_[0])/edge_LEDs[6], (B3[1]-B3_[1])/edge_LEDs[6], (B3[2]-B3_[2])/edge_LEDs[6]},
    // Kante8: B3 --> L3
    {(L3[0]-B3[0])/edge_LEDs[7], (L3[1]-B3[1])/edge_LEDs[7], (L3[2]-B3[2])/edge_LEDs[7]},
    // Kante9: L3 --> H1
    {(H1[0]-L3[0])/edge_LEDs[8], (H1[1]-L3[1])/edge_LEDs[8], (H1[2]-L3[2])/edge_LEDs[8]},
    // Kante10: B3 --> B2
    {(B2[0]-B3[0])/edge_LEDs[9], (B2[1]-B3[1])/edge_LEDs[9], (B2[2]-B3[2])/edge_LEDs[9]},
    // Kante11: B2 --> L2
    {(L2[0]-B2[0])/edge_LEDs[10], (L2[1]-B2[1])/edge_LEDs[10], (L2[2]-B2[2])/edge_LEDs[10]},
    // Kante12: L2 --> H2
    {(H2[0]-L2[0])/edge_LEDs[11], (H2[1]-L2[1])/edge_LEDs[11], (H2[2]-L2[2])/edge_LEDs[11]},
    // Kante13: B2 --> B1
    {(B1[0]-B2[0])/edge_LEDs[12], (B1[1]-B2[1])/edge_LEDs[12], (B1[2]-B2[2])/edge_LEDs[12]},
    // Kante14: B3 --> L3
    {(L3[0]-B3[0])/edge_LEDs[13], (L3[1]-B3[1])/edge_LEDs[13], (L3[2]-B3[2])/edge_LEDs[13]},
    // Kante15: L1 --> H3
    {(H3[0]-L1[0])/edge_LEDs[14], (H3[1]-L1[1])/edge_LEDs[14], (H3[2]-L1[2])/edge_LEDs[14]},
    // Kante16: L2_ --> H2_
    {(H2_[0]-L2_[0])/edge_LEDs[15], (H2_[1]-L2_[1])/edge_LEDs[15], (H2_[2]-L2_[2])/edge_LEDs[15]},
    // Kante17: H2_ --> T2_
    {(T2_[0]-H2_[0])/edge_LEDs[16], (T2_[1]-H2_[1])/edge_LEDs[16], (T2_[2]-H2_[2])/edge_LEDs[16]},
    // Kante18: T2_ --> T3_
    {(T3_[0]-T2_[0])/edge_LEDs[17], (T3_[1]-T2_[1])/edge_LEDs[17], (T3_[2]-T2_[2])/edge_LEDs[17]},
    // Kante19: L3_ --> H1
    {(H1[0]-L3_[0])/edge_LEDs[18], (H1[1]-L3_[1])/edge_LEDs[18], (H1[2]-L3_[2])/edge_LEDs[18]},
    // Kante20: H1 --> T1
    {(T1[0]-H1[0])/edge_LEDs[19], (T1[1]-H1[1])/edge_LEDs[19], (T1[2]-H1[2])/edge_LEDs[19]},
    // Kante21: T1 --> T2_
    {(T2_[0]-T1[0])/edge_LEDs[20], (T2_[1]-T1[1])/edge_LEDs[20], (T2_[2]-T1[2])/edge_LEDs[20]},
    // Kante22: L3 --> H2
    {(H2[0]-L3[0])/edge_LEDs[21], (H2[1]-L3[1])/edge_LEDs[21], (H2[2]-L3[2])/edge_LEDs[21]},
    // Kante23: H2 --> T2
    {(T2[0]-H2[0])/edge_LEDs[22], (T2[1]-H2[1])/edge_LEDs[22], (T2[2]-H2[2])/edge_LEDs[22]},
    // Kante24: T2 --> T1
    {(T1[0]-T2[0])/edge_LEDs[23], (T1[1]-T2[1])/edge_LEDs[23], (T2[2]-T1[2])/edge_LEDs[23]},
    // Kante25: L2 --> H3
    {(H3[0]-L2[0])/edge_LEDs[24], (H3[1]-L2[1])/edge_LEDs[24], (H3[2]-L2[2])/edge_LEDs[24]},
    // Kante26: H3 --> T3
    {(T3[0]-H3[0])/edge_LEDs[25], (T3[1]-H3[1])/edge_LEDs[25], (T3[2]-H3[2])/edge_LEDs[25]},
    // Kante27: T3 --> T2
    {(T2[0]-T3[0])/edge_LEDs[26], (T2[1]-T3[1])/edge_LEDs[26], (T2[2]-T3[2])/edge_LEDs[26]},
    // Kante28: L1 --> H3_
    {(H3_[0]-L1[0])/edge_LEDs[27], (H3_[1]-L1[1])/edge_LEDs[27], (H3_[2]-L1[2])/edge_LEDs[27]},
    // Kante29: H3_ --> T3_
    {(T3_[0]-H3_[0])/edge_LEDs[28], (T3_[1]-H3_[1])/edge_LEDs[28], (T3_[2]-H3_[2])/edge_LEDs[28]},
    // Kante30: T3_ --> T3
    {(T3[0]-T3_[0])/edge_LEDs[29], (T3[1]-T3_[1])/edge_LEDs[29], (T3[2]-T3_[2])/edge_LEDs[29]}};

    // Urpsrung der Vektoren
    double Ursprung[][3] = {
        {B1[0],B1[1],B1[2]}, {B2[0],B2[1],B2[2]}, {L2_[0],L2_[1],L2_[2]}, {B2_[0],B2_[1],B2_[2]}, {B3_[0],B3_[1],B3_[2]},
        {L3_[0],L3_[1],L3_[2]}, {B3_[0],B3_[1],B3_[2]}, {B3[0],B3[1],B3[2]}, {L3[0],L3[1],L3[2]}, {B3[0],B3[1],B3[2]},
        {B2[0],B2[1],B2[2]}, {L2[0],L2[1],L2[2]}, {B2[0],B2[1],B2[2]}, {B3[0],B3[1],B3[2]}, {L1[0],L1[1],L1[2]},
        {L2_[0],L2_[1],L2_[2]}, {H2_[0],H2_[1],H2_[2]}, {T2_[0],T2_[1],T2_[2]}, {L3_[0],L3_[1],L3_[2]}, {H1[0],H1[1],H1[2]}, 
        {T1[0],T1[1],T1[2]}, {L3[0],L3[1],L3[2]}, {H2[0],H2[1],H2[2]}, {T2[0],T2[1],T2[2]}, {L2[0],L2[1],L2[2]},
        {H3[0],H3[1],H3[2]}, {T3[0],T3[1],T3[2]}, {L1[0],L1[1],L1[2]}, {H3_[0],H3_[1],H3_[2]}, {T3[0],T3[1],T3[2]}
    };

    // Generieren der Koordinaten
    int j = 0;
    for(int k=0; k<30; k++){ //über Kanten iterieren
        for(int i =0; i < edge_LEDs[k]; i++) // über LEDs pro Kante iterieren
        {
            j++;
        };
    };
    // Kartesische Koordinaten:  {Kantenindex, LED-Index, X, Y, Z}
    float LED_array[j][5];
    j=0;
    for(int k=0; k<30; k++){ //über Kanten iterieren
        for(int i =0; i < edge_LEDs[k]; i++) // über LEDs pro Kante iterieren
        {
            LED_array[j][0] = float(k);
            LED_array[j][1] = float (i);
            LED_array[j][2] = float(Ursprung[k][0] + i*Vektor[k][0]);
            LED_array[j][3] = float(Ursprung[k][1] + i*Vektor[k][1]);
            LED_array[j][4] = float(Ursprung[k][2] + i*Vektor[k][2]);
            j++;
        };
    };

    // Kugel-Koordinaten:  {Kantenindex, LED-Index, r, phi, theta}
    float LED_array_spherical[j][5];
    j=0;
    double x,y,z;
    for(int k=0; k<30; k++){ //über Kanten iterieren
        for(int i =0; i < edge_LEDs[k]; i++) // über LEDs pro Kante iterieren
        {
            LED_array_spherical[j][0] = float(k);
            LED_array_spherical[j][1] = float (i);
            x = (Ursprung[k][0] + i*Vektor[k][0]);
            y = (Ursprung[k][1] + i*Vektor[k][1]);
            z = (Ursprung[k][2] + i*Vektor[k][2]);
            LED_array_spherical[j][2] = float(sqrt(x*x+y*y+z*z)); //radius
            LED_array_spherical[j][3] = float(atan(y/x)); //phi
            LED_array_spherical[j][4] = float(acos(z/(sqrt(x*x+y*y+z*z)))); //theta
            j++;
        };
    };
}