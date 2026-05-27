#include "RandomNumberGenerator.h"
#include "HAL/PlatformTime.h"

RandomNumberGenerator::RandomNumberGenerator(): m_uSeed(FPlatformTime::Cycles64())
{
  if (this->m_uSeed == 0) 
  {
    this->m_uSeed = 1; // Ensure seed is never zero
  }
}

RandomNumberGenerator::RandomNumberGenerator(uint64_t _uSeed)
{
  if (_uSeed <= 0)
  {
    // we ensure that the seed is never zero or negative
    m_uSeed = 1;
  }
  else
  {
    m_uSeed = _uSeed;
  }
}

int RandomNumberGenerator::GetRandomInRange(int _min, int _max)
{
  if (_min > _max)
  {
    std::swap(_min, _max); // This is just in case the given range is backwards
  }
  uint64_t range = static_cast<uint64_t>(_max) - static_cast<uint64_t>(_min) + 1;
  return _min + static_cast<int>(XorShift64() % range);
}

int RandomNumberGenerator::GetRandom(int _max)
{
  return GetRandomInRange(1, _max);
}

int RandomNumberGenerator::GetRandomInIntervals(const std::vector<std::pair<int, int>>& _intervals)
{
  if (_intervals.empty())
  {
    throw std::invalid_argument("Intervals vector is empty");
  }

  int totalSize = 0;
  int rangeSize = 0;

  for (const auto& range : _intervals)
  {
    totalSize += (range.second - range.first + 1);
  }

  int randIndex = XorShift64() % totalSize;

  for (const auto& range : _intervals)
  {
    rangeSize = range.second - range.first + 1;
    if (randIndex < rangeSize)
    {
      return range.first + randIndex;
    }
    randIndex -= rangeSize;
  }

  return _intervals.back().second;
}

float RandomNumberGenerator::GetRandomFloatInRange(float _min, float _max)
{
  if (_min > _max)
  {
    std::swap(_min, _max);
  }

  double normalized = static_cast<double>(XorShift64()) / static_cast<double>(UINT64_MAX);
  return _min + static_cast<float>((_max - _min) * normalized);
}

float RandomNumberGenerator::GetRandomFloat(float _max)
{
  return GetRandomFloatInRange(0.0f, _max);
}

uint64_t RandomNumberGenerator::GetSeed() const
{
  return m_uSeed;
}

uint64_t RandomNumberGenerator::XorShift64()
{
  m_uSeed ^= m_uSeed >> 12;
  m_uSeed ^= m_uSeed << 25;
  m_uSeed ^= m_uSeed >> 27;
  return m_uSeed * 0x2545F4914F6CDD1DULL; // Multiplier for better mixing
}
