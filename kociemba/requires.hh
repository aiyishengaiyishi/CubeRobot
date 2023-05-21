#include <string>

namespace cuberobot {

const int NTWIST = 2187;
const int NFLIP = 2048;
const int NSLICE = 495;
const int NMOVE = 18;
const int NCP = 40320;
const int NEP1 = 40320;
const int NEP2 = 24;

typedef enum { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB } corner;
typedef enum { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR } edge;
typedef enum { U, D, L, R, F, B } direction;
typedef enum { g, w, o, r, y, b } color;
int fac[10] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};
int C[12][5] = {
    0, 0, 0,  0,  0,   1, 1,  0,  0,   0,   1, 2,  1,  0,   0,
    1, 3, 3,  1,  0,   1, 4,  6,  4,   1,   1, 5,  10, 10,  5,
    1, 6, 15, 20, 15,  1, 7,  21, 35,  35,  1, 8,  28, 56,  70,
    1, 9, 36, 84, 126, 1, 10, 45, 120, 210, 1, 11, 55, 165, 330,
};

int twistMoveTable[NTWIST][NMOVE];
int flipMoveTable[NFLIP][NMOVE];
int sliceMoveTable[NSLICE][NMOVE];
int cpMoveTable[NCP][NMOVE];
int epMoveTable1[NEP1][NMOVE];
int epMoveTable2[NEP2][NMOVE];
int PruneTable1[NTWIST];
int PruneTable2[NFLIP];
int PruneTable3[NSLICE];
int PruneTable4[NCP];
int PruneTable5[NEP1];
int PruneTable6[NEP2];

struct Corner {
  corner c;
  int o;
};
struct Edge {
  edge e;
  int o;
};
struct Cube {
  Corner co[8];
  Edge eo[12];
};

struct ColorCorner {
  int color[3];
};
struct ColorEdge {
  int color[2];
};
struct ColorCube {
  ColorCorner co1[8];
  ColorEdge eo1[12];
};

struct Operation {
  direction a;
  int b;
};
Operation operation[10] = {{U, 0}, {U, 1}, {U, 2}, {D, 0}, {D, 1},
                           {D, 2}, {L, 1}, {R, 1}, {F, 1}, {B, 1}};
Cube Move[6];
Cube initc;
struct zhuan {
  int xulie[10000000];
  int len = 0;
} ans;
int flag = 0;
int xu = 0;
int minn = 25;
std::string trans[18] = {"U1", "U2", "U'", "D1", "D2", "D'", "L1", "L2", "L'",
                         "R1", "R2", "R'", "F1", "F2", "F'", "B1", "B2", "B'"};

int initccp, initcep1, initcep2;
int pan[10000];

int start_time, end_time;
std::string sequence[10000];

}  // namespace cuberobot