#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

struct Value {
  int value = 0;
  bool constant = true;
  mutable int usedCount = 0;
  Value(int val) { value = val; }

  bool operator==(const Value &other) const {
    return value == other.value and constant == other.constant &&
           usedCount == other.usedCount;
  }
};

class Operation {
public:
  std::map<std::string, Value>::iterator first;
  Operation(std::map<std::string, Value>::iterator a) : first(a) {}

  virtual ~Operation(){};
  virtual void execute() = 0;
  virtual void print() = 0;
};

class BinaryOperation : public Operation {
public:
  std::map<std::string, Value>::const_iterator op1;
  std::map<std::string, Value>::const_iterator op2;
  BinaryOperation(std::map<std::string, Value>::iterator a,
                  std::map<std::string, Value>::const_iterator b,
                  std::map<std::string, Value>::const_iterator c)
      : Operation(a), op1(b), op2(c) {
    op1->second.usedCount++;
    op2->second.usedCount++;
  }
  virtual ~BinaryOperation() {
    op1->second.usedCount--;
    op2->second.usedCount--;
  };
  void replaceOp1(std::map<std::string, Value>::const_iterator newOp) {
    op1->second.usedCount--;
    op1 = newOp;
    op1->second.usedCount++;
  }
  void replaceOp2(std::map<std::string, Value>::const_iterator newOp) {
    op2->second.usedCount--;
    op2 = newOp;
    op2->second.usedCount++;
  }
  virtual void execute() = 0;
  virtual void print() = 0;
};

class InputOp : public Operation {
public:
  std::vector<int>::const_iterator second;

  InputOp(std::map<std::string, Value>::iterator a,
          std::vector<int>::const_iterator b)
      : Operation(a), second(b) {
    first->second.constant = false;
  }
  virtual ~InputOp() {}
  virtual void execute() override final { first->second.value = *second; };
  virtual void print() override final {
    std::cout << "for(int " << first->first << "=1;" << first->first << "<=9;++"
              << first->first << "){\n";
  }
};

class AddOp : public BinaryOperation {
public:
  AddOp(std::map<std::string, Value>::iterator a,
        std::map<std::string, Value>::const_iterator b,
        std::map<std::string, Value>::const_iterator c)
      : BinaryOperation(a, b, c) {
    first->second.constant = op1->second.constant and op2->second.constant;
    if (first->second.constant) {
      first->second.value = op1->second.value + op2->second.value;
    }
  }
  virtual void execute() override final {
    first->second.value = op1->second.value + op2->second.value;
  };
  virtual void print() override final {
    std::cout << "int " << first->first << "=" << op1->first << "+"
              << op2->first << ";\n";
  }
};

class MulOp : public BinaryOperation {
public:
  MulOp(std::map<std::string, Value>::iterator a,
        std::map<std::string, Value>::const_iterator b,
        std::map<std::string, Value>::const_iterator c)
      : BinaryOperation(a, b, c) {
    first->second.constant = op1->second.constant and op2->second.constant;
    if (first->second.constant) {
      first->second.value = op1->second.value * op2->second.value;
    }
  }
  virtual void execute() override final {
    first->second.value = op1->second.value * op2->second.value;
  };
  virtual void print() override final {
    std::cout << "int " << first->first << "=" << op1->first << "*"
              << op2->first << ";\n";
  }
};

class ModOp : public BinaryOperation {
public:
  ModOp(std::map<std::string, Value>::iterator a,
        std::map<std::string, Value>::const_iterator b,
        std::map<std::string, Value>::const_iterator c)
      : BinaryOperation(a, b, c) {
    first->second.constant = op1->second.constant and op2->second.constant;
    if (first->second.constant) {
      first->second.value = op1->second.value % op2->second.value;
    }
  }
  virtual void execute() override final {
    first->second.value = op1->second.value % op2->second.value;
  };
  virtual void print() override final {
    std::cout << "int " << first->first << "=" << op1->first << "%"
              << op2->first << ";\n";
  }
};

