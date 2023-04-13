////////////////////////////////////////////////////////////////////////////////
//   _______                               ________________________________   //
//   \\ .   \                     ________/ . . . . . . . . . . . . . .   /   //
//    \\ .   \     ____       ___/ . . . . .   __________________________/    //
//     \\ .   \   //   \   __/. . .  _________/   /    // .  _________/       //
//      \\ .   \_//     \_//     ___/.  _____    /    // .  /_____            //
//       \\ .   \/   _   \/    _/// .  /    \\   |    \\  .       \           //
//        \\ .      /\\       /  || .  |    ||   |     \\______    \          //
//         \\ .    /  \\     /   || .  \____//   |  _________//    /          //
//          \\ .  /    \\   /    //  .           / // . . . .     /           //
//           \\__/      \\_/    //______________/ //_____________/            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//   This is free and unencumbered software released into the public domain.  //
//                                                                            //
//   Anyone is free to copy, modify, publish, use, compile, sell, or          //
//   distribute this software, either in source code form or as a compiled    //
//   binary, for any purpose, commercial or non-commercial, and by any        //
//   means.                                                                   //
//                                                                            //
//   In jurisdictions that recognize copyright laws, the author or authors    //
//   of this software dedicate any and all copyright interest in the          //
//   software to the public domain. We make this dedication for the benefit   //
//   of the public at large and to the detriment of our heirs and             //
//   successors. We intend this dedication to be an overt act of              //
//   relinquishment in perpetuity of all present and future rights to this    //
//   software under copyright law.                                            //
//                                                                            //
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          //
//   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       //
//   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   //
//   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR        //
//   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,    //
//   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR    //
//   OTHER DEALINGS IN THE SOFTWARE.                                          //
//                                                                            //
//   For more information, please refer to <https://unlicense.org>            //
////////////////////////////////////////////////////////////////////////////////
//    WOS : Web Operating System                                              //
//     Math/Vector2i.h : 2 components int vector management                   //
////////////////////////////////////////////////////////////////////////////////
#ifndef WOS_MATH_VECTOR2I_HEADER
#define WOS_MATH_VECTOR2I_HEADER

    #include "../System/System.h"
    #include "Math.h"

    #include <cstdint>


    ////////////////////////////////////////////////////////////////////////////
    //  Vector2i class definition                                             //
    ////////////////////////////////////////////////////////////////////////////
    class Vector2i
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  Vector2i default constructor                                  //
            ////////////////////////////////////////////////////////////////////
            Vector2i()
            {
                vec[0] = 0;
                vec[1] = 0;
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i copy constructor                                     //
            ////////////////////////////////////////////////////////////////////
            Vector2i(const Vector2i& vector)
            {
                vec[0] = vector.vec[0];
                vec[1] = vector.vec[1];
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i components constructor                               //
            ////////////////////////////////////////////////////////////////////
            Vector2i(int64_t x, int64_t y)
            {
                vec[0] = x;
                vec[1] = y;
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i destructor                                           //
            ////////////////////////////////////////////////////////////////////
            ~Vector2i()
            {
                vec[0] = 0;
                vec[1] = 0;
            }


            ////////////////////////////////////////////////////////////////////
            //  Reset Vector2i to zero                                        //
            ////////////////////////////////////////////////////////////////////
            inline void reset()
            {
                vec[0] = 0;
                vec[1] = 0;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set Vector2i components from a vector                         //
            ////////////////////////////////////////////////////////////////////
            inline void set(const Vector2i& vector)
            {
                vec[0] = vector.vec[0];
                vec[1] = vector.vec[1];
            }

            ////////////////////////////////////////////////////////////////////
            //  Set Vector2i components from X and Y components               //
            ////////////////////////////////////////////////////////////////////
            inline void set(int64_t x, int64_t y)
            {
                vec[0] = x;
                vec[1] = y;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set Vector2i X component                                      //
            ////////////////////////////////////////////////////////////////////
            inline void setX(int64_t x)
            {
                vec[0] = x;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set Vector2i Y component                                      //
            ////////////////////////////////////////////////////////////////////
            inline void setY(int64_t y)
            {
                vec[1] = y;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get the dot product of this vector and another                //
            ////////////////////////////////////////////////////////////////////
            inline int64_t dotProduct(Vector2i& v) const
            {
                return ((vec[0]*v.vec[0]) + (vec[1]*v.vec[1]));
            }

            ////////////////////////////////////////////////////////////////////
            //  Get Vector2i length                                           //
            ////////////////////////////////////////////////////////////////////
            inline int64_t length() const
            {
                return Math::sqrt((vec[0]*vec[0]) + (vec[1]*vec[1]));
            }

            ////////////////////////////////////////////////////////////////////
            //  Normalize Vector2i                                            //
            ////////////////////////////////////////////////////////////////////
            inline void normalize()
            {
                int64_t len = length();
                if (len > 0)
                {
                    // Normalize between 0 and Math::OneInt
                    vec[0] = (vec[0] << Math::OneIntShift) / len;
                    vec[1] = (vec[1] << Math::OneIntShift) / len;
                }
            }


            ////////////////////////////////////////////////////////////////////
            //  Get Vector2i x component                                      //
            //  return : X component of the vector                            //
            ////////////////////////////////////////////////////////////////////
            inline int64_t& x()
            {
                return vec[0];
            }

            ////////////////////////////////////////////////////////////////////
            //  Get Vector2i y component                                      //
            //  return : Y component of the vector                            //
            ////////////////////////////////////////////////////////////////////
            inline int64_t& y()
            {
                return vec[1];
            }


            ////////////////////////////////////////////////////////////////////
            //  Vector2i affectation operator                                 //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i& operator=(const Vector2i& vector)
            {
                vec[0] = vector.vec[0];
                vec[1] = vector.vec[1];
                return *this;
            }

            inline Vector2i& operator=(int64_t val)
            {
                vec[0] = val;
                vec[1] = val;
                return *this;
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i addition operator                                    //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i operator+(const Vector2i& vector) const
            {
                return Vector2i(vec[0]+vector.vec[0], vec[1]+vector.vec[1]);
            }

            inline Vector2i operator+(int64_t val) const
            {
                return Vector2i(vec[0]+val, vec[1]+val);
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i subtraction operator                                 //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i operator-(const Vector2i& vector) const
            {
                return Vector2i(vec[0]-vector.vec[0], vec[1]-vector.vec[1]);
            }

            inline Vector2i operator-(int64_t val) const
            {
                return Vector2i(vec[0]-val, vec[1]-val);
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i unary plus operator                                  //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i operator+() const
            {
                return Vector2i(*this);
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i unary minus operator                                 //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i operator-() const
            {
                return Vector2i(-vec[0], -vec[1]);
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i multiplication operator                              //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i operator*(const Vector2i& vector) const
            {
                return Vector2i(vec[0]*vector.vec[0], vec[1]*vector.vec[1]);
            }

            inline Vector2i operator*(int64_t val) const
            {
                return Vector2i(vec[0]*val, vec[1]*val);
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i division operator                                    //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i operator/(const Vector2i& vector) const
            {
                Vector2i result(*this);
                if (vector.vec[0] != 0)
                {
                    result.vec[0] /= vector.vec[0];
                }
                if (vector.vec[1] != 0)
                {
                    result.vec[1] /= vector.vec[1];
                }
                return result;
            }

            inline Vector2i operator/(int64_t val) const
            {
                Vector2i result(*this);
                if (val != 0)
                {
                    result.vec[0] /= val;
                    result.vec[1] /= val;
                }
                return result;
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i prefix increment operator                            //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i& operator++()
            {
                ++vec[0];
                ++vec[1];
                return *this;
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i postfix increment operator                           //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i operator++(int)
            {
                return Vector2i(vec[0]++, vec[1]++);
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i prefix decrement operator                            //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i& operator--()
            {
                --vec[0];
                --vec[1];
                return *this;
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i postfix decrement operator                           //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i operator--(int)
            {
                return Vector2i(vec[0]--, vec[1]--);
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i addition assignment operator                         //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i& operator+=(const Vector2i& vector)
            {
                vec[0] += vector.vec[0];
                vec[1] += vector.vec[1];
                return *this;
            }

            inline Vector2i& operator+=(int64_t val)
            {
                vec[0] += val;
                vec[1] += val;
                return *this;
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i subtraction assignment operator                      //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i& operator-=(const Vector2i& vector)
            {
                vec[0] -= vector.vec[0];
                vec[1] -= vector.vec[1];
                return *this;
            }

            inline Vector2i& operator-=(int64_t val)
            {
                vec[0] -= val;
                vec[1] -= val;
                return *this;
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i multiplication assignment operator                   //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i& operator*=(const Vector2i& vector)
            {
                vec[0] *= vector.vec[0];
                vec[1] *= vector.vec[1];
                return *this;
            }

            inline Vector2i& operator*=(int64_t val)
            {
                vec[0] *= val;
                vec[1] *= val;
                return *this;
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i division assignment operator                         //
            ////////////////////////////////////////////////////////////////////
            inline Vector2i& operator/=(const Vector2i& vector)
            {
                if (vector.vec[0] != 0)
                {
                    vec[0] /= vector.vec[0];
                }
                if (vector.vec[1] != 0)
                {
                    vec[1] /= vector.vec[1];
                }
                return *this;
            }

            inline Vector2i& operator/=(int64_t val)
            {
                if (val != 0)
                {
                    vec[0] /= val;
                    vec[1] /= val;
                }
                return *this;
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i equal to operator                                    //
            ////////////////////////////////////////////////////////////////////
            inline bool operator==(const Vector2i& vector) const
            {
                return ((vec[0] == vector.vec[0]) && (vec[1] == vector.vec[1]));
            }

            ////////////////////////////////////////////////////////////////////
            //  Vector2i not equal to operator                                //
            ////////////////////////////////////////////////////////////////////
            inline bool operator!=(const Vector2i& vector) const
            {
                return ((vec[0] != vector.vec[0]) || (vec[1] != vector.vec[1]));
            }


        public:
            int64_t     vec[2];     // 2 components int vector representation
    };


#endif // WOS_MATH_VECTOR2I_HEADER
