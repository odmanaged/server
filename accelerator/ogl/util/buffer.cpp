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

#include "../../StdAfx.h"

#include "buffer.h"

#include "texture.h"
#include "device.h"

#include <common/except.h>
#include <common/gl/gl_check.h>
#include <common/timer.h>

#include <GL/glew.h>

#include <boost/property_tree/ptree.hpp>

#include <atomic>
namespace caspar { namespace accelerator { namespace ogl {

static std::atomic<int>			g_w_total_count;
static std::atomic<std::size_t>	g_w_total_size;
static std::atomic<int>			g_r_total_count;
static std::atomic<std::size_t>	g_r_total_size;

struct buffer::impl : boost::noncopyable
{	
	GLuint						pbo_;
	const std::size_t			size_;
	std::atomic<uint8_t*>		data_;
	GLenum						usage_;
	GLenum						target_;

public:
	impl(std::size_t size, buffer::usage usage) 
		: size_(size)
		, target_(usage == buffer::usage::write_only ? GL_PIXEL_UNPACK_BUFFER : GL_PIXEL_PACK_BUFFER)
		, usage_(usage == buffer::usage::write_only ? GL_STREAM_DRAW : GL_STREAM_READ)
	{
		CASPAR_LOG_CALL(trace) << "buffer::buffer() <- " << get_context();
		caspar::timer timer;

		data_ = nullptr;
		GL(glGenBuffers(1, &pbo_));
		bind();	
		GL(glBufferData(target_, size_, NULL, usage_));		
		if(usage_ == GL_STREAM_DRAW)
		{
			auto result = GL2(glMapBuffer(target_, usage_ == GL_STREAM_DRAW ? GL_WRITE_ONLY : GL_READ_ONLY));
			data_ = reinterpret_cast<uint8_t*>(result);
		}
		unbind();

		if(!pbo_)
			CASPAR_THROW_EXCEPTION(caspar_exception() << msg_info("Failed to allocate buffer."));

		(usage == buffer::usage::write_only ? g_w_total_count : g_r_total_count)	++;
		(usage == buffer::usage::write_only ? g_w_total_size : g_r_total_size)		+= size_;
		
		if(timer.elapsed() > 0.02)
			CASPAR_LOG(warning) << L"[buffer] Performance warning. Buffer allocation blocked: " << timer.elapsed();
	
		//CASPAR_LOG(trace) << "[buffer] [" << ++(usage_ == buffer::usage::write_only ? g_w_total_count : g_r_total_count) << L"] allocated size:" << size_ << " usage: " << (usage == buffer::usage::write_only ? "write_only" : "read_only");
	}	

	~impl()
	{
		CASPAR_LOG_CALL(trace) << "buffer::~buffer() <- " << get_context();
		glDeleteBuffers(1, &pbo_);
		(usage_ == GL_STREAM_DRAW ? g_w_total_size : g_r_total_size)	-= size_;
		(usage_ == GL_STREAM_DRAW ? g_w_total_count : g_r_total_count)	--;
	}

	void* map()
	{
		if(data_ != nullptr)
			return data_;
		
		caspar::timer timer;

		GL(glBindBuffer(target_, pbo_));
		if(usage_ == GL_STREAM_DRAW)			
			GL(glBufferData(target_, size_, NULL, usage_));	// Notify OpenGL that we don't care about previous data.
		
		auto result = GL2(glMapBuffer(target_, usage_ == GL_STREAM_DRAW ? GL_WRITE_ONLY : GL_READ_ONLY));
		data_ = (uint8_t*) result;

		if(timer.elapsed() > 0.02)
			CASPAR_LOG(warning) << L"[buffer] Performance warning. Buffer mapping blocked: " << timer.elapsed();

		GL(glBindBuffer(target_, 0));
		if(!data_)
			CASPAR_THROW_EXCEPTION(invalid_operation() << msg_info("Failed to map target OpenGL Pixel Buffer Object."));

		return data_;
	}
	
	void unmap()
	{
		if(data_ == nullptr)
			return;
		
		GL(glBindBuffer(target_, pbo_));
		GL(glUnmapBuffer(target_));	
		if(usage_ == GL_STREAM_READ)			
			GL(glBufferData(target_, size_, NULL, usage_));	// Notify OpenGL that we don't care about previous data.
		data_ = nullptr;	
		GL(glBindBuffer(target_, 0));
	}

	void bind()
	{
		GL(glBindBuffer(target_, pbo_));
	}

	void unbind()
	{
		GL(glBindBuffer(target_, 0));
	}
};

buffer::buffer(std::size_t size, usage usage) : impl_(new impl(size, usage)){}
buffer::buffer(buffer&& other) : impl_(std::move(other.impl_)){}
buffer::~buffer(){}
buffer& buffer::operator=(buffer&& other){impl_ = std::move(other.impl_); return *this;}
uint8_t* buffer::data(){return impl_->data_;}
void buffer::map(){impl_->map();}
void buffer::unmap(){impl_->unmap();}
void buffer::bind() const{impl_->bind();}
void buffer::unbind() const{impl_->unbind();}
std::size_t buffer::size() const { return impl_->size_; }
int buffer::id() const {return impl_->pbo_;}

boost::property_tree::wptree buffer::info()
{
	boost::property_tree::wptree info;

	info.add(L"total_read_count", g_r_total_count);
	info.add(L"total_write_count", g_w_total_count);
	info.add(L"total_read_size", g_r_total_size);
	info.add(L"total_write_size", g_w_total_size);

	return info;
}

}}}
