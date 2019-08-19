
#include <Dode.h>
#include <math.h>
#include <stdlib.h>

#define my_pi 3.14159265358979323846

Coordinates coordinates[NR_TOTAL_LEDS];

void Dode::generate_mapping()
{
    const float a = 1;                     // Kantenlänge
    const float d = a / 2 * (1 + sqrt(5)); // virtuelle Kantenlänge für Zwischenebenen
    const float alpha = my_pi * 2 / 5;
    const float beta = my_pi * 3 / 10;
    //const float gamma = my_pi*3/5; // unused
    const float radius = a / 4 * sqrt(18 + 6 * sqrt(5));

    const float y_b1 = a / (2 * cos(beta));
    const float y_b2 = a * cos(alpha) / (2 * cos(beta));
    const float y_b3 = tan(beta) * a / 2;
    const float x_b2 = a * sin(alpha) / (2 * cos(beta));

    const float theta_t = asin(y_b1 / radius);
    const float z_t = radius * cos(theta_t);
    const float z_b = -z_t;

    const float y_l1 = d / (2 * cos(beta));
    const float y_l2 = y_l1 * cos(alpha);
    const float y_l3 = d / 2 * tan(beta);
    const float x_l2 = y_l1 * sin(alpha);
    //float x_l3 = d/2; // unused

    const float theta_h = asin(y_l1 / radius);
    const float z_h = radius * cos(theta_h);
    const float z_l = -z_h;

    //Eckkoordinaten Baseplate B
    const float corner_B1[3] = {0, y_b1, z_b};
    const float corner_B2[3] = {x_b2, y_b2, z_b};
    const float corner_B2_[3] = {-x_b2, y_b2, z_b};
    const float corner_B3[3] = {a / 2, -y_b3, z_b};
    const float corner_B3_[3] = {-a / 2, -y_b3, z_b};

    //Eckkoordinaten Topplate T
    const float corner_T1[3] = {0, -y_b1, z_t};
    const float corner_T2[3] = {x_b2, -y_b2, z_t};
    const float corner_T2_[3] = {-x_b2, -y_b2, z_t};
    const float corner_T3[3] = {a / 2, y_b3, z_t};
    const float corner_T3_[3] = {-a / 2, y_b3, z_t};

    //Zwischenebene H (Higher/Upper plane)
    const float corner_H1[3] = {0, -y_l1, z_h};
    const float corner_H2[3] = {x_l2, -y_l2, z_h};
    const float corner_H2_[3] = {-x_l2, -y_l2, z_h};
    const float corner_H3[3] = {d / 2, y_l3, z_h};
    const float corner_H3_[3] = {-d / 2, y_l3, z_h};

    //Zwischenebene L (Lower plane)
    const float corner_L1[3] = {0, y_l1, z_l};
    const float corner_L2[3] = {x_l2, y_l2, z_l};
    const float corner_L2_[3] = {-x_l2, y_l2, z_l};
    const float corner_L3[3] = {d / 2, -y_l3, z_l};
    const float corner_L3_[3] = {-d / 2, -y_l3, z_l};
    

    // Längen (Anzhal LEDs) der Kanten #1 bis #30
    const int edge_LEDs[EDGE_COUNT] = {
        NR_LEDS_EDGE_01, NR_LEDS_EDGE_02, NR_LEDS_EDGE_03, NR_LEDS_EDGE_04, NR_LEDS_EDGE_05,
        NR_LEDS_EDGE_06, NR_LEDS_EDGE_07, NR_LEDS_EDGE_08, NR_LEDS_EDGE_09, NR_LEDS_EDGE_10,
        NR_LEDS_EDGE_11, NR_LEDS_EDGE_12, NR_LEDS_EDGE_13, NR_LEDS_EDGE_14, NR_LEDS_EDGE_15,
        NR_LEDS_EDGE_16, NR_LEDS_EDGE_17, NR_LEDS_EDGE_18, NR_LEDS_EDGE_19, NR_LEDS_EDGE_20,
        NR_LEDS_EDGE_21, NR_LEDS_EDGE_22, NR_LEDS_EDGE_23, NR_LEDS_EDGE_24, NR_LEDS_EDGE_25,
        NR_LEDS_EDGE_26, NR_LEDS_EDGE_27, NR_LEDS_EDGE_28, NR_LEDS_EDGE_29, NR_LEDS_EDGE_30};

    // Einzel-Vektoren der LED-Abstände: Zwischenräume = Anzahl_LEDs-1;
    float Vektor[EDGE_COUNT][3] = {
        // Kante1: corner_B1 --> corner_B2_
        {(corner_B2_[0] - corner_B1[0]) / (edge_LEDs[0] - 1), (corner_B2_[1] - corner_B1[1]) / (edge_LEDs[0] - 1), (corner_B2_[2] - corner_B1[2]) / (edge_LEDs[0] - 1)},
        // Kante2: corner_B2_ --> corner_L2_
        {(corner_L2_[0] - corner_B2_[0]) / (edge_LEDs[1] - 1), (corner_L2_[1] - corner_B2_[1]) / (edge_LEDs[1] - 1), (corner_L2_[2] - corner_B2_[2]) / (edge_LEDs[1] - 1)},
        // Kante3: corner_L2_ --> corner_H3_
        {(corner_H3_[0] - corner_L2_[0]) / (edge_LEDs[2] - 1), (corner_H3_[1] - corner_L2_[1]) / (edge_LEDs[2] - 1), (corner_H3_[2] - corner_L2_[2]) / (edge_LEDs[2] - 1)},
        // Kante4: corner_B2_ --> corner_B3_
        {(corner_B3_[0] - corner_B2_[0]) / (edge_LEDs[3] - 1), (corner_B3_[1] - corner_B2_[1]) / (edge_LEDs[3] - 1), (corner_B3_[2] - corner_B2_[2]) / (edge_LEDs[3] - 1)},
        // Kante5: corner_B3_ --> corner_L3_
        {(corner_L3_[0] - corner_B3_[0]) / (edge_LEDs[4] - 1), (corner_L3_[1] - corner_B3_[1]) / (edge_LEDs[4] - 1), (corner_L3_[2] - corner_B3_[2]) / (edge_LEDs[4] - 1)},
        // Kante6: corner_L3_ --> corner_H2_
        {(corner_H2_[0] - corner_L3_[0]) / (edge_LEDs[5] - 1), (corner_H2_[1] - corner_L3_[1]) / (edge_LEDs[5] - 1), (corner_H2_[2] - corner_L3_[2]) / (edge_LEDs[5] - 1)},
        // Kante7: corner_B3_ --> corner_B3
        {(corner_B3[0] - corner_B3_[0]) / (edge_LEDs[6] - 1), (corner_B3[1] - corner_B3_[1]) / (edge_LEDs[6] - 1), (corner_B3[2] - corner_B3_[2]) / (edge_LEDs[6] - 1)},
        // Kante8: corner_B3 --> corner_L3
        {(corner_L3[0] - corner_B3[0]) / (edge_LEDs[7] - 1), (corner_L3[1] - corner_B3[1]) / (edge_LEDs[7] - 1), (corner_L3[2] - corner_B3[2]) / (edge_LEDs[7] - 1)},
        // Kante9: corner_L3 --> corner_H1
        {(corner_H1[0] - corner_L3[0]) / (edge_LEDs[8] - 1), (corner_H1[1] - corner_L3[1]) / (edge_LEDs[8] - 1), (corner_H1[2] - corner_L3[2]) / (edge_LEDs[8] - 1)},
        // Kante10: corner_B3 --> corner_B2
        {(corner_B2[0] - corner_B3[0]) / (edge_LEDs[9] - 1), (corner_B2[1] - corner_B3[1]) / (edge_LEDs[9] - 1), (corner_B2[2] - corner_B3[2]) / (edge_LEDs[9] - 1)},
        // Kante11: corner_B2 --> corner_L2
        {(corner_L2[0] - corner_B2[0]) / (edge_LEDs[10] - 1), (corner_L2[1] - corner_B2[1]) / (edge_LEDs[10] - 1), (corner_L2[2] - corner_B2[2]) / (edge_LEDs[10] - 1)},
        // Kante12: corner_L2 --> corner_H2
        {(corner_H2[0] - corner_L2[0]) / (edge_LEDs[11] - 1), (corner_H2[1] - corner_L2[1]) / (edge_LEDs[11] - 1), (corner_H2[2] - corner_L2[2]) / (edge_LEDs[11] - 1)},
        // Kante13: corner_B2 --> corner_B1
        {(corner_B1[0] - corner_B2[0]) / (edge_LEDs[12] - 1), (corner_B1[1] - corner_B2[1]) / (edge_LEDs[12] - 1), (corner_B1[2] - corner_B2[2]) / (edge_LEDs[12] - 1)},
        // Kante14: corner_B3 --> corner_L3
        {(corner_L3[0] - corner_B3[0]) / (edge_LEDs[13] - 1), (corner_L3[1] - corner_B3[1]) / (edge_LEDs[13] - 1), (corner_L3[2] - corner_B3[2]) / (edge_LEDs[13] - 1)},
        // Kante15: corner_L1 --> corner_H3
        {(corner_H3[0] - corner_L1[0]) / (edge_LEDs[14] - 1), (corner_H3[1] - corner_L1[1]) / (edge_LEDs[14] - 1), (corner_H3[2] - corner_L1[2]) / (edge_LEDs[14] - 1)},
        // Kante16: corner_L2_ --> corner_H2_
        {(corner_H2_[0] - corner_L2_[0]) / (edge_LEDs[15] - 1), (corner_H2_[1] - corner_L2_[1]) / (edge_LEDs[15] - 1), (corner_H2_[2] - corner_L2_[2]) / (edge_LEDs[15] - 1)},
        // Kante17: corner_H2_ --> corner_T2_
        {(corner_T2_[0] - corner_H2_[0]) / (edge_LEDs[16] - 1), (corner_T2_[1] - corner_H2_[1]) / (edge_LEDs[16] - 1), (corner_T2_[2] - corner_H2_[2]) / (edge_LEDs[16] - 1)},
        // Kante18: corner_T2_ --> corner_T3_
        {(corner_T3_[0] - corner_T2_[0]) / (edge_LEDs[17] - 1), (corner_T3_[1] - corner_T2_[1]) / (edge_LEDs[17] - 1), (corner_T3_[2] - corner_T2_[2]) / (edge_LEDs[17] - 1)},
        // Kante19: corner_L3_ --> corner_H1
        {(corner_H1[0] - corner_L3_[0]) / (edge_LEDs[18] - 1), (corner_H1[1] - corner_L3_[1]) / (edge_LEDs[18] - 1), (corner_H1[2] - corner_L3_[2]) / (edge_LEDs[18] - 1)},
        // Kante20: corner_H1 --> corner_T1
        {(corner_T1[0] - corner_H1[0]) / (edge_LEDs[19] - 1), (corner_T1[1] - corner_H1[1]) / (edge_LEDs[19] - 1), (corner_T1[2] - corner_H1[2]) / (edge_LEDs[19] - 1)},
        // Kante21: corner_T1 --> corner_T2_
        {(corner_T2_[0] - corner_T1[0]) / (edge_LEDs[20] - 1), (corner_T2_[1] - corner_T1[1]) / (edge_LEDs[20] - 1), (corner_T2_[2] - corner_T1[2]) / (edge_LEDs[20] - 1)},
        // Kante22: corner_L3 --> corner_H2
        {(corner_H2[0] - corner_L3[0]) / (edge_LEDs[21] - 1), (corner_H2[1] - corner_L3[1]) / (edge_LEDs[21] - 1), (corner_H2[2] - corner_L3[2]) / (edge_LEDs[21] - 1)},
        // Kante23: corner_H2 --> corner_T2
        {(corner_T2[0] - corner_H2[0]) / (edge_LEDs[22] - 1), (corner_T2[1] - corner_H2[1]) / (edge_LEDs[22] - 1), (corner_T2[2] - corner_H2[2]) / (edge_LEDs[22] - 1)},
        // Kante24: corner_T2 --> corner_T1
        {(corner_T1[0] - corner_T2[0]) / (edge_LEDs[23] - 1), (corner_T1[1] - corner_T2[1]) / (edge_LEDs[23] - 1), (corner_T2[2] - corner_T1[2]) / (edge_LEDs[23] - 1)},
        // Kante25: corner_L2 --> corner_H3
        {(corner_H3[0] - corner_L2[0]) / (edge_LEDs[24] - 1), (corner_H3[1] - corner_L2[1]) / (edge_LEDs[24] - 1), (corner_H3[2] - corner_L2[2]) / (edge_LEDs[24] - 1)},
        // Kante26: corner_H3 --> corner_T3
        {(corner_T3[0] - corner_H3[0]) / (edge_LEDs[25] - 1), (corner_T3[1] - corner_H3[1]) / (edge_LEDs[25] - 1), (corner_T3[2] - corner_H3[2]) / (edge_LEDs[25] - 1)},
        // Kante27: corner_T3 --> corner_T2
        {(corner_T2[0] - corner_T3[0]) / (edge_LEDs[26] - 1), (corner_T2[1] - corner_T3[1]) / (edge_LEDs[26] - 1), (corner_T2[2] - corner_T3[2]) / (edge_LEDs[26] - 1)},
        // Kante28: corner_L1 --> corner_H3_
        {(corner_H3_[0] - corner_L1[0]) / (edge_LEDs[27] - 1), (corner_H3_[1] - corner_L1[1]) / (edge_LEDs[27] - 1), (corner_H3_[2] - corner_L1[2]) / (edge_LEDs[27] - 1)},
        // Kante29: corner_H3_ --> corner_T3_
        {(corner_T3_[0] - corner_H3_[0]) / (edge_LEDs[28] - 1), (corner_T3_[1] - corner_H3_[1]) / (edge_LEDs[28] - 1), (corner_T3_[2] - corner_H3_[2]) / (edge_LEDs[28] - 1)},
        // Kante30: corner_T3_ --> corner_T3
        {(corner_T3[0] - corner_T3_[0]) / (edge_LEDs[29] - 1), (corner_T3[1] - corner_T3_[1]) / (edge_LEDs[29] - 1), (corner_T3[2] - corner_T3_[2]) / (edge_LEDs[29] - 1)}};

    
    // Urpsrung der Vektoren
    float Ursprung[][3] = {
        {corner_B1[0], corner_B1[1], corner_B1[2]}, 
        {corner_B2_[0], corner_B2_[1], corner_B2_[2]}, 
        {corner_L2_[0], corner_L2_[1], corner_L2_[2]}, 
        {corner_B2_[0], corner_B2_[1], corner_B2_[2]}, 
        {corner_B3_[0], corner_B3_[1], corner_B3_[2]}, //5
        {corner_L3_[0], corner_L3_[1], corner_L3_[2]}, 
        {corner_B3_[0], corner_B3_[1], corner_B3_[2]}, 
        {corner_B3[0], corner_B3[1], corner_B3[2]}, 
        {corner_L3[0], corner_L3[1], corner_L3[2]}, 
        {corner_B3[0], corner_B3[1], corner_B3[2]}, //10
        {corner_B2[0], corner_B2[1], corner_B2[2]}, 
        {corner_L2[0], corner_L2[1], corner_L2[2]}, 
        {corner_B2[0], corner_B2[1], corner_B2[2]}, 
        {corner_B1[0], corner_B1[1], corner_B1[2]}, 
        {corner_L1[0], corner_L1[1], corner_L1[2]}, //15
        {corner_L2_[0], corner_L2_[1], corner_L2_[2]}, 
        {corner_H2_[0], corner_H2_[1], corner_H2_[2]}, 
        {corner_T2_[0], corner_T2_[1], corner_T2_[2]}, 
        {corner_L3_[0], corner_L3_[1], corner_L3_[2]}, 
        {corner_H1[0], corner_H1[1], corner_H1[2]}, //20
        {corner_T1[0], corner_T1[1], corner_T1[2]}, 
        {corner_L3[0], corner_L3[1], corner_L3[2]}, 
        {corner_H2[0], corner_H2[1], corner_H2[2]}, 
        {corner_T2[0], corner_T2[1], corner_T2[2]}, 
        {corner_L2[0], corner_L2[1], corner_L2[2]}, //25
        {corner_H3[0], corner_H3[1], corner_H3[2]}, 
        {corner_T3[0], corner_T3[1], corner_T3[2]}, 
        {corner_L1[0], corner_L1[1], corner_L1[2]}, 
        {corner_H3_[0], corner_H3_[1], corner_H3_[2]}, 
        {corner_T3_[0], corner_T3_[1], corner_T3_[2]}}; // 30

    // Generieren der Koordinaten
    // Kartesische Koordinaten:  {X, Y, Z}
    // Kugel-Koordinaten:  {r, phi, theta}
    int j = 0;
    float c_x, c_y, c_z;
    for (int k = 0; k < EDGE_COUNT; k++)
    {                                          //über Kanten iterieren
        for (int i = 0; i < edge_LEDs[k]; i++) // über LEDs pro Kante iterieren
        {
            coordinates[j].i_edge = k;
            coordinates[j].i_led = i;

            c_x = float(Ursprung[k][0] + i * Vektor[k][0]);
            c_y = float(Ursprung[k][1] + i * Vektor[k][1]);
            c_z = float(Ursprung[k][2] + i * Vektor[k][2]);
            coordinates[j].x = c_x;
            coordinates[j].y = c_y;
            coordinates[j].z = c_z;
            coordinates[j].r = float(sqrt(c_x * c_x + c_y * c_y + c_z * c_z));                   //radius
            //phi:
            if(c_x==0) { //workaround for div by 0
                if(c_y>0) coordinates[j].phi = my_pi/2;
                else if(c_y<0) coordinates[j].phi = -my_pi/2;
                else coordinates[j].phi = 0;
            }
            else {
                coordinates[j].phi = float(atan(c_y / c_x));
            }
            coordinates[j].theta = float(acos(c_z / (sqrt(c_x * c_x + c_y * c_y + c_z * c_z)))); //theta
            j++;
        };
    };
}