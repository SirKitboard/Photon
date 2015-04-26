#pragma once

#include <random>
#include <string>

/// A bunch of simple utility functions. Feel free to add some here.
/// @author Jesse Talavera-Greenberg

namespace cse380 {
  namespace sssf {
    namespace util {
      using std::default_random_engine;
      using std::random_device;
      using std::string;
      using std::wstring;

      template<class NumberType, class MinType, class MaxType>
      NumberType clamp(const NumberType value, const MinType min, const MaxType max) {
        // Note: template classes or methods must be defined in headers
        if (value < min) return min;
        if (value > max) return max;
        return value;
      }

      template<class VectorTypeOut, class VectorTypeIn>
      /// <summary>
      /// Given two types, each with public members "x" and "y" (like mathematical
      /// vectors often have), converts one type to another.  You'll often find
      /// yourself converting between Box2D vectors and your media framework's vector
      /// (i.e. sf::Vector2 for SFML, POINT for DirectX).
      /// </summary>
      VectorTypeOut convertVector(const VectorTypeIn& in) {
        return VectorTypeOut(in.x, in.y);
      }

      std::string to_string(const std::wstring& w);
      std::wstring to_wstring(const std::string& s);

      namespace random {
        /// Convenience definition for the OS's random device; use this if you
        /// need "true" random numbers, like for security purposes or for initializing
        /// pseudo-RNGs.
        extern std::random_device RANDOM_DEVICE;

        /// Convenience definition for the random number generator. Prefer using
        /// this for games, as it's much faster (but slightly less random; not
        /// an issue if you're not using it for security purposes, though).
        extern std::default_random_engine RANDOM;
      }
    }
  }
}