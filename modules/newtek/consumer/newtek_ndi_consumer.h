/*
 * Copyright 2013 Sveriges Television AB http://casparcg.com/
 *
 * This file is part of CasparCG (www.casparcg.com).
 *
 * CasparCG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CasparCG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CasparCG. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Krzysztof Zegzula, zegzulakrzysztof@gmail.com
 * based on work of Robert Nagy, ronag89@gmail.com work
 */

#pragma once

#include <common/memory.h>

#include <core/fwd.h>

#include <boost/property_tree/ptree_fwd.hpp>

#include <string>

namespace caspar { namespace newtek {

void describe_consumer(core::help_sink& sink, const core::help_repository& repo);
spl::shared_ptr<core::frame_consumer> create_ndi_consumer( 
                const std::vector<std::wstring>& params, core::interaction_sink*,
                std::vector<spl::shared_ptr<core::video_channel>> channels);
spl::shared_ptr<core::frame_consumer> create_preconfigured_ndi_consumer(
                const boost::property_tree::wptree& ptree, core::interaction_sink*,
                std::vector<spl::shared_ptr<core::video_channel>> channels);
}} // namespace caspar::newtek
