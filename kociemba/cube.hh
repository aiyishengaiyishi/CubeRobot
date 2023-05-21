#include "requires.hh"

namespace cuberobot {

class CubeRobot {
 public:
  CubeRobot() {}
  static int idCornerOrientation(Cube c);
  static int idEdgeOrientation(Cube c);
  static int idCornerPosition(Cube c);
  static int idEdgePosition(Cube c, int l, int r, int delta = 0);
  static int idUpDownEdgePosition(Cube c) { return idEdgePosition(c, UR, DB); }
  static int idMiddleEdgePosition(Cube c) { return idEdgePosition(c, FR, BR, 8); }
  static Cube cornerPosition(int id);
  static Cube edgePosition(int id, bool flag);
  static Cube upDownEdgePosition(int id) { return edgePosition(id, true); }
  static Cube middleEdgePosition(int id) { return edgePosition(id, false); }
  static int idSlice(Cube c);
  static Cube sliceCube(int id);
  static Cube twistCube(int id);
  static Cube flipCube(int id);
  static void cornerMultiply(const Cube *a, const Cube *b, Cube *ab);
  static void edgeMultiply(const Cube *a, const Cube *b, Cube *ab);
  static void edgeMultiply1(const Cube *a, const Cube *b, Cube *ab);
  static Cube cubeMove(Cube c, int m);
  static void initPositionTable(int num,
                                Cube (*cubeStatus)(int),
                                void (*cubeMultiply)(const Cube *, const Cube *, Cube *),
                                int table[][NMOVE],
                                int (*idCube)(Cube));
  static void initCornerPositionMoveTable() {
    initPositionTable(NCP, cornerPosition, cornerMultiply, cpMoveTable, idCornerPosition);
  }
  static void initUpDownEdgePositionMoveTable() {
    initPositionTable(NEP1, upDownEdgePosition, edgeMultiply, epMoveTable1, idUpDownEdgePosition);
  }
  static void initMiddleEdgePositionMoveTable() {
    initPositionTable(NEP2, middleEdgePosition, edgeMultiply, epMoveTable2, idMiddleEdgePosition);
  }
  static void initSliceMoveTable() {
    initPositionTable(NSLICE, sliceCube, edgeMultiply, sliceMoveTable, idSlice);
  }
  static void initTwistMoveTable() {
    initPositionTable(NTWIST, twistCube, cornerMultiply, twistMoveTable, idCornerOrientation);
  }
  static void initFlipMoveTable() {
    initPositionTable(NFLIP, flipCube, edgeMultiply, flipMoveTable, idEdgeOrientation);
  }
  static void initPruneTable(int num,
                             int prune_table[],
                             int move_table[][NMOVE],
                             bool flag);
  static void initPruneTable1() {
    initPruneTable(NTWIST, PruneTable1, twistMoveTable, false);
  }
  static void initPruneTable2() {
    initPruneTable(NFLIP, PruneTable2, flipMoveTable, false);
  }
  static void initPruneTable3() {
    initPruneTable(NSLICE, PruneTable3, sliceMoveTable, false);
  }
  static void initPruneTable4() {
    initPruneTable(NCP, PruneTable4, cpMoveTable, true);
  }
  static void initPruneTable5() {
    initPruneTable(NEP1, PruneTable5, epMoveTable1, true);
  }
  static void initPruneTable6() {
    initPruneTable(NEP2, PruneTable6, epMoveTable2, true);
  }
  static int tranint(char x);
  static int cornerId(int x, int y, int z);
  static int edgeId(int x, int y);
  static int edgeOrientation(int id, int pos, int c0, int c1);
  static Cube getCube();
  static void search2(int CP, int EP1, int EP2, int cnt, int togo2);
  static void search1(int twist, int flip, int slice, int togo1);
  static void initMoveTable();
  static void init();
  static void solve();
};

}  // namespace cuberobot