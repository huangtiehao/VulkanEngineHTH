#include "HTH_descriptors.hpp"

// std
#include <cassert>
#include <stdexcept>
    // *************** Descriptor Set Layout Builder *********************

HTH_descriptorSetLayout::Builder& HTH_descriptorSetLayout::Builder::addBinding(
    uint32_t binding,
    VkDescriptorType descriptorType,
    VkShaderStageFlags stageFlags,
    uint32_t count) {
    assert(bindings.count(binding) == 0 && "Binding already in use");
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = count;
    layoutBinding.stageFlags = stageFlags;
    bindings[binding] = layoutBinding;
    return *this;
}

std::unique_ptr<HTH_descriptorSetLayout> HTH_descriptorSetLayout::Builder::build() const {
    return std::make_unique<HTH_descriptorSetLayout>(hth_device, bindings);
}

// *************** Descriptor Set Layout *********************

HTH_descriptorSetLayout::HTH_descriptorSetLayout(
    HTH_device& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
    : hth_device{ device }, bindings{ bindings } {
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
    for (auto kv : bindings) {
        setLayoutBindings.push_back(kv.second);
    }

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
    descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

    if (vkCreateDescriptorSetLayout(
        device.device(),
        &descriptorSetLayoutInfo,
        nullptr,
        &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

HTH_descriptorSetLayout::~HTH_descriptorSetLayout() {
    vkDestroyDescriptorSetLayout(hth_device.device(), descriptorSetLayout, nullptr);
}

// *************** Descriptor Pool Builder *********************

HTH_descriptorPool::Builder& HTH_descriptorPool::Builder::addPoolSize(
    VkDescriptorType descriptorType, uint32_t count) {
    poolSizes.push_back({ descriptorType, count });
    return *this;
}

HTH_descriptorPool::Builder& HTH_descriptorPool::Builder::setPoolFlags(
    VkDescriptorPoolCreateFlags flags) {
    poolFlags = flags;
    return *this;
}
HTH_descriptorPool::Builder& HTH_descriptorPool::Builder::setMaxSets(uint32_t count) {
    maxSets = count;
    return *this;
}

std::unique_ptr<HTH_descriptorPool> HTH_descriptorPool::Builder::build() const {
    return std::make_unique<HTH_descriptorPool>(hth_device, maxSets, poolFlags, poolSizes);
}

// *************** Descriptor Pool *********************

HTH_descriptorPool::HTH_descriptorPool(
    HTH_device& device,
    uint32_t maxSets,
    VkDescriptorPoolCreateFlags poolFlags,
    const std::vector<VkDescriptorPoolSize>& poolSizes)
    : hth_device{ device } {
    VkDescriptorPoolCreateInfo descriptorPoolInfo{};
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    descriptorPoolInfo.pPoolSizes = poolSizes.data();
    descriptorPoolInfo.maxSets = maxSets;
    descriptorPoolInfo.flags = poolFlags;

    if (vkCreateDescriptorPool(device.device(), &descriptorPoolInfo, nullptr, &descriptorPool) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

HTH_descriptorPool::~HTH_descriptorPool() {
    vkDestroyDescriptorPool(hth_device.device(), descriptorPool, nullptr);
}

bool HTH_descriptorPool::allocateDescriptor(
    const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const {
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.pSetLayouts = &descriptorSetLayout;
    allocInfo.descriptorSetCount = 1;

    // Might want to create a "DescriptorPoolManager" class that handles this case, and builds
    // a new pool whenever an old pool fills up. But this is beyond our current scope
    if (vkAllocateDescriptorSets(hth_device.device(), &allocInfo, &descriptor) != VK_SUCCESS) {
        return false;
    }
    return true;
}

void HTH_descriptorPool::freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const {
    vkFreeDescriptorSets(
        hth_device.device(),
        descriptorPool,
        static_cast<uint32_t>(descriptors.size()),
        descriptors.data());
}

void HTH_descriptorPool::resetPool() {
    vkResetDescriptorPool(hth_device.device(), descriptorPool, 0);
}

// *************** Descriptor Writer *********************

HTH_descriptorWriter::HTH_descriptorWriter(HTH_descriptorSetLayout& setLayout, HTH_descriptorPool& pool)
    : setLayout{ setLayout }, pool{ pool } {}

HTH_descriptorWriter& HTH_descriptorWriter::writeBuffer(
    uint32_t binding, VkDescriptorBufferInfo* bufferInfo) {
    assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

    auto& bindingDescription = setLayout.bindings[binding];

    assert(
        bindingDescription.descriptorCount == 1 &&
        "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pBufferInfo = bufferInfo;
    write.descriptorCount = 1;

    writes.push_back(write);
    return *this;
}

HTH_descriptorWriter& HTH_descriptorWriter::writeImage(
    uint32_t binding, VkDescriptorImageInfo* imageInfo) {
    assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

    auto& bindingDescription = setLayout.bindings[binding];

    assert(
        bindingDescription.descriptorCount == 1 &&
        "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pImageInfo = imageInfo;
    write.descriptorCount = 1;

    writes.push_back(write);
    return *this;
}

bool HTH_descriptorWriter::build(VkDescriptorSet& set) {
    bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
    if (!success) {
        return false;
    }
    overwrite(set);
    return true;
}

void HTH_descriptorWriter::overwrite(VkDescriptorSet& set) {
    for (auto& write : writes) {
        write.dstSet = set;
    }
    vkUpdateDescriptorSets(pool.hth_device.device(), writes.size(), writes.data(), 0, nullptr);
}
