/**
 * @file singleton.h
 * @author Nicolas IRAGNE (nicolas.iragne@alyce.fr)
 * @brief Template for singleton classes.
 * @date 2022-03-20
 *
 * @copyright Copyright niragne (c) 2022
 */

#pragma once

#include <memory>

namespace niragne::utils
{

template <typename T>
class Singleton
{
protected:
    Singleton() = default;

public:
    static T& getInstance()
    {
        static T instance;
        return instance;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
};

} // namespace niragne::utils