#ifndef SFUTILS_HPP_INCLUDED
#define SFUTILS_HPP_INCLUDED

#include <cmath>

// --------------------------------------------
//   CALCULATIONS ON VECTORS
// --------------------------------------------

// ##################### DISTANCE #####################
/*
 * Calculates distance between two points
 * Args: sf::Vector vec1, sf::Vector vec2
 * Returns float
 */
template <typename T, typename C>
float dist(const sf::Vector2<T>& vec1, const sf::Vector2<C>& vec2)
{
    return sqrt(pow(vec2.x - vec1.x, 2) + (pow(vec2.y - vec1.y, 2)));
}

// #################### MAGNITUDE #####################
/*
 * Calculates magnitude of a vector
 * Args: sf::Vector vec
 * Returns float
 */
template <typename T>
float mag(const sf::Vector2<T>& vec)
{
    return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
}

/*
 * Sets new magnitude to a vector
 * Args: sf::Vector vector, any value
 */
template <typename T, typename C>
void setMag(sf::Vector2<T>& vec, C value)
{
    float scale (mag(vec));
    scale = value / scale;
    vec *= scale;
}

// ################# NORMALIZE ########################
/*
 * Normalizes vector (sets magnitude to 1)
 * (normalizing for sf::Vector2<int> is not possible, because its components will always be (0, 0))
 * Args: sf::Vector vec
 */
void normalize(sf::Vector2<float>& vec)
{
    if(mag(vec) != 0)
    {
        float length {mag(vec)};
        vec.x /= length;
        vec.y /= length;
    }
}
void normalize(sf::Vector2<double>& vec)
{
    if(mag(vec) != 0)
    {
        float length {mag(vec)};
        vec.x /= length;
        vec.y /= length;
    }
}

// #################### LIMIT #########################
/*
 * Limits magnitude of a vector (if greater)
 * Args: sf::Vector vec, any value
 */
template <typename T, typename C>
void limit(sf::Vector2<T>& vec, C value)
{
    if(mag(vec) > value)
    {
        setMag(vec, value);
    }
}

// ##################### DOT ##########################
/*
 * Calculates dot product of two vectors
 * Args: sf::Vector vec1, sf::Vector vec2
 * Returns float
 */
template <typename T, typename C>
float dot(const sf::Vector2<T>& vec1, const sf::Vector2<C>& vec2)
{
    return ((vec1.x * vec2.x) + (vec1.y * vec2.y));
}

// --------------------------------------------
//   HSV TO RGB CONVERTING
// --------------------------------------------
/*
 * Converts HSV value to RGB value
 * Args: float h (0 - 360), float s (0, 100), float v (0, 100)
 * Returns sf::Color
 */
sf::Color fromHsv(float h, float s, float v)
{
	s = s / 100.0f;
	v = v / 100.0f;
	float c {v * s};
	float x {c * (1.0f - static_cast<float>(fabs(static_cast<float>(fmod(h / 60.0f, 2) - 1.0f))))};
	float m {v - c};

	float r {0}, g {0}, b {0};
	if(0 <= h && h < 60)         r = c, g = x, b = 0;
	else if(60 <= h && h < 120)  r = x, g = c, b = 0;
	else if(120 <= h && h < 180) r = 0, g = c, b = x;
	else if(180 <= h && h < 240) r = 0, g = x, b = c;
	else if(240 <= h && h < 300) r = x, g = 0, b = c;
	else                         r = c, g = 0, b = x;

	r = (r + m) * 255;
	g = (g + m) * 255;
	b = (b + m) * 255;

	sf::Color result {static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b)};
	return result;
}

// --------------------------------------------
//   ANGLES CONVERTING
// --------------------------------------------
// ##################### RADIANS TO DEGREES ##########################
/*
 * Converts radians to degrees
 * Args: float radians
 * Returns float
 */
float degrees(float radians)
{
    float sfutilsPi {3.14159265359};
    return radians * 180.0f / sfutilsPi;
}
// ##################### DEGREES TO RADIANS ##########################
/*
 * Converts degrees to radians
 * Args: float degrees
 * Returns float
 */
float radians(float degrees)
{
    float sfutilsPi {3.14159265359};
    return degrees * sfutilsPi / 180.0f;
}


// --------------------------------------------
//   HASH FUNCTION FOR sf::Vector2i
// --------------------------------------------
namespace std
{
template<>
struct hash<sf::Vector2i>
{
    size_t operator()(const sf::Vector2i& vec) const
    {
        return ((hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1)) >> 1);
    }
};
}

#endif // SFUTILS_HPP_INCLUDED
