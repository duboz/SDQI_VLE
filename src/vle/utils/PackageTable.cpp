/**
 * @file vle/utils/PackageTable.cpp
 * @author The VLE Development Team
 */

/*
 * VLE Environment - the multimodeling and simulation environment
 * This file is a part of the VLE environment (http://vle.univ-littoral.fr)
 * Copyright (C) 2003 - 2008 The VLE Development Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <vle/utils/PackageTable.hpp>
#include <vle/utils/Exception.hpp>

namespace vle { namespace utils {

PackageTable::PackageTable()
{
    m_current = m_table.insert(std::string()).first;
}

void PackageTable::current(const std::string& package)
{
    std::pair < const_iterator, bool > r = m_table.insert(package);
    if (r.second) {
        m_current = r.first;
    }
}

PackageTable::index PackageTable::get(const std::string& package)
{
    return m_table.insert(package).first;
}

void PackageTable::remove(index i)
{
    if (m_current == i) {
        throw utils::ArgError("PackageTable: Cannot remove currrent package");
    }

    m_table.erase(i);
}

}} // namespace vle utils