#pragma once
#include "../StateTests/Filler/InfoIncomplete.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
#include <retesteth/testStructures/basetypes.h>
#include <retesteth/testStructures/configs/FORK.h>
#include <map>

namespace test
{
namespace teststruct
{
struct DifficultyTestVector
{
    DifficultyTestVector(spDataObject const&);
    spVALUE currentBlockNumber;
    spVALUE currentDifficulty;
    spVALUE currentTimestamp;
    spVALUE parentDifficulty;
    spVALUE parentTimestamp;
    bool parentUncles;
    string testVectorName;
};

typedef std::vector<DifficultyTestVector> TestVector;

struct DifficultyTestInFilled : GCP_SPointerBase
{
    DifficultyTestInFilled(spDataObject&);
    string const& testName() const { return m_name; }
    std::map<string, TestVector> const& testVectors() const { return m_testVectors; }

private:
    DifficultyTestInFilled() {}
    string m_name;
    std::map<string, TestVector> m_testVectors;
};

struct DifficultyTest
{
    DifficultyTest(spDataObject&);
    std::vector<DifficultyTestInFilled> const& tests() const { return m_tests; }

private:
    DifficultyTest() {}
    std::vector<DifficultyTestInFilled> m_tests;
};


}  // namespace teststruct
}  // namespace test
