#pragma once
#pragma once

#include "HTH_device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>


class HTH_descriptorSetLayout {
public:
    class Builder {
    public:
        Builder(HTH_device& device) : hth_device{device} {}

        Builder& addBinding(
            uint32_t binding,
            VkDescriptorType descriptorType,
            VkShaderStageFlags stageFlags,
            uint32_t count = 1);
        std::unique_ptr<HTH_descriptorSetLayout> build() const;

    private:
        HTH_device& hth_device;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
    };

    HTH_descriptorSetLayout(
        HTH_device& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
    ~HTH_descriptorSetLayout();
    HTH_descriptorSetLayout(const HTH_descriptorSetLayout&) = delete;
    HTH_descriptorSetLayout& operator=(const HTH_descriptorSetLayout&) = delete;

    VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

private:
    HTH_device& hth_device;
    VkDescriptorSetLayout descriptorSetLayout;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

    friend class HTH_descriptorWriter;
};

class HTH_descriptorPool {
public:
    class Builder {
    public:
        Builder(HTH_device& device) : hth_device{ device } {}

        Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
        Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
        Builder& setMaxSets(uint32_t count);
        std::unique_ptr<HTH_descriptorPool> build() const;

    private:
        HTH_device& hth_device;
        std::vector<VkDescriptorPoolSize> poolSizes{};
        uint32_t maxSets = 1000;
        VkDescriptorPoolCreateFlags poolFlags = 0;
    };

    HTH_descriptorPool(
        HTH_device& device,
        uint32_t maxSets,
        VkDescriptorPoolCreateFlags poolFlags,
        const std::vector<VkDescriptorPoolSize>& poolSizes);
    ~HTH_descriptorPool();
    HTH_descriptorPool(const  HTH_descriptorPool&) = delete;
    HTH_descriptorPool& operator=(const  HTH_descriptorPool&) = delete;

    bool allocateDescriptor(
        const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

    void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

    void resetPool();

private:
    HTH_device& hth_device;
    VkDescriptorPool descriptorPool;

    friend class HTH_descriptorWriter;
};

class HTH_descriptorWriter {
public:
    HTH_descriptorWriter(HTH_descriptorSetLayout& setLayout, HTH_descriptorPool& pool);

    HTH_descriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
    HTH_descriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

    bool build(VkDescriptorSet& set);
    void overwrite(VkDescriptorSet& set);

private:
    HTH_descriptorSetLayout& setLayout;
    HTH_descriptorPool& pool;
    std::vector<VkWriteDescriptorSet> writes;
};