class DivOp : public BinaryOperation {
public:
  DivOp(std::map<std::string, Value>::iterator a,
        std::map<std::string, Value>::const_iterator b,
        std::map<std::string, Value>::const_iterator c)
      : BinaryOperation(a, b, c) {
    first->second.constant = op1->second.constant and op2->second.constant;
    if (first->second.constant) {
      first->second.value = op1->second.value / op2->second.value;
    }
  }
  virtual void execute() override final {
    first->second.value = op1->second.value / op2->second.value;
  };
  virtual void print() override final {
    std::cout << "int " << first->first << "=" << op1->first << "/"
              << op2->first << ";\n";
  }
};

class EqOp : public BinaryOperation {
public:
  EqOp(std::map<std::string, Value>::iterator a,
       std::map<std::string, Value>::const_iterator b,
       std::map<std::string, Value>::const_iterator c)
      : BinaryOperation(a, b, c) {
    first->second.constant = op1->second.constant and op2->second.constant;
    if (first->second.constant) {
      first->second.value = op1->second.value == op2->second.value;
    }
  }
  virtual void execute() override final {
    first->second.value = op1->second.value == op2->second.value;
  };
  virtual void print() override final {
    std::cout << "int " << first->first << "=" << op1->first
              << "==" << op2->first << ";\n";
  }
};

class NeqOp : public BinaryOperation {
public:
  NeqOp(std::map<std::string, Value>::iterator a,
        std::map<std::string, Value>::const_iterator b,
        std::map<std::string, Value>::const_iterator c)
      : BinaryOperation(a, b, c) {
    first->second.constant = op1->second.constant and op2->second.constant;
    if (first->second.constant) {
      first->second.value = op1->second.value != op2->second.value;
    }
  }
  virtual void execute() override final {
    first->second.value = op1->second.value != op2->second.value;
  };
  virtual void print() override final {
    std::cout << "int " << first->first << "=" << op1->first
              << "!=" << op2->first << ";\n";
  }
};

class SetToOp : public Operation {
public:
  std::map<std::string, Value>::const_iterator second;

  SetToOp(std::map<std::string, Value>::iterator a,
          std::map<std::string, Value>::const_iterator b)
      : Operation(a), second(b) {
    second->second.usedCount++;
    first->second.constant = second->second.constant;
    std::cout << "Creating setToOperation. Using " << first->first << " and "
              << second->first << " "
              << (second->second.constant ? "constant" : "not constant")
              << "\n";
    if (first->second.constant) {
      first->second.value = second->second.value;
    }
  }
  virtual ~SetToOp() { second->second.usedCount--; }
  virtual void execute() override final {
    first->second.value = second->second.value;
  };
  virtual void print() override final {
    std::cout << "int " << first->first << "=" << second->first << ";\n";
  }
};

std::pair<std::string, std::string> split(const std::string &in,
                                          const std::string &sep = "..") {
  auto middle = in.find(sep);
  if (middle == in.npos) {
    throw std::logic_error("can't split");
  }
  return {in.substr(0, middle), in.substr(middle + sep.length())};
}

