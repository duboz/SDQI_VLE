/** 
 * @file vpz/NoVLEs.cpp
 * @brief 
 * @author The vle Development Team
 * @date mer, 22 fév 2006 12:23:26 +0100
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

#include <vle/vpz/NoVLEs.hpp>
#include <vle/vpz/Model.hpp>
#include <vle/utils/XML.hpp>
#include <vle/utils/Debug.hpp>
#include <utility>

namespace vle { namespace vpz {

using namespace vle::utils;

void NoVLEs::write(std::ostream& out) const
{
    if (not empty()) {
        out << "<translators>\n";
        for (const_iterator it = begin(); it != end(); ++it) {
            out << it->second;
        }
        out << "</translators>\n";
    }
}

NoVLE& NoVLEs::add(const NoVLE& novle)
{
    const_iterator it = find(novle.name());
    Assert(utils::SaxParserError, it == end(),
           (boost::format("NoVLE %1% already exist") % novle.name()));

    return (*insert(std::make_pair < std::string, NoVLE >(
            novle.name(), novle)).first).second;
}

void NoVLEs::del(const std::string& modelname)
{
    iterator it = find(modelname);
    if (it != end()) {
        erase(it);
    }
}

void NoVLEs::fusion(const Project& prj,
                    Model& model,
                    Dynamics& dynamics,
                    Conditions& conditions,
                    Views& views)
{
    AtomicModelList::iterator it = model.atomicModels().begin();
    while (it != model.atomicModels().end()) {
       if (not it->second.translator().empty()) {
           graph::Model* mdl(it->first);
           const std::string& trl(it->second.translator());
           NoVLE& novle(get(trl));
           novle.callTranslator(prj, mdl, model, dynamics, conditions, views);
           AtomicModelList::iterator todel = it;
           ++it;
           model.atomicModels().erase(todel);
       } else {
           ++it;
       }
    }
}

const NoVLE& NoVLEs::get(const std::string& novle) const
{
    const_iterator it = find(novle);
    Assert(utils::SaxParserError, it != end(),
           (boost::format("The NoVLE %1% not exist.") % novle));
    return it->second;
}

NoVLE& NoVLEs::get(const std::string& novle)
{
    iterator it = find(novle);
    Assert(utils::SaxParserError, it != end(),
           (boost::format("The NoVLE %1% not exist.") % novle));
    return it->second;
}

}} // namespace vle vpz

