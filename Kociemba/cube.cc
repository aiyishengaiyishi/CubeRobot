#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>

#include "cube.hh"

namespace cuberobot {

int CubeRobot::idCornerOrientation(Cube c) {
  int id = 0;
  for (int i = URF; i < DRB; ++i) {
    id = id * 3 + c.co[i].o;
  }
  return id;
}

int CubeRobot::idEdgeOrientation(Cube c) {
  int id = 0;
  for (int i = UR; i < BR; ++i) {
    id = id * 2 + c.eo[i].o;
  }
  return id;
}

int CubeRobot::idCornerPosition(Cube c) {
  int id = 0;
  for (int i = DRB; i > URF; --i) {
    int s = 0;
    for (int j = i - 1; j >= URF; --j) {
      if (c.co[j].c > c.co[i].c) {
        ++s;
      }
    }
    id = (id + s) * i;
  }
  return id;
}

int CubeRobot::idEdgePosition(Cube c, int l, int r, int delta) {
  int id = 0;
  for (int i = r; i > l; --i) {
    int s = 0;
    for (int j = i - 1; j >= l; --j) {
      if (c.eo[j].e > c.eo[i].e) ++s;
    }
    id = (id + s) * (i - delta);
  }
  return id;
}

Cube CubeRobot::cornerPosition(int id) {
  Cube c;
  std::vector<bool> st(8);
  for (int i = 7; i >= 0; --i) {
    int s = id / fac[i];
    id = id % fac[i];
    for (int j = 7; j >= 0; --j) {
      if (st[j] == true) continue;
      if (!s) {
        c.co[i].c = corner(j);
        st[j] = true;
      }
      --s;
    }
  }
  return c;
}

Cube CubeRobot::edgePosition(int id, bool flag) {
  int r = (flag ? 8 : 4);
  int delta = (flag ? 0 : 8);
  Cube c;
  std::vector<bool> st(r);
  for (int i = r - 1; i >= 0; --i) {
    int s = id / fac[i];
    id = id % fac[i];
    for (int j = r - 1; j >= 0; --j) {
      if (st[j] == true) continue;
      if (!s) {
        c.eo[i + delta].e = edge(j + delta);
        st[j] = true;
        break;
      }
      --s;
    }
  }
  return c;
}

int CubeRobot::idSlice(Cube c) {
  int id = 0, y = 0;
  for (int i = BR; i >= UR; --i) {
    if (c.eo[i].e >= FR && c.eo[i].e <= BR) {
      id += C[11 - i][y + 1];
      ++y;
    }
  }
  return id;
}

Cube CubeRobot::sliceCube(int id) {
  Cube c;
  int y = 4;
  for (int i = 0; i < 12; ++i) {
    c.eo[i].e = edge(0);
  }
  for (int i = UR; i <= BR; ++i) {
    if (id >= C[11 - i][y]) {
      c.eo[i].e = edge(12 - y);
      id -= C[11 - i][y];
      --y;
    }
  }
  return c;
}

Cube CubeRobot::twistCube(int id) {
  Cube c;
  int tmp = 0;
  for (int i = DBL; i >= URF; --i) {
    c.co[i].o = id % 3;
    tmp = (id % 3 + tmp) % 3;
    id /= 3;
  }
  c.co[DRB].o = (3 - tmp) % 3;
  return c;
}

Cube CubeRobot::flipCube(int id) {
  Cube c;
  int tmp = 0;
  for (int i = BL; i >= UR; --i) {
    c.eo[i].o = id % 2;
    tmp = (id % 2 + tmp) % 2;
    id /= 2;
  }
  c.eo[BR].o = (2 - tmp) % 2;
  return c;
}

void CubeRobot::cornerMultiply(const Cube *a, const Cube *b, Cube *ab) {
  for (int i = URF; i <= DRB; ++i) {
    ab->co[i].c = a->co[b->co[i].c].c;
    ab->co[i].o = (a->co[b->co[i].c].o + b->co[i].o) % 3;
  }
}

void CubeRobot::edgeMultiply(const Cube *a, const Cube *b, Cube *ab) {
  for (int i = UR; i <= BR; ++i) {
    ab->eo[i].e = a->eo[b->eo[i].e].e;
    ab->eo[i].o = (a->eo[b->eo[i].e].o + b->eo[i].o) % 2;
  }
}

void CubeRobot::edgeMultiply1(const Cube *a, const Cube *b, Cube *ab) {
  for (int i = UR; i <= BR; ++i) {
    ab->eo[i].e = a->eo[b->eo[i].e].e;
  }
}

Cube CubeRobot::cubeMove(Cube c, int m) {
  Cube c_ret;
  cornerMultiply(&c, &Move[m], &c_ret);
  edgeMultiply(&c, &Move[m], &c_ret);
  return c_ret;
}

void CubeRobot::initPositionTable(int num, Cube (*cubeStatus)(int),
                                  void (*cubeMultiply)(const Cube *,
                                                       const Cube *, Cube *),
                                  int table[][NMOVE], int (*idCube)(Cube)) {
  Cube a, b;
  for (int i = 0; i < num; ++i) {
    a = cubeStatus(i);
    for (int j = U; j <= B; ++j) {
      for (int k = 0; k < 3; ++k) {
        cubeMultiply(&a, &Move[j], &b);
        a = b;
        table[i][j * 3 + k] = idCube(b);
      }
      cubeMultiply(&a, &Move[j], &b);
      a = b;
    }
  }
}

void CubeRobot::initPruneTable(int num, int prune_table[],
                               int move_table[][NMOVE], bool flag) {
  for (int i = 0; i < num; ++i) {
    prune_table[i] = -1;
  }
  prune_table[0] = 0;
  int depth = 0, done = 1;
  while (done < num) {
    for (int i = 0; i < num; ++i) {
      if (prune_table[i] == depth) {
        for (int j = 0; j < NMOVE; ++j) {
          if (flag && j >= 10) continue;
          int id = move_table[i][j];
          if (flag) {
            int x = operation[j].a, y = operation[j].b;
            id = move_table[i][x * 3 + y];
          }
          if (prune_table[id] == -1) {
            prune_table[id] = depth + 1;
            ++done;
          }
        }
      }
    }
    ++depth;
  }
}

int CubeRobot::tranint(char x) {
  switch (x) {
    case 'y':
      return 0;
    case 'w':
      return 1;
    case 'b':
      return 2;
    case 'g':
      return 3;
    case 'r':
      return 4;
    case 'o':
      return 5;
  }
  return -1;
}

int CubeRobot::cornerId(int x, int y, int z) {
  if (y > z) std::swap(y, z);
  if (x > y) std::swap(x, y);
  if (y > z) std::swap(y, z);
  if (x == 0 && y == 3 && z == 4) return 0;
  if (x == 0 && y == 2 && z == 4) return 1;
  if (x == 0 && y == 2 && z == 5) return 2;
  if (x == 0 && y == 3 && z == 5) return 3;
  if (x == 1 && y == 3 && z == 4) return 4;
  if (x == 1 && y == 2 && z == 4) return 5;
  if (x == 1 && y == 2 && z == 5) return 6;
  if (x == 1 && y == 3 && z == 5) return 7;
  return -1;
}

int CubeRobot::edgeId(int x, int y) {
  if (x > y) std::swap(x, y);
  if (x == 0 && y == 3) return 0;
  if (x == 0 && y == 4) return 1;
  if (x == 0 && y == 2) return 2;
  if (x == 0 && y == 5) return 3;
  if (x == 1 && y == 3) return 4;
  if (x == 1 && y == 4) return 5;
  if (x == 1 && y == 2) return 6;
  if (x == 1 && y == 5) return 7;
  if (x == 3 && y == 4) return 8;
  if (x == 2 && y == 4) return 9;
  if (x == 2 && y == 5) return 10;
  if (x == 3 && y == 5) return 11;
  return -1;
}

int CubeRobot::edgeOrientation(int id, int pos, int c0, int c1) {
  if (id == 0 && pos == 0) {
    if (c0 <= 1) return 0;
    if (c1 <= 1) return 1;
  }
  if (id == 0 && pos == 1) {
    if (c0 <= 1) return 1;
    if (c1 <= 1) return 0;
  }
  if (id == 1 && pos == 0) {
    if (c0 >= 2 && c0 <= 3) return 1;
    if (c1 >= 2 && c1 <= 3) return 0;
  }
  if (id == 1 && pos == 1) {
    if (c0 >= 2 && c0 <= 3) return 0;
    if (c1 >= 2 && c1 <= 3) return 1;
  }
  return -1;
}

Cube CubeRobot::getCube() {
  std::ifstream fin("input.txt");
  ColorCube x;
  Cube y;
  int tmp[10];
  char amp[10];
  for (int i = 1; i <= 9; i++) {  // U
    fin >> amp[i];
    tmp[i] = tranint(amp[i]);
  }
  x.co1[2].color[0] = tmp[1];
  x.co1[3].color[0] = tmp[3];
  x.co1[1].color[0] = tmp[7];
  x.co1[0].color[0] = tmp[9];
  x.eo1[3].color[0] = tmp[2];
  x.eo1[2].color[0] = tmp[4];
  x.eo1[0].color[0] = tmp[6];
  x.eo1[1].color[0] = tmp[8];

  for (int i = 1; i <= 9; i++) {  // D
    fin >> amp[i];
    tmp[i] = tranint(amp[i]);
  }
  x.co1[5].color[0] = tmp[1];
  x.co1[4].color[0] = tmp[3];
  x.co1[6].color[0] = tmp[7];
  x.co1[7].color[0] = tmp[9];
  x.eo1[5].color[0] = tmp[2];
  x.eo1[6].color[0] = tmp[4];
  x.eo1[4].color[0] = tmp[6];
  x.eo1[7].color[0] = tmp[8];

  for (int i = 1; i <= 9; i++) {  // L
    fin >> amp[i];
    tmp[i] = tranint(amp[i]);
  }
  x.co1[2].color[1] = tmp[1];
  x.co1[1].color[2] = tmp[3];
  x.co1[6].color[2] = tmp[7];
  x.co1[5].color[1] = tmp[9];
  x.eo1[2].color[1] = tmp[2];
  x.eo1[10].color[0] = tmp[4];
  x.eo1[9].color[0] = tmp[6];
  x.eo1[6].color[1] = tmp[8];

  for (int i = 1; i <= 9; i++) {  // R
    fin >> amp[i];
    tmp[i] = tranint(amp[i]);
  }
  x.co1[0].color[1] = tmp[1];
  x.co1[3].color[2] = tmp[3];
  x.co1[4].color[2] = tmp[7];
  x.co1[7].color[1] = tmp[9];
  x.eo1[0].color[1] = tmp[2];
  x.eo1[8].color[0] = tmp[4];
  x.eo1[11].color[0] = tmp[6];
  x.eo1[4].color[1] = tmp[8];

  for (int i = 1; i <= 9; i++) {  // F
    fin >> amp[i];
    tmp[i] = tranint(amp[i]);
  }
  x.co1[1].color[1] = tmp[1];
  x.co1[0].color[2] = tmp[3];
  x.co1[5].color[2] = tmp[7];
  x.co1[4].color[1] = tmp[9];
  x.eo1[1].color[1] = tmp[2];
  x.eo1[9].color[1] = tmp[4];
  x.eo1[8].color[1] = tmp[6];
  x.eo1[5].color[1] = tmp[8];

  for (int i = 1; i <= 9; i++) {  // B
    fin >> amp[i];
    tmp[i] = tranint(amp[i]);
  }
  x.co1[3].color[1] = tmp[1];
  x.co1[2].color[2] = tmp[3];
  x.co1[7].color[2] = tmp[7];
  x.co1[6].color[1] = tmp[9];
  x.eo1[3].color[1] = tmp[2];
  x.eo1[11].color[1] = tmp[4];
  x.eo1[10].color[1] = tmp[6];
  x.eo1[7].color[1] = tmp[8];

  fin.close();

  for (int i = 0; i < 8; ++i) {
    y.co[i].c = corner(
        cornerId(x.co1[i].color[0], x.co1[i].color[1], x.co1[i].color[2]));
    for (int j = 0; j < 3; ++j) {
      if (x.co1[i].color[j] <= 1) {
        y.co[i].o = j;
        break;
      }
    }
  }

  for (int i = 0; i < 12; ++i) {
    y.eo[i].e = edge(edgeId(x.eo1[i].color[0], x.eo1[i].color[1]));
    y.eo[i].o = edgeOrientation(y.eo[i].e / 8, i / 8, x.eo1[i].color[0],
                                x.eo1[i].color[1]);
  }

  return y;
}

void CubeRobot::search2(int CP, int EP1, int EP2, int cnt, int togo2) {
  if (((double)clock() - start_time) / CLOCKS_PER_SEC >= 5.0) return;
  if (flag) return;
  if (togo2 == 0) {
    if (!CP && !EP1 && !EP2) {
      ++xu;
      for (int i = 1; i <= ans.len; ++i) {
        sequence[xu] += trans[ans.xulie[i]];
        if (i != ans.len) sequence[xu] += " ";
      }
      minn = ans.len;
      flag = 1;
      --pan[cnt];
    }
  } else {
    int cp, ep1, ep2;
    for (int i = 0; i < 10; ++i) {
      if (flag) return;
      int x = operation[i].a, y = operation[i].b;
      int j = x * 3 + y;
      int m = ans.xulie[ans.len];
      if (m / 3 == x) continue;
      if (m / 3 - x == 1 && (m / 3) % 2 == 1) continue;
      cp = cpMoveTable[CP][j];
      ep1 = epMoveTable1[EP1][j];
      ep2 = epMoveTable2[EP2][j];
      int dist2 = std::max(std::max(PruneTable4[cp], PruneTable5[ep1]),
                           PruneTable6[ep2]);
      if (dist2 > togo2 - 1) continue;
      ++ans.len;
      ans.xulie[ans.len] = x * 3 + y;
      search2(cp, ep1, ep2, cnt, togo2 - 1);
      --ans.len;
    }
  }
}

void CubeRobot::search1(int twist, int flip, int slice, int togo1) {
  if (((double)clock() - start_time) / CLOCKS_PER_SEC >= 5.0) return;
  if (togo1 == 0) {
    if (!twist && !flip && !slice) {
      int x = initccp;
      Cube a = initc;
      Cube b;
      for (int i = 1; i <= ans.len; ++i) {
        x = cpMoveTable[x][ans.xulie[i]];
        for (int j = 0; j <= ans.xulie[i] % 3; ++j) {
          edgeMultiply1(&a, &Move[ans.xulie[i] / 3], &b);
          a = b;
        }
      }
      int y = idUpDownEdgePosition(a), z = idMiddleEdgePosition(a);
      int m =
          std::max(std::max(PruneTable4[x], PruneTable5[y]), PruneTable6[z]);
      for (int i = m; i <= minn - ans.len; ++i) {
        if (flag) break;
        if (pan[i + ans.len] <= 0) continue;
        search2(x, y, z, i + ans.len, i);
        flag = 0;
      }
    }
  } else {
    int flip1, twist1, slice1;
    for (int i = 0; i < NMOVE; ++i) {
      int x = ans.xulie[ans.len];
      if (x / 3 == i / 3) continue;
      if (x / 3 - i / 3 == 1 && (x / 3) % 2 == 1) continue;
      twist1 = twistMoveTable[twist][i];
      flip1 = flipMoveTable[flip][i];
      slice1 = sliceMoveTable[slice][i];
      int dist1 = std::max(std::max(PruneTable1[twist1], PruneTable2[flip1]),
                           PruneTable3[slice1]);
      if (dist1 > togo1 - 1) continue;
      ++ans.len;
      ans.xulie[ans.len] = i;
      search1(twist1, flip1, slice1, togo1 - 1);
      --ans.len;
    }
  }
}

void CubeRobot::initMoveTable() {
  Move[U] = {
        {{UBR,0},{URF,0},{UFL,0},{ULB,0},{DFR,0},{DLF,0},{DBL,0},{DRB,0}},
        {{UB,0},{UR,0},{UF,0},{UL,0},{DR,0},{DF,0},{DL,0},{DB,0},{FR,0},{FL,0},{BL,0},{BR,0}}};
  Move[D] = {
        {{URF,0},{UFL,0},{ULB,0},{UBR,0},{DLF,0},{DBL,0},{DRB,0},{DFR,0}},
        {{UR,0},{UF,0},{UL,0},{UB,0},{DF,0},{DL,0},{DB,0},{DR,0},{FR,0},{FL,0},{BL,0},{BR,0}}};
  Move[L] = {
        {{URF,0},{ULB,1},{DBL,2},{UBR,0},{DFR,0},{UFL,2},{DLF,1},{DRB,0}},
        {{UR,0},{UF,0},{BL,0},{UB,0},{DR,0},{DF,0},{FL,0},{DB,0},{FR,0},{UL,0},{DL,0},{BR,0}}};
  Move[R] = {
        {{DFR,2},{UFL,0},{ULB,0},{URF,1},{DRB,1},{DLF,0},{DBL,0},{UBR,2}},
        {{FR,0},{UF,0},{UL,0},{UB,0},{BR,0},{DF,0},{DL,0},{DB,0},{DR,0},{FL,0},{BL,0},{UR,0}}};
  Move[F] = {
        {{UFL,1},{DLF,2},{ULB,0},{UBR,0},{URF,2},{DFR,1},{DBL,0},{DRB,0}},
        {{UR,0},{FL,1},{UL,0},{UB,0},{DR,0},{FR,1},{DL,0},{DB,0},{UF,1},{DF,1},{BL,0},{BR,0}}};
  Move[B] = {
        {{URF,0},{UFL,0},{UBR,1},{DRB,2},{DFR,0},{DLF,0},{ULB,2},{DBL,1}},
        {{UR,0},{UF,0},{UL,0},{BR,1},{DR,0},{DF,0},{DL,0},{BL,1},{FR,0},{FL,0},{UB,1},{DB,1}}};
}

void CubeRobot::init() {
  initMoveTable();
  initTwistMoveTable();
  initFlipMoveTable();
  initSliceMoveTable();
  initCornerPositionMoveTable();
  initUpDownEdgePositionMoveTable();
  initMiddleEdgePositionMoveTable();
  initPruneTable1();
  initPruneTable2();
  initPruneTable3();
  initPruneTable4();
  initPruneTable5();
  initPruneTable6();
  for (int i = 0; i <= 999; i++) pan[i] = 1;
}

void CubeRobot::solve() {
  init();
  initc = getCube();
  ans.xulie[0] = 7890;
  int twist = idCornerOrientation(initc);
  int flip = idEdgeOrientation(initc);
  int slice = idSlice(initc);
  initccp = idCornerPosition(initc);
  initcep1 = idUpDownEdgePosition(initc);
  initcep2 = idMiddleEdgePosition(initc);
  int x = std::max(std::max(PruneTable1[twist], PruneTable2[flip]),
                   PruneTable3[slice]);
  start_time = clock();
  for (int i = x; i <= minn; i++) {
    search1(twist, flip, slice, i);
  }
  std::ofstream fout("output.txt");
  fout << sequence[xu] << std::endl;
  fout.close();
}

}  // namespace cuberobot

int main() {
  cuberobot::CubeRobot::solve();
  return 0;
}
