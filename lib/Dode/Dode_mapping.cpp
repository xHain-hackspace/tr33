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

    //Eckkoordinaten Base Level (BL)
    const float corner_BL1[3] = {x_b2, y_b2, z_b};    
    const float corner_BL2[3] = {a / 2, -y_b3, z_b};
    const float corner_BL3[3] = {-a / 2, -y_b3, z_b};
    const float corner_BL4[3] = {-x_b2, y_b2, z_b};
    const float corner_BL5[3] = {0, y_b1, z_b};

    //Eckkoordinaten Top Level (TL)
    const float corner_TL1[3] = {-a / 2, y_b3, z_t};
    const float corner_TL2[3] = {a / 2, y_b3, z_t};
    const float corner_TL3[3] = {x_b2, -y_b2, z_t};
    const float corner_TL4[3] = {0, -y_b1, z_t};
    const float corner_TL5[3] = {-x_b2, -y_b2, z_t};   

    //Zwischenebene Higher Level (HL)
    const float corner_HL1[3] = {d / 2, y_l3, z_h};
    const float corner_HL2[3] = {x_l2, -y_l2, z_h};
    const float corner_HL3[3] = {0, -y_l1, z_h};
    const float corner_HL4[3] = {-x_l2, -y_l2, z_h};
    const float corner_HL5[3] = {-d / 2, y_l3, z_h};

    //Zwischenebene Lower Level (LL)
    const float corner_LL1[3] = {0, y_l1, z_l};
    const float corner_LL2[3] = {x_l2, y_l2, z_l};
    const float corner_LL3[3] = {d / 2, -y_l3, z_l};
    const float corner_LL4[3] = {-d / 2, -y_l3, z_l};
    const float corner_LL5[3] = {-x_l2, y_l2, z_l};

    // Längen (Anzhal LEDs) der Kanten #1 bis #30
    const int edge_LEDs[EDGE_COUNT] = {
        NR_LEDS_EDGE_01, NR_LEDS_EDGE_02, NR_LEDS_EDGE_03, NR_LEDS_EDGE_04, NR_LEDS_EDGE_05,
        NR_LEDS_EDGE_06, NR_LEDS_EDGE_07, NR_LEDS_EDGE_08, NR_LEDS_EDGE_09, NR_LEDS_EDGE_10,
        NR_LEDS_EDGE_11, NR_LEDS_EDGE_12, NR_LEDS_EDGE_13, NR_LEDS_EDGE_14, NR_LEDS_EDGE_15,
        NR_LEDS_EDGE_16, NR_LEDS_EDGE_17, NR_LEDS_EDGE_18, NR_LEDS_EDGE_19, NR_LEDS_EDGE_20,
        NR_LEDS_EDGE_21, NR_LEDS_EDGE_22, NR_LEDS_EDGE_23, NR_LEDS_EDGE_24, NR_LEDS_EDGE_25,
        NR_LEDS_EDGE_26, NR_LEDS_EDGE_27, NR_LEDS_EDGE_28, NR_LEDS_EDGE_29, NR_LEDS_EDGE_30};

    // Einzel-Vektoren der LED-Abstände in XYZ-Koordinaten: Zwischenräume = Anzahl_LEDs-1;
    // C-förmige LED-Streifen im Uhrzeigersinn (von innen betrachtet)
    float Vektor[EDGE_COUNT][3] = {
        // LED-Streifen 1B: Kanten 1-2-3
        // Kante1: corner_BL1 --> corner_BL5
        {(corner_BL5[0] - corner_BL1[0]) / (edge_LEDs[12] - 1), (corner_BL5[1] - corner_BL1[1]) / (edge_LEDs[12] - 1), (corner_BL5[2] - corner_BL1[2]) / (edge_LEDs[12] - 1)},
        // Kante2: corner_BL5 --> corner_LL1
        {(corner_LL1[0] - corner_BL5[0]) / (edge_LEDs[13] - 1), (corner_LL1[1] - corner_BL5[1]) / (edge_LEDs[13] - 1), (corner_LL1[2] - corner_BL5[2]) / (edge_LEDs[13] - 1)},
        // Kante3: corner_LL1 --> corner_HL1
        {(corner_HL1[0] - corner_LL1[0]) / (edge_LEDs[14] - 1), (corner_HL1[1] - corner_LL1[1]) / (edge_LEDs[14] - 1), (corner_HL1[2] - corner_LL1[2]) / (edge_LEDs[14] - 1)},
        
        // LED-Streifen 2B: Kanten 4-5-6
        // Kante4: corner_BL2 --> corner_BL1
        {(corner_BL1[0] - corner_BL2[0]) / (edge_LEDs[9] - 1), (corner_BL1[1] - corner_BL2[1]) / (edge_LEDs[9] - 1), (corner_BL1[2] - corner_BL2[2]) / (edge_LEDs[9] - 1)},
        // Kante5: corner_BL1 --> corner_LL2
        {(corner_LL2[0] - corner_BL1[0]) / (edge_LEDs[10] - 1), (corner_LL2[1] - corner_BL1[1]) / (edge_LEDs[10] - 1), (corner_LL2[2] - corner_BL1[2]) / (edge_LEDs[10] - 1)},
        // Kante6: corner_LL2 --> corner_HL2
        {(corner_HL2[0] - corner_LL2[0]) / (edge_LEDs[11] - 1), (corner_HL2[1] - corner_LL2[1]) / (edge_LEDs[11] - 1), (corner_HL2[2] - corner_LL2[2]) / (edge_LEDs[11] - 1)},
        
        // LED-Streifen 3B: Kanten 7-8-9
        // Kante7: corner_BL3 --> corner_BL2
        {(corner_BL2[0] - corner_BL3[0]) / (edge_LEDs[6] - 1), (corner_BL2[1] - corner_BL3[1]) / (edge_LEDs[6] - 1), (corner_BL2[2] - corner_BL3[2]) / (edge_LEDs[6] - 1)},
        // Kante8: corner_BL2 --> corner_LL3
        {(corner_LL3[0] - corner_BL2[0]) / (edge_LEDs[7] - 1), (corner_LL3[1] - corner_BL2[1]) / (edge_LEDs[7] - 1), (corner_LL3[2] - corner_BL2[2]) / (edge_LEDs[7] - 1)},
        // Kante9: corner_LL3 --> corner_HL3
        {(corner_HL3[0] - corner_LL3[0]) / (edge_LEDs[8] - 1), (corner_HL3[1] - corner_LL3[1]) / (edge_LEDs[8] - 1), (corner_HL3[2] - corner_LL3[2]) / (edge_LEDs[8] - 1)},
        
        // LED-Streifen 4B: Kanten 10-11-12
        // Kante10: corner_BL4 --> corner_BL3
        {(corner_BL3[0] - corner_BL4[0]) / (edge_LEDs[3] - 1), (corner_BL3[1] - corner_BL4[1]) / (edge_LEDs[3] - 1), (corner_BL3[2] - corner_BL4[2]) / (edge_LEDs[3] - 1)},
        // Kante11: corner_BL3 --> corner_LL4
        {(corner_LL4[0] - corner_BL3[0]) / (edge_LEDs[4] - 1), (corner_LL4[1] - corner_BL3[1]) / (edge_LEDs[4] - 1), (corner_LL4[2] - corner_BL3[2]) / (edge_LEDs[4] - 1)},
        // Kante12: corner_LL4 --> corner_HL4
        {(corner_HL4[0] - corner_LL4[0]) / (edge_LEDs[5] - 1), (corner_HL4[1] - corner_LL4[1]) / (edge_LEDs[5] - 1), (corner_HL4[2] - corner_LL4[2]) / (edge_LEDs[5] - 1)},
        
        // LED-Streifen 5B: Kanten 13-14-15
        // Kante13: corner_BL5 --> corner_BL4
        {(corner_BL4[0] - corner_BL5[0]) / (edge_LEDs[0] - 1), (corner_BL4[1] - corner_BL5[1]) / (edge_LEDs[0] - 1), (corner_BL4[2] - corner_BL5[2]) / (edge_LEDs[0] - 1)},
        // Kante14: corner_BL4 --> corner_LL5
        {(corner_LL5[0] - corner_BL4[0]) / (edge_LEDs[1] - 1), (corner_LL5[1] - corner_BL4[1]) / (edge_LEDs[1] - 1), (corner_LL5[2] - corner_BL4[2]) / (edge_LEDs[1] - 1)},
        // Kante15: corner_LL5 --> corner_HL5
        {(corner_HL5[0] - corner_LL5[0]) / (edge_LEDs[2] - 1), (corner_HL5[1] - corner_LL5[1]) / (edge_LEDs[2] - 1), (corner_HL5[2] - corner_LL5[2]) / (edge_LEDs[2] - 1)},
        
        // LED-Streifen 1A: Kanten 16-17-18
        // Kante16: corner_LL1 --> corner_HL5
        {(corner_HL5[0] - corner_LL1[0]) / (edge_LEDs[27] - 1), (corner_HL5[1] - corner_LL1[1]) / (edge_LEDs[27] - 1), (corner_HL5[2] - corner_LL1[2]) / (edge_LEDs[27] - 1)},
        // Kante17: corner_HL5 --> corner_TL1
        {(corner_TL1[0] - corner_HL5[0]) / (edge_LEDs[28] - 1), (corner_TL1[1] - corner_HL5[1]) / (edge_LEDs[28] - 1), (corner_TL1[2] - corner_HL5[2]) / (edge_LEDs[28] - 1)},
        // Kante18: corner_TL1 --> corner_TL2
        {(corner_TL2[0] - corner_TL1[0]) / (edge_LEDs[29] - 1), (corner_TL2[1] - corner_TL1[1]) / (edge_LEDs[29] - 1), (corner_TL2[2] - corner_TL1[2]) / (edge_LEDs[29] - 1)}};
        
        // LED-Streifen 2A: Kanten 19-20-21
        // Kante19: corner_LL2 --> corner_HL1
        {(corner_HL1[0] - corner_LL2[0]) / (edge_LEDs[24] - 1), (corner_HL1[1] - corner_LL2[1]) / (edge_LEDs[24] - 1), (corner_HL1[2] - corner_LL2[2]) / (edge_LEDs[24] - 1)},
        // Kante20: corner_HL1 --> corner_TL2
        {(corner_TL2[0] - corner_HL1[0]) / (edge_LEDs[25] - 1), (corner_TL2[1] - corner_HL1[1]) / (edge_LEDs[25] - 1), (corner_TL2[2] - corner_HL1[2]) / (edge_LEDs[25] - 1)},
        // Kante21: corner_TL2 --> corner_TL3
        {(corner_TL3[0] - corner_TL2[0]) / (edge_LEDs[26] - 1), (corner_TL3[1] - corner_TL2[1]) / (edge_LEDs[26] - 1), (corner_TL3[2] - corner_TL2[2]) / (edge_LEDs[26] - 1)},
        
        // LED-Streifen 3A: Kanten 22-23-24
        // Kante22: corner_LL3 --> corner_HL2
        {(corner_HL2[0] - corner_LL3[0]) / (edge_LEDs[21] - 1), (corner_HL2[1] - corner_LL3[1]) / (edge_LEDs[21] - 1), (corner_HL2[2] - corner_LL3[2]) / (edge_LEDs[21] - 1)},
        // Kante23: corner_HL2 --> corner_TL3
        {(corner_TL3[0] - corner_HL2[0]) / (edge_LEDs[22] - 1), (corner_TL3[1] - corner_HL2[1]) / (edge_LEDs[22] - 1), (corner_TL3[2] - corner_HL2[2]) / (edge_LEDs[22] - 1)},
        // Kante24: corner_TL3 --> corner_TL4
        {(corner_TL4[0] - corner_TL3[0]) / (edge_LEDs[23] - 1), (corner_TL4[1] - corner_TL3[1]) / (edge_LEDs[23] - 1), (corner_TL3[2] - corner_TL4[2]) / (edge_LEDs[23] - 1)},
        
        // LED-Streifen 4A: Kanten 25-26-27
        // Kante25: corner_LL4 --> corner_HL3
        {(corner_HL3[0] - corner_LL4[0]) / (edge_LEDs[18] - 1), (corner_HL3[1] - corner_LL4[1]) / (edge_LEDs[18] - 1), (corner_HL3[2] - corner_LL4[2]) / (edge_LEDs[18] - 1)},
        // Kante26: corner_HL3 --> corner_TL4
        {(corner_TL4[0] - corner_HL3[0]) / (edge_LEDs[19] - 1), (corner_TL4[1] - corner_HL3[1]) / (edge_LEDs[19] - 1), (corner_TL4[2] - corner_HL3[2]) / (edge_LEDs[19] - 1)},
        // Kante27: corner_TL4 --> corner_TL5
        {(corner_TL5[0] - corner_TL4[0]) / (edge_LEDs[20] - 1), (corner_TL5[1] - corner_TL4[1]) / (edge_LEDs[20] - 1), (corner_TL5[2] - corner_TL4[2]) / (edge_LEDs[20] - 1)},
        
        // LED-Streifen 5A: Kanten 28-29-30
        // Kante28: corner_LL5 --> corner_HL4
        {(corner_HL4[0] - corner_LL5[0]) / (edge_LEDs[15] - 1), (corner_HL4[1] - corner_LL5[1]) / (edge_LEDs[15] - 1), (corner_HL4[2] - corner_LL5[2]) / (edge_LEDs[15] - 1)},
        // Kante29: corner_HL4 --> corner_TL5
        {(corner_TL5[0] - corner_HL4[0]) / (edge_LEDs[16] - 1), (corner_TL5[1] - corner_HL4[1]) / (edge_LEDs[16] - 1), (corner_TL5[2] - corner_HL4[2]) / (edge_LEDs[16] - 1)},
        // Kante30: corner_TL5 --> corner_TL1
        {(corner_TL1[0] - corner_TL5[0]) / (edge_LEDs[17] - 1), (corner_TL1[1] - corner_TL5[1]) / (edge_LEDs[17] - 1), (corner_TL1[2] - corner_TL5[2]) / (edge_LEDs[17] - 1)},
    
    // Startpunkt (Eck-Koordinaten XYZ) der 30 Kanten-Vektoren
    float Ursprung[][3] = {
        // LED-Streifen 1B
        {corner_BL1[0], corner_BL1[1], corner_BL1[2]}, 
        {corner_BL5[0], corner_BL5[1], corner_BL5[2]}, 
        {corner_LL1[0], corner_LL1[1], corner_LL1[2]},

        // LED-Streifen 2B
        {corner_BL2[0], corner_BL2[1], corner_BL2[2]}, // Kante 4
        {corner_BL1[0], corner_BL1[1], corner_BL1[2]},
        {corner_LL2[0], corner_LL2[1], corner_LL2[2]},

        // LED-Streifen 3B
        {corner_BL3[0], corner_BL3[1], corner_BL3[2]}, // Kante 7
        {corner_BL2[0], corner_BL2[1], corner_BL2[2]},
        {corner_LL3[0], corner_LL3[1], corner_LL3[2]}, 

        // LED-Streifen 4B
        {corner_BL4[0], corner_BL4[1], corner_BL4[2]}, // Kante 10
        {corner_BL3[0], corner_BL3[1], corner_BL3[2]}, 
        {corner_LL4[0], corner_LL4[1], corner_LL4[2]}, 

        // LED-Streifen 5B
        {corner_BL5[0], corner_BL5[1], corner_BL5[2]}, // Kante 13
        {corner_BL4[0], corner_BL4[1], corner_BL4[2]}, 
        {corner_LL5[0], corner_LL5[1], corner_LL5[2]}, 

        // LED-Streifen 1A
        {corner_LL1[0], corner_LL1[1], corner_LL1[2]}, // Kante 16
        {corner_HL5[0], corner_HL5[1], corner_HL5[2]}, 
        {corner_TL1[0], corner_TL1[1], corner_TL1[2]}}

        // LED-Streifen 2A
        {corner_LL2[0], corner_LL2[1], corner_LL2[2]}, // Kante 19
        {corner_HL1[0], corner_HL1[1], corner_HL1[2]}, 
        {corner_TL1[0], corner_TL1[1], corner_TL1[2]}}

        // LED-Streifen 3A
        {corner_LL3[0], corner_LL3[1], corner_LL3[2]},  // Kante 22
        {corner_HL2[0], corner_HL2[1], corner_HL2[2]}, 
        {corner_TL3[0], corner_TL3[1], corner_TL3[2]},

        // LED-Streifen 4A
        {corner_LL4[0], corner_LL4[1], corner_LL4[2]}, // Kante 25
        {corner_HL3[0], corner_HL3[1], corner_HL3[2]}, 
        {corner_TL4[0], corner_TL4[1], corner_TL4[2]}, 

        // LED-Streifen 5A
        {corner_LL5[0], corner_LL5[1], corner_LL5[2]}, // Kante 28
        {corner_HL4[0], corner_HL4[1], corner_HL4[2]}, 
        {corner_TL5[0], corner_TL5[1], corner_TL5[2]}
        };

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
            //phi (from 0 to 2*pi -> offset of pi needed):
            if(c_x==0) { //workaround for div by 0
                if(c_y>0) coordinates[j].phi = my_pi/2+my_pi;
                else if(c_y<0) coordinates[j].phi = -my_pi/2+my_pi;
                else coordinates[j].phi = 0+my_pi;
            }
            else {
                coordinates[j].phi = float(atan2(c_y , c_x))+my_pi;
            }
            coordinates[j].theta = float(acos(c_z / (sqrt(c_x * c_x + c_y * c_y + c_z * c_z)))); //theta
            j++;
        };
    };
}