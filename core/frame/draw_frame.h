/*
* Copyright (c) 2011 Sveriges Television AB <info@casparcg.com>
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
* Author: Robert Nagy, ronag89@gmail.com
*/

#pragma once

#include "frame_visitor.h"
#include "../fwd.h"
#include "core/ancillary/ancillary.h"
#include <common/memory.h>

#include <vector>

namespace caspar { namespace core {
	
class draw_frame final
{
public:		
	// Static Members

	static draw_frame interlace(draw_frame frame1, draw_frame frame2, core::field_mode mode);
	static draw_frame over(draw_frame frame1, draw_frame frame2);
	static draw_frame mask(draw_frame fill, draw_frame key);
	static draw_frame still(draw_frame frame);
	static draw_frame push(draw_frame frame);
		
	static const draw_frame& empty();
	static const draw_frame& late();

	// Constructors

	draw_frame();
	draw_frame(const draw_frame& other);
	draw_frame(draw_frame&& other);	
	explicit draw_frame(const_frame&& frame);
	explicit draw_frame(mutable_frame&& frame);
	explicit draw_frame(std::vector<draw_frame> frames);

	~draw_frame();
	
	// Methods

	draw_frame& operator=(draw_frame other);

	void swap(draw_frame& other);	
	
	void accept(frame_visitor& visitor) const;
	
	int64_t get_and_record_age_millis();
	
	bool operator==(const draw_frame& other) const;
	bool operator!=(const draw_frame& other) const;

	caspar::core::ancillary::AncillaryContainer& ancillary();
	// Properties

	const core::frame_transform&	transform() const;
	core::frame_transform&			transform();

private:
	struct impl;
	spl::unique_ptr<impl> impl_;
};

}}

