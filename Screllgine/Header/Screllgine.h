#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

#include "VDeleter.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

static VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

static void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr)
	{
		func(instance, callback, pAllocator);
	}
}

struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete()
	{
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class Screllgine
{
public:
	void Start();
	void Run();

	void DrawFrame();

	static glm::vec2 WindowSize();

private:
	void InitVulkan();
	void RecreateSwapChain();

	void InitInstance();
	void InitCallbacks();
	void InitSurface();
	void InitDevice();
	void InitLogicDevice();
	void InitSwapChain();
	void InitImageViews();
	void InitRenderPass();
	void InitDescriptor();
	void InitGraphicsPipeline();
	void InitFramebuffers();
	void InitCommandPool();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateUniformBuffer();
	void InitDescriptorPool();
	void InitDescriptorSet();
	void InitCommandBuffers();
	void InitSemaphores();

	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VDeleter<VkBuffer>& buffer, VDeleter<VkDeviceMemory>& bufferMemory);


	void InitGLFW();
	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();
	int RateDeviceSuitability(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	static std::vector<char> ReadFile(const std::string& filename);
	void CreateShaderModule(const std::vector<char>& code, VDeleter<VkShaderModule>& shaderModule);


	static void OnWindowResized(GLFWwindow* window, int width, int height);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void UpdateUniformBuffer();

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData);

private:

	GLFWwindow* m_gWindow;
	VkPhysicalDevice m_pDevice;
	VkQueue m_qGraphicsQueue;
	VkQueue m_qPresentQueue;
	VkFormat m_fSwapChainImageFormat;
	VkExtent2D m_eSwapChainExtent;
	VkDescriptorSet m_sDescriptorSet;

	std::vector<VkPhysicalDevice> m_svDevices;
	std::vector<VkImage> m_svSwapChainImages;
	std::vector<VkCommandBuffer> m_svCommandBuffers;

	static int Width;
	static int Height;

	VDeleter<VkDebugReportCallbackEXT> callback{ m_vInstance, DestroyDebugReportCallbackEXT };
	VDeleter<VkInstance> m_vInstance{ vkDestroyInstance };
	VDeleter<VkDevice> m_vDevice{ vkDestroyDevice };
	VDeleter<VkSurfaceKHR> m_vSurface{ m_vInstance, vkDestroySurfaceKHR };
	VDeleter<VkSwapchainKHR> m_vSwapChain{ m_vDevice, vkDestroySwapchainKHR };
	VDeleter<VkDescriptorSetLayout> m_vDescriptorSetLayout{ m_vDevice, vkDestroyDescriptorSetLayout };
	VDeleter<VkPipelineLayout> m_vPipelineLayout{ m_vDevice, vkDestroyPipelineLayout };
	VDeleter<VkRenderPass> m_vRenderPass{ m_vDevice, vkDestroyRenderPass };
	VDeleter<VkPipeline> m_vGraphicsPipeline{ m_vDevice, vkDestroyPipeline };
	VDeleter<VkCommandPool> commandPool{ m_vDevice, vkDestroyCommandPool };
	VDeleter<VkSemaphore> m_vImageAvailableSemaphore{ m_vDevice, vkDestroySemaphore };
	VDeleter<VkSemaphore> m_vRenderFinishedSemaphore{ m_vDevice, vkDestroySemaphore };
	VDeleter<VkBuffer> m_vVertexBuffer{ m_vDevice, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> m_vVertexBufferMemory{ m_vDevice, vkFreeMemory };
	VDeleter<VkBuffer> m_vIndexBuffer{ m_vDevice, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> m_vIndexBufferMemory{ m_vDevice, vkFreeMemory };
	VDeleter<VkBuffer> m_vUniformStagingBuffer{ m_vDevice, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> m_vUniformStagingBufferMemory{ m_vDevice, vkFreeMemory };
	VDeleter<VkBuffer> m_vUniformBuffer{ m_vDevice, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> m_vUniformBufferMemory{ m_vDevice, vkFreeMemory };
	VDeleter<VkDescriptorPool> m_vDescriptorPool{ m_vDevice, vkDestroyDescriptorPool };


	std::vector<VDeleter<VkImageView>> m_vSwapChainImageViews;
	std::vector<VDeleter<VkFramebuffer>> m_vSwapChainFramebuffers;


	VDeleter<VkShaderModule> vertShaderModule{ m_vDevice, vkDestroyShaderModule };
	VDeleter<VkShaderModule> fragShaderModule{ m_vDevice, vkDestroyShaderModule };
};