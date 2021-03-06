#ifndef ANIMATION_H
#define ANIMATION_H

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include "vulkan-render/texture_loader.h"
#include "vulkan-render/vkhelper.h"
#include "timer.h"

struct Frame
{
	Resource::Texture tex;
	glm::vec4 textureOffset;
	glm::vec2 size;
	float delay;
};

class Animation
{
public:
	Animation(){};
	Animation(std::vector<Resource::Texture> textures, float delay)
	{
		for(const auto &tex: textures)
		{
			frames.push_back(Frame());
			frames.back().tex = tex;
			frames.back().textureOffset = glm::vec4(0, 0, 1, 1);
			frames.back().delay = delay;
			totalDuration += delay;
			frames.back().size = tex.dim;
		}
	}

	Animation(Resource::Texture texture, float delay, float FrameWidth)
	{
		frames.resize(texture.dim.x / FrameWidth);
		
		for(unsigned int i = 0; i < frames.size(); i++)
		{
			frames[i].tex = texture;
			frames[i].textureOffset = vkhelper::calcTexOffset
				(texture.dim, glm::vec4(i * FrameWidth, 0, FrameWidth, texture.dim.y));
			frames[i].delay = delay;
			totalDuration += delay;
			frames[i].size = glm::vec2(FrameWidth, texture.dim.y);
		}
	}

	Animation(Resource::Texture texture, float delay, float FrameWidth, bool invertX)
	{
		frames.resize(texture.dim.x / FrameWidth);
		
		for(unsigned int i = 0; i < frames.size(); i++)
		{
			frames[i].tex = texture;
			if(invertX)
			{
				frames[i].textureOffset = vkhelper::calcTexOffset
				(texture.dim, glm::vec4((i+1) * FrameWidth, 0, -FrameWidth, texture.dim.y));
			}
			else
			{
				frames[i].textureOffset = vkhelper::calcTexOffset
					(texture.dim, glm::vec4(i * FrameWidth, 0, FrameWidth, texture.dim.y));
			}
			frames[i].delay = delay;
			totalDuration += delay;
			frames[i].size = glm::vec2(FrameWidth, texture.dim.y);
		}
	}

	Animation(Resource::Texture texture, float delay, float FrameWidth, bool invertX, bool reverse)
	{
		frames.resize(texture.dim.x / FrameWidth);
		
		for(unsigned int i = 0; i < frames.size(); i++)
		{
			frames[i].tex = texture;
			if(invertX)
			{
				frames[i].textureOffset = vkhelper::calcTexOffset
				(texture.dim, glm::vec4((i+1) * FrameWidth, 0, -FrameWidth, texture.dim.y));
			}
			else
			{
				frames[i].textureOffset = vkhelper::calcTexOffset
					(texture.dim, glm::vec4(i * FrameWidth, 0, FrameWidth, texture.dim.y));
			}
			frames[i].delay = delay;
			totalDuration += delay;
			frames[i].size = glm::vec2(FrameWidth, texture.dim.y);
		}
		if(reverse)
			std::reverse(frames.begin(), frames.end());
	}
	
	Animation(Resource::Texture texture, float delay, float FrameWidth, float FrameHeight, float yOffset, int frameCount)
	{
		frames.resize(frameCount);
		
		for(unsigned int i = 0; i < frames.size(); i++)
		{
			frames[i].tex = texture;
			frames[i].textureOffset = vkhelper::calcTexOffset
				(texture.dim, glm::vec4(i * FrameWidth, yOffset, FrameWidth, FrameHeight));
			frames[i].delay = delay;
			totalDuration += delay;
			frames[i].size = glm::vec2(FrameWidth, FrameHeight);
		}
	}

	Animation(std::vector<Frame> frames)
	{
		this->frames = frames;
		for(const auto &f: frames)
			totalDuration += f.delay;
	}

	std::vector<Frame> getAllFrames()
	{
		return frames;
	}

	Frame getFrame(int index)
	{
		return frames[index];
	}
	

	Frame Play(Timer &timer)
	{
		frameTimer += timer.FrameElapsed();
		if(frameTimer > frames[current].delay)
		{
			frameTimer = 0;
			current++;
			if(current >= frames.size())
				current = 0;
		}
		return frames[current];
	}

	Frame PlayOnce(Timer &timer)
	{
		if(!done)
		{
		frameTimer += timer.FrameElapsed();
		if(frameTimer > frames[current].delay)
		{
			frameTimer = 0;
			current++;
			if(current >= frames.size())
			{
				done = true;
				current--;
			}
		}
		}
		return frames[current];
	}

	void Reset()
	{
		done = false;
		current = 0;
	}

	float getTotalDuration()
	{
		return totalDuration;
	}

private:
	unsigned int current = 0;
	std::vector<Frame> frames;
	float frameTimer = 0;
	float totalDuration = 0;
	bool done = false;
};


#endif