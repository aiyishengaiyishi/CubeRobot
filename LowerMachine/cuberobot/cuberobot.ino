#include <Servo.h>

class Stepper {
 public:
  Stepper(int dir, int step) {
    dir_pin_ = dir;
    step_pin_ = step;
    
    pinMode(dir_pin_, OUTPUT);
    pinMode(step_pin_, OUTPUT);
  }
  rotateDirStep(bool dir, int step) {  // dir: 顺时针(true)/逆时针(false)
    if (dir) {
      digitalWrite(dir_pin_, LOW);
    } else {
      digitalWrite(dir_pin_, HIGH);
    }
    rotateStep(step);
  }
  rotateStep(int step) {
    for (int i = 0; i < step * STEPS_PER_REV; ++i) {
      digitalWrite(step_pin_,HIGH); 
      delayMicroseconds(500);
      digitalWrite(step_pin_,LOW); 
      delayMicroseconds(500);
    }
  }
  clockWiseRotate() {  // 顺时针 90°
    rotateDirStep(true, 1);
    delayTime();
  }
  invClockWiseRotate() {  // 逆时针 90°
    rotateDirStep(false, 1);
    delayTime();
  }
  halfInvClockWiseRotate() {  // 逆时针转动 180°
    rotateDirStep(false, 2);
    delayTime();
  }
  halfClockWiseRotate() {  // 顺时针 180°
    rotateDirStep(true, 2);
    delayTime();
  }
  rotateSign(char sign, char flag) {
    if (sign == '1') {
      clockWiseRotate();
    } else if (sign == '2') {
      if (flag) {
        halfInvClockWiseRotate();
      } else {
        halfClockWiseRotate();
      }
    } else if (sign == '\'') {
      invClockWiseRotate();
    }
  }
  invRotateSign(char sign, char flag) {
    if (sign == '1') {
      invClockWiseRotate();
    } else if (sign == '2') {
      if (flag) {
        halfClockWiseRotate();
      } else {
        halfInvClockWiseRotate();
      }
    } else if (sign == '\'') {
      clockWiseRotate();
    }
  }
 private:
  int dir_pin_;
  int step_pin_;
  const static int STEPS_PER_REV = 1600;
  delayTime() { delay(1000); }
};

class Engine {
 public:
  Engine(int pin, int closure, int open) {
    pin_ = pin;
    closure_ = closure;
    open_ = open;
  }
  init() {
    servo_.attach(pin_);
    servo_.write(closure_);
    delayTime();
  }
  closureEngine() {  // 闭合机械爪
    servo_.write(closure_);
    delayTime();
  }
  openEngine() {  // 张开机械爪
    servo_.write(open_);
    delayTime();
  }
 private:
  int pin_;
  int closure_;
  int open_;
  Servo servo_;
  delayTime() { delay(1000); }
};

Stepper stepper_left(9, 11);
Stepper stepper_right(8, 10);
Engine engine_left(6, 160, 48);
Engine engine_right(5, 180, 60);

class CubeSolve {
 public:
  leftRotate(char sign) {
    engine_left.closureEngine();
    stepper_left.rotateSign(sign, true);
    engine_left.openEngine();
    stepper_left.invRotateSign(sign, true);
    engine_left.closureEngine();
  }
  rightRotate(char sign) {
    engine_right.closureEngine();
    stepper_right.rotateSign(sign, false);
    engine_right.openEngine();
    stepper_right.invRotateSign(sign, false);
    engine_right.closureEngine();
  }
  rightRotateCube(bool flag, bool is_half) {  // flag: 顺时针(false)/逆时针(true)
    engine_left.openEngine();
    if (flag) {
      stepper_right.rotateSign('\'', false);
      if (is_half) {
        stepper_right.rotateSign('\'', false);
      }
    } else {
      stepper_right.rotateSign('1', false);
      if (is_half) {
        stepper_right.rotateSign('1', false);
      }
    }
    engine_left.closureEngine();
    
    engine_right.openEngine();
    if (flag) {
      stepper_right.rotateSign('1', false);
      if (is_half) {
        stepper_right.rotateSign('1', false);
      }
    } else {
      stepper_right.rotateSign('\'', false);
      if (is_half) {
        stepper_right.rotateSign('\'', false);
      }
    }
    engine_right.closureEngine();
  }
  leftRotateCube(bool flag, bool is_half) {  // flag: 顺时针(true)/逆时针(false)
    engine_right.openEngine();
    if (flag) {
      stepper_left.rotateSign('1', true);
      if (is_half) {
        stepper_left.rotateSign('1', true);
      }
    } else {
      stepper_left.rotateSign('\'', true);
      if (is_half) {
        stepper_left.rotateSign('\'', true);
      }
    }
    engine_right.closureEngine();

    engine_left.openEngine();
    if (flag) {
      stepper_left.rotateSign('\'', false);
      if (is_half) {
        stepper_left.rotateSign('\'', false);
      }
    } else {
      stepper_left.rotateSign('1', false);
      if (is_half) {
        stepper_left.rotateSign('1', false);
      }
    }
    engine_left.closureEngine();
  }
  rotateDown(char sign) {
    leftRotate(sign);
  }
  rotateRight(char sign) {
    rightRotate(sign);
  }
  rotateFront(char sign) {
    rightRotateCube(true, false);
    leftRotate(sign);
    rightRotateCube(false, false);
  }
  rotateUp(char sign) {
    rightRotateCube(false, true);
    leftRotate(sign);
    rightRotateCube(false, true);
  }
  rotateLeft(char sign) {
    leftRotateCube(false, true);
    rightRotate(sign);
    leftRotateCube(false, true);
  }
  rotateBack(char sign) {
    leftRotateCube(false, false);
    rightRotate(sign);
    leftRotateCube(true, false);
  }
  rotate(char side, char sign) {
    if (side == 'D') {
      rotateDown(sign);
    } else if (side == 'R') {
      rotateRight(sign);
    } else if (side == 'F') {
      rotateFront(sign);
    } else if (side == 'U') {
      rotateUp(sign);
    } else if (side == 'L') {
      rotateLeft(sign);
    } else if (side == 'B') {
      rotateBack(sign);
    }
  }
  init() {
    // F
    rightRotateCube(false, false);
    Serial.println("F.jpg");
    delay(5000);

    // D
    rightRotateCube(false, false);
    Serial.println("D.jpg");
    delay(5000);

    // B
    rightRotateCube(false, false);
    Serial.println("B.jpg");
    delay(5000);

    // U
    rightRotateCube(false, false);
    Serial.println("U.jpg");
    delay(5000);

    // L
    leftRotateCube(true, false);
    rightRotateCube(false, false);
    Serial.println("L.jpg");
    delay(5000);

    // R
    rightRotateCube(false, true);
    Serial.println("R.jpg");
    delay(5000);

    // restore
    rightRotateCube(false, false);
    leftRotateCube(false, false);
  }
};

CubeSolve cube_solve;

void setup() {
  Serial.begin(9600);
  engine_left.init();
  engine_right.init();
}

void loop() {
  String str = "";
  while(Serial.available() > 0){
    str += char(Serial.read());
    delay(10);
  }
  if (str == "init") {
    cube_solve.init();
  }
  if(str != ""){
    int len = str.length() - 1;
    for (int i = 0; i < len; i += 3) {
      char side = str[i];
      char sign = str[i + 1];
      Serial.print(side);
      Serial.println(sign);
      cube_solve.rotate(side, sign);
      delay(1000);
    }
  }
}
