#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <optional>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <unordered_map>

struct Packet {

  std::vector<bool> data;
  int version = 0;
  int typeID = 0;
  bool isLiteral;
  size_t literalValue;
  std::vector<Packet> packets;

  size_t getValue() {
    size_t ret;
    switch (typeID) {
    case 0:
      ret = 0;
      for (auto &pack : packets) {
        ret += pack.getValue();
      }
      break;

    case 1:
      ret = 1;
      for (auto &pack : packets) {
        ret *= pack.getValue();
      }
      break;
    case 2:
      ret = packets.at(0).getValue();
      for (size_t i = 1; i < packets.size(); ++i) {
        ret = std::min(ret, packets.at(i).getValue());
      }
      break;
    case 3:
      ret = packets.at(0).getValue();
      for (size_t i = 1; i < packets.size(); ++i) {
        ret = std::max(ret, packets.at(i).getValue());
      }
      break;
    case 4:
      ret = literalValue;
      break;
    case 5:

      ret = packets.at(0).getValue() > packets.at(1).getValue();
      break;
    case 6:

      ret = packets.at(0).getValue() < packets.at(1).getValue();
      break;
    case 7:

      ret = packets.at(0).getValue() == packets.at(1).getValue();
      break;

    default:
      throw std::logic_error("unknown");
    }
    return ret;
  }
};

std::vector<bool> makeBitVector(std::string &input) {
  std::vector<bool> bits;
  for (char c : input) {
    int nr = 0;
    if (c >= '0' && c <= '9') {
      nr = int(c - '0');
    }
    if (c >= 'A' && c <= 'F') {
      nr = int(10 + c - 'A');
    }
    bits.push_back((nr & 0x08) > 0);
    bits.push_back((nr & 0x04) > 0);
    bits.push_back((nr & 0x02) > 0);
    bits.push_back((nr & 0x01) > 0);
  }
  return bits;
}

int toInt(const std::vector<bool> &bits, size_t start, size_t length) {
  int result = 0;
  for (size_t i = start; i < start + length; ++i) {
    result = result * 2 + bits.at(i);
  }
  return result;
}

std::pair<size_t, size_t> readLiteral(const std::vector<bool> &bits,
                                      size_t start) {
  size_t ret = 0;
  size_t readAt = start;
  while (true) {
    ret = (ret << 4) + size_t(toInt(bits, readAt + 1, 4));
    if (bits.at(readAt) == false)
      break;
    readAt += 5;
  }
  return {ret, readAt - start + 5};
}

std::pair<Packet, size_t> makePacket(const std::vector<bool> &input,
                                     size_t start) {
  // std::cout << "Read starting at " << start << "\n";
  size_t readPos = start;
  Packet ret;
  ret.version = toInt(input, readPos, 3);
  readPos += 3;
  ret.typeID = toInt(input, readPos, 3);
  readPos += 3;
  ret.isLiteral = ret.typeID == 4;

  if (ret.isLiteral) {
    auto [literal, lengthRead] = readLiteral(input, readPos);
    ret.literalValue = literal;
    readPos += lengthRead;
    std::cout << "Read literal with value " << ret.literalValue << "\n";
  } else {
    bool lengthTypeID = input.at(readPos);
    readPos++;
    if (lengthTypeID) { // read by number of packets
      int nrOfPacketsToRead = toInt(input, readPos, 11);
      readPos += 11;
      std::cout << "Reading " << nrOfPacketsToRead << " subpackets\n";
      for (int i = 0; i < nrOfPacketsToRead; ++i) {
        auto [pack, lengthRead] = makePacket(input, readPos);
        ret.packets.push_back(pack);
        readPos += lengthRead;
      }
    } else {
      int nrOfBitsToRead = toInt(input, readPos, 15);
      readPos += 15;
      std::cout << "Reading " << nrOfBitsToRead << " bits of subpackts\n";
      auto endPos = readPos + nrOfBitsToRead;
      while (readPos < endPos) {

        auto [pack, lengthRead] = makePacket(input, readPos);
        // std::cout << " Current pos " << readPos << " new package " <<
        // lengthRead
        //           << "\n";
        ret.packets.push_back(pack);
        readPos += lengthRead;
      }
      if (readPos != endPos) {
        std::cout << "Error with number of bits?\n";
      }
    }
  }
  for (size_t i = start; i < readPos; ++i) {
    ret.data.push_back(input.at(i));
  }
  return {ret, readPos - start};
}

int versionSum(const Packet &packet) {
  auto ret = packet.version;
  for (auto &sub : packet.packets) {
    ret += versionSum(sub);
  }
  return ret;
}

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }

  std::vector<Packet> packets;
  std::string in;
  int totalSum = 0;
  while (input >> in) {
    std::cout << "working on >>" << in << "<<\n";
    auto bits = makeBitVector(in);
    auto [pack, read] = makePacket(bits, 0);
    packets.push_back(pack);
    std::cout << "used " << read << " of " << bits.size() << " bits\n";
    // std::cout << versionSum(pack) << " version sum\n";
    std::cout << pack.getValue() << " value\n";
    totalSum += versionSum(pack);
  }
  std::cout << "Result " << packets.at(0).getValue() << "\n";

  return 0;
}
