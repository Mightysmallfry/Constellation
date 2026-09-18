#ifndef CONSTELLATION_SPARSESET_H
#define CONSTELLATION_SPARSESET_H

#include <cstdint>
#include <vector>

#include "Containers/Handle.h"

namespace constellation
{
    // Dense-array-backed sparse set keyed by Handle::Index, with a
    // per-slot generation check against Handle::Generation.
    //
    // NOTE ON HEADER-ONLY: unlike the rest of this library, this class
    // is a template, so its definition must stay in the header (or use
    // explicit instantiation, which isn't worth it for a container this
    // general-purpose). This is a deliberate exception to the
    // headers-declare/.cpp-defines convention, not a default.
    template <typename T>
    class SparseSet
    {
    public:
        Handle Insert(const T& value)
        {
            uint32_t index;
            if (!m_freeIndices.empty())
            {
                index = m_freeIndices.back();
                m_freeIndices.pop_back();
            }
            else
            {
                index = static_cast<uint32_t>(m_generations.size());
                m_generations.push_back(0);
                m_denseIndexForSlot.push_back(0);
            }

            const uint32_t generation = m_generations[index];
            m_dense.push_back(value);
            m_sparseIndexForDense.push_back(index);
            m_denseIndexForSlot[index] = static_cast<uint32_t>(m_dense.size() - 1);

            return Handle{ index, generation };
        }

        void Remove(Handle handle)
        {
            if (!Contains(handle))
            {
                return;
            }

            const uint32_t denseIndex = m_denseIndexForSlot[handle.Index];
            const uint32_t lastDenseIndex = static_cast<uint32_t>(m_dense.size() - 1);

            m_dense[denseIndex] = m_dense[lastDenseIndex];
            m_sparseIndexForDense[denseIndex] = m_sparseIndexForDense[lastDenseIndex];
            m_denseIndexForSlot[m_sparseIndexForDense[denseIndex]] = denseIndex;

            m_dense.pop_back();
            m_sparseIndexForDense.pop_back();

            m_generations[handle.Index]++;
            m_freeIndices.push_back(handle.Index);
        }

        bool Contains(Handle handle) const
        {
            return handle.Index < m_generations.size()
                && m_generations[handle.Index] == handle.Generation;
        }

        T* Get(Handle handle)
        {
            if (!Contains(handle))
            {
                return nullptr;
            }
            return &m_dense[m_denseIndexForSlot[handle.Index]];
        }

        const T* Get(Handle handle) const
        {
            if (!Contains(handle))
            {
                return nullptr;
            }
            return &m_dense[m_denseIndexForSlot[handle.Index]];
        }

        size_t Size() const { return m_dense.size(); }

        std::vector<T>& Dense() { return m_dense; }
        const std::vector<T>& Dense() const { return m_dense; }

    private:
        std::vector<T> m_dense;
        std::vector<uint32_t> m_sparseIndexForDense;
        std::vector<uint32_t> m_denseIndexForSlot;
        std::vector<uint32_t> m_generations;
        std::vector<uint32_t> m_freeIndices;
    };
}

#endif