int getCode() {
  int w0 = 0;
  int x0 = 0;
  int y0 = 0;
  int z0 = 0;
  for (int l1 = 1; l1 <= 9; ++l1) {
    int w1 = l1;
    int x2 = z0;
    int x3 = x2 % 26;
    int z1 = z0 / 1;
    int x4 = x3 + 10;
    int x5 = x4 == w1 ? 1 : 0;
    int x6 = x5 == 0 ? 1 : 0;
    int y2 = 25;
    int y3 = y2 * x6;
    int y4 = y3 + 1;
    int z2 = z1 * y4;
    int y6 = w1;
    int y7 = y6 + 12;
    int y8 = y7 * x6;
    int z3 = z2 + y8;
    for (int l2 = 1; l2 <= 9; ++l2) {
      int w2 = l2;
      int x8 = z3;
      int x9 = x8 % 26;
      int z4 = z3 / 1;
      int x10 = x9 + 12;
      int x11 = x10 == w2 ? 1 : 0;
      int x12 = x11 == 0 ? 1 : 0;
      int y10 = 25;
      int y11 = y10 * x12;
      int y12 = y11 + 1;
      int z5 = z4 * y12;
      int y14 = w2;
      int y15 = y14 + 7;
      int y16 = y15 * x12;
      int z6 = z5 + y16;
      for (int l3 = 1; l3 <= 9; ++l3) {
        int w3 = l3;
        int x14 = z6;
        int x15 = x14 % 26;
        int z7 = z6 / 1;
        int x16 = x15 + 10;
        int x17 = x16 == w3 ? 1 : 0;
        int x18 = x17 == 0 ? 1 : 0;
        int y18 = 25;
        int y19 = y18 * x18;
        int y20 = y19 + 1;
        int z8 = z7 * y20;
        int y22 = w3;
        int y23 = y22 + 8;
        int y24 = y23 * x18;
        int z9 = z8 + y24;
        for (int l4 = 1; l4 <= 9; ++l4) {
          int w4 = l4;
          int x20 = z9;
          int x21 = x20 % 26;
          int z10 = z9 / 1;
          int x22 = x21 + 12;
          int x23 = x22 == w4 ? 1 : 0;
          int x24 = x23 == 0 ? 1 : 0;
          int y26 = 25;
          int y27 = y26 * x24;
          int y28 = y27 + 1;
          int z11 = z10 * y28;
          int y30 = w4;
          int y31 = y30 + 8;
          int y32 = y31 * x24;
          int z12 = z11 + y32;
          for (int l5 = 1; l5 <= 9; ++l5) {
            std::cout << "step" << l1 << l2 << l3 << l4 << l5 << "\n";

            int w5 = l5;
            int x26 = z12;
            int x27 = x26 % 26;
            int z13 = z12 / 1;
            int x28 = x27 + 11;
            int x29 = x28 == w5 ? 1 : 0;
            int x30 = x29 == 0 ? 1 : 0;
            int y34 = 25;
            int y35 = y34 * x30;
            int y36 = y35 + 1;
            int z14 = z13 * y36;
            int y38 = w5;
            int y39 = y38 + 15;
            int y40 = y39 * x30;
            int z15 = z14 + y40;
            for (int l6 = 1; l6 <= 9; ++l6) {
              int w6 = l6;
              int x32 = z15;
              int x33 = x32 % 26;
              int z16 = z15 / 26;
              int x34 = x33 + -16;
              int x35 = x34 == w6 ? 1 : 0;
              int x36 = x35 == 0 ? 1 : 0;
              int y42 = 25;
              int y43 = y42 * x36;
              int y44 = y43 + 1;
              int z17 = z16 * y44;
              int y46 = w6;
              int y47 = y46 + 12;
              int y48 = y47 * x36;
              int z18 = z17 + y48;
              for (int l7 = 1; l7 <= 9; ++l7) {
                int w7 = l7;
                int x38 = z18;
                int x39 = x38 % 26;
                int z19 = z18 / 1;
                int x40 = x39 + 10;
                int x41 = x40 == w7 ? 1 : 0;
                int x42 = x41 == 0 ? 1 : 0;
                int y50 = 25;
                int y51 = y50 * x42;
                int y52 = y51 + 1;
                int z20 = z19 * y52;
                int y54 = w7;
                int y55 = y54 + 8;
                int y56 = y55 * x42;
                int z21 = z20 + y56;
                for (int l8 = 1; l8 <= 9; ++l8) {
                  int w8 = l8;
                  int x44 = z21;
                  int x45 = x44 % 26;
                  int z22 = z21 / 26;
                  int x46 = x45 + -11;
                  int x47 = x46 == w8 ? 1 : 0;
                  int x48 = x47 == 0 ? 1 : 0;
                  int y58 = 25;
                  int y59 = y58 * x48;
                  int y60 = y59 + 1;
                  int z23 = z22 * y60;
                  int y62 = w8;
                  int y63 = y62 + 13;
                  int y64 = y63 * x48;
                  int z24 = z23 + y64;
                  for (int l9 = 1; l9 <= 9; ++l9) {
                    int w9 = l9;
                    int x50 = z24;
                    int x51 = x50 % 26;
                    int z25 = z24 / 26;
                    int x52 = x51 + -13;
                    int x53 = x52 == w9 ? 1 : 0;
                    int x54 = x53 == 0 ? 1 : 0;
                    int y66 = 25;
                    int y67 = y66 * x54;
                    int y68 = y67 + 1;
                    int z26 = z25 * y68;
                    int y70 = w9;
                    int y71 = y70 + 3;
                    int y72 = y71 * x54;
                    int z27 = z26 + y72;
                    for (int l10 = 1; l10 <= 9; ++l10) {
                      int w10 = l10;
                      int x56 = z27;
                      int x57 = x56 % 26;
                      int z28 = z27 / 1;
                      int x58 = x57 + 13;
                      int x59 = x58 == w10 ? 1 : 0;
                      int x60 = x59 == 0 ? 1 : 0;
                      int y74 = 25;
                      int y75 = y74 * x60;
                      int y76 = y75 + 1;
                      int z29 = z28 * y76;
                      int y78 = w10;
                      int y79 = y78 + 13;
                      int y80 = y79 * x60;
                      int z30 = z29 + y80;
                      for (int l11 = 1; l11 <= 9; ++l11) {
                        int w11 = l11;
                        int x62 = z30;
                        int x63 = x62 % 26;
                        int z31 = z30 / 26;
                        int x64 = x63 + -8;
                        int x65 = x64 == w11 ? 1 : 0;
                        int x66 = x65 == 0 ? 1 : 0;
                        int y82 = 25;
                        int y83 = y82 * x66;
                        int y84 = y83 + 1;
                        int z32 = z31 * y84;
                        int y86 = w11;
                        int y87 = y86 + 3;
                        int y88 = y87 * x66;
                        int z33 = z32 + y88;
                        for (int l12 = 1; l12 <= 9; ++l12) {
                          int w12 = l12;
                          int x68 = z33;
                          int x69 = x68 % 26;
                          int z34 = z33 / 26;
                          int x70 = x69 + -1;
                          int x71 = x70 == w12 ? 1 : 0;
                          int x72 = x71 == 0 ? 1 : 0;
                          int y90 = 25;
                          int y91 = y90 * x72;
                          int y92 = y91 + 1;
                          int z35 = z34 * y92;
                          int y94 = w12;
                          int y95 = y94 + 9;
                          int y96 = y95 * x72;
                          int z36 = z35 + y96;
                          for (int l13 = 1; l13 <= 9; ++l13) {
                            int w13 = l13;
                            int x74 = z36;
                            int x75 = x74 % 26;
                            int z37 = z36 / 26;
                            int x76 = x75 + -4;
                            int x77 = x76 == w13 ? 1 : 0;
                            int x78 = x77 == 0 ? 1 : 0;
                            int y98 = 25;
                            int y99 = y98 * x78;
                            int y100 = y99 + 1;
                            int z38 = z37 * y100;
                            int y102 = w13;
                            int y103 = y102 + 4;
                            int y104 = y103 * x78;
                            int z39 = z38 + y104;
                            for (int l14 = 1; l14 <= 9; ++l14) {
                              int w14 = l14;
                              int x80 = z39;
                              int x81 = x80 % 26;
                              int z40 = z39 / 26;
                              int x82 = x81 + -14;
                              int x83 = x82 == w14 ? 1 : 0;
                              int x84 = x83 == 0 ? 1 : 0;
                              int y106 = 25;
                              int y107 = y106 * x84;
                              int y108 = y107 + 1;
                              int z41 = z40 * y108;
                              int y110 = w14;
                              int y111 = y110 + 13;
                              int y112 = y111 * x84;
                              int z42 = z41 + y112;
                              if (z42 == 0) {
                                std::cout << "Correct number " << l1 << l2 << l3
                                          << l4 << l5 << l6 << l7 << l8 << l9
                                          << l10 << l11 << l12 << l13 << l14
                                          << std::endl;
                                return 0;
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  std::cout << "No correct number" << std::endl;
  return -1;
}

void addSetTo(std::vector<std::unique_ptr<Operation>> &programm) {
  for (size_t idx = 0; idx < programm.size() - 1; ++idx) {
    MulOp *mul = dynamic_cast<MulOp *>(programm[idx].get());
    AddOp *add = dynamic_cast<AddOp *>(programm[idx + 1].get());
    if (mul != nullptr && add != nullptr) {
      if (mul->op2->first == "0" && *(mul->first) == *(add->op1)) {
        std::cout << "Replacing\n";
        mul->print();
        add->print();

        auto setOp = std::make_unique<SetToOp>(add->first, add->op2);
        std::cout << "by\n";
        setOp->print();
        programm[idx + 1] = std::move(setOp);
        // programm.erase(programm.begin() + idx + 1);
      }
    }
  }
}

void removeUnused(std::vector<std::unique_ptr<Operation>> &programm) {
  for (long idx = programm.size() - 2; idx >= 0; --idx) {
    if (programm[idx]->first->second.usedCount == 0) {
      std::cout << "remove unused\n";
      programm[idx]->print();
      programm.erase(programm.begin() + idx);
    }
  }
}

void addNeqInstructions(std::vector<std::unique_ptr<Operation>> &programm) {
  for (size_t idx = 0; idx < programm.size() - 1; ++idx) {
    EqOp *eq1 = dynamic_cast<EqOp *>(programm[idx].get());
    EqOp *eq2 = dynamic_cast<EqOp *>(programm[idx + 1].get());
    if (eq1 != nullptr && eq2 != nullptr) {
      if (eq2->op2->first == "0" && *(eq1->first) == *(eq2->op1) &&
          eq1->first->second.usedCount == 1) {
        std::cout << "Replacing\n";
        eq1->print();
        eq2->print();

        auto neqOp = std::make_unique<NeqOp>(eq2->first, eq1->op1, eq1->op2);
        std::cout << "by\n";
        neqOp->print();
        programm[idx + 1] = std::move(neqOp);
        // programm.erase(programm.begin() + idx + 1);
      }
    }
  }
}

void fixConstantValues(std::vector<std::unique_ptr<Operation>> &programm,
                       std::map<std::string, Value> &constants) {
  for (long idx = programm.size() - 2; idx >= 0; --idx) {
    if (programm[idx]->first->second.constant) {

      std::cout << "following expression is constant with value "
                << programm[idx]->first->second.value << "\n";
      programm[idx]->print();
      int value = programm[idx]->first->second.value;
      auto [itOp, ok] = constants.emplace(std::to_string(value), value);

      auto setOp = std::make_unique<SetToOp>(programm[idx]->first, itOp);
      programm[idx] = std::move(setOp);
    }
  }
}

void fixUselessOperations(std::vector<std::unique_ptr<Operation>> &programm) {
  for (size_t idx = 0; idx < programm.size(); ++idx) {
    if (AddOp *op = dynamic_cast<AddOp *>(programm[idx].get()); op) {
      if (op->op1->first == "0") {
        std::cout << "Replacing\n";
        op->print();
        auto neqOp = std::make_unique<SetToOp>(op->first, op->op2);
        std::cout << "by\n";
        neqOp->print();
        programm[idx] = std::move(neqOp);
      } else if (op->op2->first == "0") {
        std::cout << "Replacing\n";
        op->print();
        auto neqOp = std::make_unique<SetToOp>(op->first, op->op1);
        std::cout << "by\n";
        neqOp->print();
        programm[idx] = std::move(neqOp);
      }
    }
    if (MulOp *op = dynamic_cast<MulOp *>(programm[idx].get()); op) {
      if (op->op1->first == "1") {
        std::cout << "Replacing\n";
        op->print();
        auto neqOp = std::make_unique<SetToOp>(op->first, op->op2);
        std::cout << "by\n";
        neqOp->print();
        programm[idx] = std::move(neqOp);
      } else if (op->op2->first == "1") {
        std::cout << "Replacing\n";
        op->print();
        auto neqOp = std::make_unique<SetToOp>(op->first, op->op1);
        std::cout << "by\n";
        neqOp->print();
        programm[idx] = std::move(neqOp);
      } else if (op->op2->first == "0") {
        std::cout << "Replacing\n";
        op->print();
        auto neqOp = std::make_unique<SetToOp>(op->first, op->op2);
        std::cout << "by\n";
        neqOp->print();
        programm[idx] = std::move(neqOp);
      } else if (op->op1->first == "0") {
        std::cout << "Replacing\n";
        op->print();
        auto neqOp = std::make_unique<SetToOp>(op->first, op->op1);
        std::cout << "by\n";
        neqOp->print();
        programm[idx] = std::move(neqOp);
      }
    }
    if (DivOp *op = dynamic_cast<DivOp *>(programm[idx].get()); op) {
      if (op->op2->first == "1") {
        std::cout << "Replacing\n";
        op->print();
        auto neqOp = std::make_unique<SetToOp>(op->first, op->op1);
        std::cout << "by\n";
        neqOp->print();
        programm[idx] = std::move(neqOp);
      }
    }
  }
}

void forwardRenamedValues(std::vector<std::unique_ptr<Operation>> &programm,
                          std::map<std::string, Value> &constants,
                          std::map<std::string, Value> &variables) {
  // SetTo Ops are just renaming stuff.
  std::map<std::string, std::string> renamingMap;
  for (size_t idx = 0; idx < programm.size(); ++idx) {
    if (SetToOp *op = dynamic_cast<SetToOp *>(programm[idx].get()); op) {
      if (renamingMap.count(op->first->first)) {
        throw std::logic_error{"unexpected"};
      }
      if (auto it = renamingMap.find(op->second->first);
          it != renamingMap.end()) {
        renamingMap[op->first->first] = it->second;
      } else {
        renamingMap[op->first->first] = op->second->first;
      }
    }
    if (BinaryOperation *op =
            dynamic_cast<BinaryOperation *>(programm[idx].get());
        op) {

      if (renamingMap.count(op->op1->first)) {
        auto &nameToReplace = renamingMap[op->op1->first];
        bool isConstant =
            std::isdigit(nameToReplace[0]) || nameToReplace[0] == '-';
        std::map<std::string, Value>::const_iterator it;

        if (isConstant) {
          it = constants.find(nameToReplace);
          if (it == constants.end()) {
            throw std::logic_error{"unexpected"};
          }
        } else {
          it = variables.find(nameToReplace);
          if (it == variables.end()) {
            throw std::logic_error{"unexpected"};
          }
        }
        std::cout << "replace " << op->op1->first << " by " << it->first
                  << " in\n";
        op->print();
        op->replaceOp1(it);
      }
      if (renamingMap.count(op->op2->first)) {
        auto &nameToReplace = renamingMap[op->op2->first];
        bool isConstant =
            std::isdigit(nameToReplace[0]) || nameToReplace[0] == '-';
        std::map<std::string, Value>::const_iterator it;

        if (isConstant) {
          it = constants.find(nameToReplace);
          if (it == constants.end()) {
            throw std::logic_error{"unexpected"};
          }
        } else {
          it = variables.find(nameToReplace);
          if (it == variables.end()) {
            throw std::logic_error{"unexpected"};
          }
        }
        std::cout << "replace " << op->op2->first << " by " << it->first
                  << " in\n";
        op->print();
        op->replaceOp2(it);
      }
    }
  }
}

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }

  std::map<std::string, Value> constants;
  std::map<std::string, Value> variables;

  std::unordered_map<std::string, int> variableCounter;
  variableCounter["w"] = 0;
  variableCounter["x"] = 0;
  variableCounter["y"] = 0;
  variableCounter["z"] = 0;
  std::vector<int> inputData(14, 9);
  std::iota(inputData.begin(), inputData.end(), 1);
  auto inIt = inputData.begin();
  std::vector<std::unique_ptr<Operation>> programm;

  auto [itZeroIt, ok1] = constants.emplace("0", 0);
  for (std::string op : {"x", "y", "z", "w"}) {
    auto varName = op + std::to_string(variableCounter[op]);
    auto [it, ok] = variables.emplace(varName, 0);
    programm.emplace_back(std::make_unique<SetToOp>(it, itZeroIt));
  }
  while (true) {
    std::string line;
    std::getline(input, line);
    if (line.size() < 2) {
      break;
    }
    auto [base, op] = split(line, " ");
    if (base == "inp") {
      ++variableCounter["l"];
      auto varNameLoop =
          std::string("l") + std::to_string(variableCounter["l"]);
      auto [itLoop, okLoop] = variables.emplace(varNameLoop, 0);
      programm.emplace_back(std::make_unique<InputOp>(itLoop, inIt));
      ++inIt;
      ++variableCounter[op];
      auto varName = op + std::to_string(variableCounter[op]);
      auto [it, ok] = variables.emplace(varName, 0);
      programm.emplace_back(std::make_unique<SetToOp>(it, itLoop));

    } else {
      auto [op1, op2] = split(op, " ");

      auto [itOp1, ok1] =
          variables.emplace(op1 + std::to_string(variableCounter[op1]), 0);
      auto [itOp2, ok2] =
          (std::isdigit(op2[0]) || op2[0] == '-')
              ? constants.emplace(op2, std::stoi(op2))
              : variables.emplace(op2 + std::to_string(variableCounter[op2]),
                                  0);
      ++variableCounter[op1];
      auto [itRet, okRet] =
          variables.emplace(op1 + std::to_string(variableCounter[op1]), 0);
      if (base == "add") {
        programm.emplace_back(std::make_unique<AddOp>(itRet, itOp1, itOp2));
      } else if (base == "mul") {
        programm.emplace_back(std::make_unique<MulOp>(itRet, itOp1, itOp2));
      } else if (base == "div") {
        programm.emplace_back(std::make_unique<DivOp>(itRet, itOp1, itOp2));
      } else if (base == "mod") {
        programm.emplace_back(std::make_unique<ModOp>(itRet, itOp1, itOp2));
      } else if (base == "eql") {
        programm.emplace_back(std::make_unique<EqOp>(itRet, itOp1, itOp2));
      } else {
        throw std::logic_error("unknown command");
      }
    }
  }

  addSetTo(programm);

  for (auto &[name, var] : variables) {
    std::cout << name << " "
              << " val=" << var.value << " used=" << var.usedCount << std::endl;
  }

  removeUnused(programm);
  fixConstantValues(programm, constants);
  removeUnused(programm);
  addNeqInstructions(programm);
  removeUnused(programm);
  fixUselessOperations(programm);
  for (auto &[name, var] : variables) {
    std::cout << name << " "
              << " val=" << var.value << " used=" << var.usedCount << std::endl;
  }

  forwardRenamedValues(programm, constants, variables);
  removeUnused(programm);
  fixUselessOperations(programm);
  removeUnused(programm);
  forwardRenamedValues(programm, constants, variables);
  removeUnused(programm);
  fixUselessOperations(programm);
  removeUnused(programm);
  forwardRenamedValues(programm, constants, variables);
  removeUnused(programm);
  std::cout << "\n";

  int steps = 0;
  for (auto &op : programm) {
    op->print();
  }
  std::cout << "if(z==0){\nstd::cout<<\"Correct number \"";
  for (auto val : inputData) {
    std::cout << "<<l" << val;
  }

  std::cout << "<<std::endl;\nreturn 0;}}}}}}}}}}}}}}}";
  std::cout << "Result " << steps << "\n";

  for (auto &[name, var] : variables) {
    std::cout << name << " "
              << " val=" << var.value << " used=" << var.usedCount << std::endl;
  }
  return 0;
}
