#pragma once

#include "logger.hpp"

namespace blackhole {

class scoped_attributes_t :
    public scoped_attributes_concept_t
{
    // Attributes provided by this guard.
    mutable log::attributes_t m_guard_attributes;
    // Merged attributes provided by this guard and all the parent guards.
    // This value is computed lazily.
    mutable log::attributes_t m_merged_attributes;

public:
    scoped_attributes_t(logger_base_t &logger, log::attributes_t&& attributes) :
        scoped_attributes_concept_t(logger),
        m_guard_attributes(std::move(attributes))
    { }

    virtual
    const log::attributes_t&
    attributes() const {
        if (m_merged_attributes.empty()) {
            m_merged_attributes = std::move(m_guard_attributes);
            if (has_parent()) {
                const auto &parent_attributes = parent().attributes();
                m_merged_attributes.insert(parent_attributes.begin(), parent_attributes.end());
            }
        }
        return m_merged_attributes;
    }
};

} // namespace blackhole