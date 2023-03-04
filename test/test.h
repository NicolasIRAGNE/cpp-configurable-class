/**
 * @file test.h
 * @author Nicolas IRAGNE (nicolas.iragne@alyce.fr)
 * @brief 
 * @date 2023-03-04
 * 
 * @copyright Copyright Alyce (c) 2023
 */

#pragma once

#include <niragne/configurable.h>
#include <string>

namespace niragne::test
{

class Printer
{
private:
    std::string m_data { "UNINITIALIZED" };
    int m_padding { 0 };

public:
    Printer() = default;
    void print() const
    {
        std::cout << "PRINTER: ";
        for (int i = 0; i < m_padding; ++i)
        {
            std::cout << " ";
        }
        std::cout << m_data << std::endl;
    }
    friend class niragne::config::ConfigRegistry;
};

} // namespace niragne::test

namespace niragne::config
{

template <>
bool niragne::config::ConfigRegistry::load<niragne::test::Printer>(niragne::test::Printer& printer);

} // namespace niragne::config