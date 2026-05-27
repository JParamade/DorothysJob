/************************************************************************
 * @description: Represents the Base Actor in the Game.
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
 
#pragma once


#include "CoreMinimal.h"


class DOROTHYSJOB_API RandomNumberGenerator
{
public:
  /**
   * @brief Constructor of Random Number Generator class
   * @param seed It's better not to give
   */
  RandomNumberGenerator();

  /**
   * @brief Constructor of Random Number Generator class
   * @param _uSeed The seed used to generate the same numbers
   */
  RandomNumberGenerator(uint64_t _uSeed);

  /**
   * @brief Gets a Random number between a given Range
   * @param _min Minimun value of the Range
   * @param _max Maximun value of the Range
   * @return Random number
   */
  int GetRandomInRange(int _min, int _max);

  /**
   * @brief Gets a Random number between 1 and max value
   * @param _max Maximun value the random number can be
   * @return Random number
   */
  int GetRandom(int _max);

  /**
   * @brief Gets a Random number between multiple intervals
   * @param _intervals Example: std::vector<std::pair<int, int>> intervals = { {0, 10}, {15, 20} };
   * @return Random Number
   */
  int GetRandomInIntervals(const std::vector<std::pair<int, int>>& _intervals);

  /**
 * @brief Gets a Random number between a given Range
 * @param _min Minimun value of the Range
 * @param _max Maximun value of the Range
 * @return Random number
 */
  float GetRandomFloatInRange(float _min, float _max);

  /**
   * @brief Gets a Random number between 1 and max value
   * @param _max Maximun value the random number can be
   * @return Random number
   */
  float GetRandomFloat(float _max);

  uint64_t GetSeed() const;
  
private:
  /**
   * @brief Seed that will generate the random number
   */
  uint64_t m_uSeed;

  /**
   * @brief Changes the Seed using a XOR-Shifting algorithm
   * @return Seed shifted
   */
  uint64_t XorShift64();
};
