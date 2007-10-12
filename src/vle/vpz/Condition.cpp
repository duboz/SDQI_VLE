/** 
 * @file vpz/Condition.cpp
 * @brief 
 * @author The vle Development Team
 * @date mar, 31 jan 2006 17:28:49 +0100
 */

/*
 * Copyright (C) 2006 - The vle Development Team
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <vle/utils/XML.hpp>
#include <vle/utils/Debug.hpp>
#include <vle/value/Value.hpp>
#include <vle/vpz/Condition.hpp>
#include <vle/value/Set.hpp>

namespace vle { namespace vpz {

using namespace vle::utils;

const value::Value& ValueList::get(const std::string& name) const
{
    const_iterator it = find(name);
    if (it == end()) {
	Throw(utils::InternalError, boost::format(
		  "Unknow port %1% for condition") % name);
    } 
    return it->second;
}

value::Value& ValueList::get(const std::string& name)
{
    iterator it = find(name);
    if (it == end()) {
	Throw(utils::InternalError, boost::format(
		  "Unknow port %1% for condition") % name);
    }
    return it->second;
}
	
Condition::Condition() :
    Base()
{
}

Condition::Condition(const std::string& name) :
    Base(),
    m_name(name)
{
}

void Condition::write(std::ostream& out) const
{
    out << "<condition name=\"" << m_name << "\" >\n";

    for (const_iterator it = begin(); it != end(); ++it) {
        out << " <port "
            << "name=\"" << it->first << "\" "
            << ">\n";

        const value::SetFactory::VectorValue& val(value::toSet(it->second));
        for (value::SetFactory::VectorValue::const_iterator jt = val.begin();
             jt != val.end(); ++jt) {
            out << (*jt)->toXML();
        }

        out << "</port>\n";
    }

    out << "</condition>\n";
}

void Condition::add(const std::string& portname)
{
    if (find(portname) == end()) {
        insert(std::make_pair < std::string, value::Set >(
                portname, value::SetFactory::create()));
    }
    m_last_port.assign(portname);
}

void Condition::del(const std::string& portname)
{
    erase(portname);
}

void Condition::addValueToPort(const std::string& portname,
                               const value::Value& value)
{
    iterator it = find(portname);

    if (it == end()) {
        value::Set newset = value::SetFactory::create();
        newset->addValue(value);

        insert(std::make_pair < std::string, value::Set >(
                portname, newset));
        m_last_port.assign(portname);
    } else {
        it->second->addValue(value);
    }
}

ValueList Condition::firstValues() const
{
    ValueList result;

    for (const_iterator it = begin(); it != end(); ++it) {
        if (it->second->size() > 0) {
            result[it->first] = it->second->getValue(0);
        } else {
            Throw(utils::SaxParserError, (boost::format(
                        "Build a empty first values for condition %1%.") %
                    m_name));
        }
    }

    return result;
}

const value::Set& Condition::getSetValues(const std::string& portname) const
{
    const_iterator it = find(portname);

    Assert(utils::InternalError, it != end(),
           boost::format("Condition %1% have no port %2%") %
           m_name % portname);

    return it->second;
}

const value::Value& Condition::firstValue(const std::string& portname) const
{
    return getSetValues(portname)->getValue(0);
}

const value::Value& Condition::nValue(const std::string& portname,
                                      size_t i) const
{
    return getSetValues(portname)->getValue(i);
}

value::Set& Condition::lastAddedPort()
{
    iterator it = find(m_last_port);

    Assert(utils::InternalError, it != end(),
           boost::format("Condition %1% have no port %2%") %
           m_name % m_last_port);

    return it->second;
}

void Condition::rebuildValueSet()
{
    for (iterator it = begin(); it != end(); ++it) {
        it->second = value::SetFactory::create();
    }
}

}} // namespace vle vpz
