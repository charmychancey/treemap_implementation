#include <gtest/gtest.h>
#include "treemap.h"

TEST(Treemap, Empty) {
  Treemap<int, int> map;

  EXPECT_EQ(map.Empty(), true);
  EXPECT_EQ(map.Size(), 0);
  EXPECT_THROW(map.Get(42), std::exception);
}

TEST(Treemap, OneKey) {
  Treemap<int, char> map;

  map.Insert(23, 'A');
  map.Insert(42, 'B');
  EXPECT_EQ(map.Empty(), false);
  EXPECT_EQ(map.Size(), 2);
  EXPECT_EQ(map.MinKey(), 23);
  EXPECT_EQ(map.MaxKey(), 42);
  EXPECT_EQ(map.Get(23), 'A');
}

TEST(Treemap, Size) {
  Treemap<int, char> map;

  map.Insert(10, 'C');
  EXPECT_EQ(map.Size(), 1);
}

TEST(Treemap, Insert2Remove1AndGet) {
  Treemap<int, char> map;

  map.Insert(1, 'A');
  map.Insert(2, 'B');
  map.Insert(3, 'C');
  map.Remove(3);
  EXPECT_EQ(map.Size(), 2);
  EXPECT_EQ(map.Get(2), 'B');
  EXPECT_EQ(map.Get(1), 'A');
}

TEST(Treemap, MultRemoveInitAndInsertSort) {
  Treemap<int, char> map;

  map.Insert(4, '4');
  map.Insert(1, '1');
  map.Insert(5, '5');
  map.Insert(2, '2');
  map.Insert(3, '3');
  map.Remove(4);
  map.Remove(2);
  EXPECT_EQ(map.Size(), 3);
  EXPECT_EQ(map.MaxKey(), 5);
  EXPECT_EQ(map.MinKey(), 1);
  EXPECT_EQ(map.Get(1), '1');
  EXPECT_EQ(map.Get(3), '3');
  EXPECT_EQ(map.Get(5), '5');
}

TEST(Treemap, ContainsKeyContainsValue) {
  Treemap<int, char> map;

  map.Insert(4, '4');
  map.Insert(1, '1');
  map.Insert(5, '5');
  map.Insert(2, '2');
  map.Insert(3, '3');
  map.Remove(4);
  map.Remove(2);
  EXPECT_EQ(map.Get(5), '5');
  EXPECT_EQ(map.ContainsValue('4'), false);
  EXPECT_EQ(map.ContainsValue('3'), true);
  EXPECT_EQ(map.ContainsKey(4), false);
  EXPECT_EQ(map.ContainsKey(3), true);
}

TEST(Treemap, FindCeil) {
  Treemap<int, char> map;

  map.Insert(4, '4');
  map.Insert(1, '1');
  map.Insert(5, '5');
  map.Insert(2, '2');
  map.Insert(3, '3');
  map.Remove(4);
  map.Remove(3);
  EXPECT_EQ(map.Size(), 3);
  EXPECT_EQ(map.CeilKey(4), 5);
  EXPECT_EQ(map.CeilKey(3), 5);
  EXPECT_EQ(map.CeilKey(2), 2);
}

TEST(Treemap, FindFloor) {
  Treemap<int, char> map;

  map.Insert(4, '4');
  map.Insert(1, '1');
  map.Insert(5, '5');
  map.Insert(2, '2');
  map.Insert(3, '3');
  map.Remove(4);
  map.Remove(3);
  EXPECT_EQ(map.Size(), 3);
  EXPECT_EQ(map.FloorKey(4), 2);
  EXPECT_EQ(map.FloorKey(3), 2);
  EXPECT_EQ(map.FloorKey(2), 2);
}

TEST(Treemap, ErrorsFloorAndCeil) {
  Treemap<int, char> map;

  map.Insert(4, '4');
  map.Insert(1, '1');
  map.Insert(5, '5');
  map.Insert(2, '2');
  map.Insert(3, '3');
  map.Remove(4);
  map.Remove(3);
  EXPECT_EQ(map.Size(), 3);
  EXPECT_THROW(map.FloorKey(0), std::invalid_argument);
  EXPECT_THROW(map.CeilKey(10), std::invalid_argument);
}

TEST(Treemap, MaxAndMin) {
  Treemap<int, char> map;

  map.Insert(4, '4');
  map.Insert(1, '1');
  map.Insert(5, '5');
  map.Insert(2, '2');
  EXPECT_EQ(map.Size(), 4);
  EXPECT_EQ(map.MaxKey(), 5);
  EXPECT_EQ(map.MinKey(), 1);
}

TEST(Treemap, ErrorsMoreErrors) {
  Treemap<int, char> map;

  map.Insert(4, '4');
  map.Insert(1, '1');
  map.Insert(5, '5');
  map.Insert(2, '2');
  map.Insert(3, '3');
  map.Remove(3);
  EXPECT_EQ(map.Size(), 4);
  EXPECT_THROW(map.Insert(4, 'h'), std::invalid_argument);
  EXPECT_THROW(map.Get(0), std::invalid_argument);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}