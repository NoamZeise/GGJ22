#ifndef RENDER_H
#define RENDER_H

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>
#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <stdexcept>
#include <iostream>
#include <string>
#include <cstring>
#include <atomic>

#include "vkinit.h"
#include "vkhelper.h"
#include "render_structs.h"
#include "pipeline.h"
#include "descriptor_sets.h"
#include "texture_loader.h"
#include "texfont.h"
#include "model_loader.h"

class Render
{
public:
	Render(GLFWwindow* window);
	Render(GLFWwindow* window, glm::vec2 target);
	void setViewMatrixAndFov(glm::mat4 view, float fov);
	void set2DViewMatrix(glm::mat4 view);
	~Render();
	Resource::Texture LoadTexture(std::string filepath);
	Resource::Font* LoadFont(std::string filepath);
	#ifndef ONLY_2D
	Resource::Model LoadModel(std::string filepath);
	#endif
	void endResourceLoad();
#ifndef ONLY_2D
	void begin3DDraw();
	#endif
	void begin2DDraw();

	void endDraw(std::atomic<bool>& submit);
	#ifndef ONLY_2D
	void DrawModel(Resource::Model model, glm::mat4 modelMatrix, glm::mat4 normalMatrix);
	#endif
	void DrawQuad(const Resource::Texture& texID, glm::mat4 modelMatrix, glm::vec4 colour);
	void DrawQuad(const Resource::Texture& texID, glm::mat4 modelMatrix, glm::vec4 colour, glm::vec4 texOffset);
	void DrawString(Resource::Font* font, std::string text, glm::vec2 position, float size, float rotate, glm::vec4 colour);
  	float MeasureString(Resource::Font* font, std::string text, float size);
	bool framebufferResized = false;
private:
	GLFWwindow* mWindow;
	glm::vec2 targetResolution;
	VkInstance mInstance;
	VkSurfaceKHR mSurface;
	Base mBase;
	FrameData mFrame;
	SwapChain mSwapchain;
	VkRenderPass mRenderPass;

	VkCommandPool mGeneralCommandPool;
	VkCommandBuffer mTransferCommandBuffer;

#ifndef ONLY_2D
	Pipeline pipeline3D;
#endif
	Pipeline pipeline2D;

	//descriptor set members
	VkDeviceMemory shaderMemory;
	VkBuffer shaderBuffer;
	#ifndef ONLY_2D
	DS::ShaderBufferSet mViewproj3DUbo;
	#endif
	DS::ShaderBufferSet mViewproj2DUbo;
	DS::ShaderBufferSet mPerInstanceSSBO;
	DS::ShaderBufferSet mLightingUbo;
	DS::DescriptorSet mTexturesDS;

	DS::viewProjection viewProjectionData3D;
	DS::viewProjection viewProjectionData2D;
	DS::lighting lightingData;
	DS::PerInstance perInstanceData;

	Resource::TextureLoader mTextureLoader;
	Resource::ModelLoader mModelLoader;
	#ifndef ONLY_2D
	bool m3DRender = true;
	Resource::Model currentModel;
	#endif	
	unsigned int modelRuns = 0;
	bool mBegunDraw = false;
	bool mFinishedLoadingResources = false;

	uint32_t mImg;
	VkSemaphore mImgAquireSem;
	float projectionFov = 45.0f;


	unsigned int currentIndex = 0;

	Resource::Texture currentTexture;
	glm::vec4 currentTexOffset = glm::vec4(0, 0, 1, 1);
	glm::vec4 currentColour = glm::vec4(1, 1, 1, 1);
	
	void initRender(GLFWwindow* window);
	void initFrameResources();
	void destroyFrameResources();
	void startDraw();
	void resize();
	void updateViewProjectionMatrix();
	void update2DProj();
	void drawBatch();

#ifndef NDEBUG
	VkDebugUtilsMessengerEXT mDebugMessenger;
#endif

};





#endif
