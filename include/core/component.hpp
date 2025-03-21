#pragma once

#include <string>
#include <vector>
#include "log.hpp"

namespace core
{
    class Component
    {
    public:
        friend class Executor;

        explicit Component() {}
        ~Component() {}

        virtual void beforeUpdate() {}
        virtual void update() {}

        template <typename T>
        struct InputInterface
        {
        public:
            friend class Component;
            InputInterface() = default;

            InputInterface(const InputInterface &) = delete;
            InputInterface &operator=(const InputInterface &) = delete;
            InputInterface(InputInterface &&) = delete;
            InputInterface &operator=(InputInterface &&) = delete;

            ~InputInterface()
            {
                if constexpr (std::is_array<T>::value)
                    delete[] data_pointer_;
                else
                    delete data_pointer_;
            }

            [[nodiscard]] bool active() const { return activated; }
            [[nodiscard]] bool ready() const { return data_pointer_ != nullptr; }

            template <typename... Args>
            void make_and_bind_directly(Args &&...args)
            {
#ifdef DEBUG
                if (ready())
                    logger::error("The interface has already been bound to somewhere");

                data_pointer_ = new T(std::forward<Args>(args)...);
#endif

                activated = true;
            }

            void bind_directly(T &destination)
            {
#ifdef DEBUG
                if (ready())
                    logger::error("The interface has already been bound to somewhere");
#endif

                data_pointer_ = &destination;
                activated = true;
            }

            const T *operator->() const { return data_pointer_; }
            const T &operator*() const { return *data_pointer_; }

        private:
            void **activate()
            {
                activated = true;
                return reinterpret_cast<void **>(&data_pointer_);
            }

            T *data_pointer_ {nullptr};
            bool activated {false};
        };

        template <typename T>
        struct OutputInterface
        {
        public:
            friend class Component;
            OutputInterface() = default;

            OutputInterface(const OutputInterface &) = delete;
            OutputInterface &operator=(const OutputInterface &) = delete;
            OutputInterface(OutputInterface &&) = delete;
            OutputInterface &operator=(OutputInterface &&) = delete;

            ~OutputInterface()
            {
                if (active())
                    delete reinterpret_cast<T *>(data_);
            }

            [[nodiscard]] bool active() const { return activated; }

            T *operator->() { return reinterpret_cast<T *>(&data_); }
            const T *operator->() const { return reinterpret_cast<const T *>(&data_); }
            T &operator*() { return *reinterpret_cast<T *>(&data_); }
            const T &operator*() const { return *reinterpret_cast<const T *>(&data_); }

        private:
            template <typename... Args>
            void *activate(Args &&...args)
            {
                ::new (&data_) T(std::forward<Args>(args)...);
                activated = true;
                return reinterpret_cast<void *>(&data_);
            }

            std::aligned_storage<sizeof(T), alignof(T)> data_;
            bool activated{false};
        };

        template <typename T>
        void registerInput(const uint8_t &id, InputInterface<T> &interface)
        {
#ifdef DEBUG
            if (interface.active)
                logger::error("The interface has been activated");
#endif
            inputList_.emplace_back(id, interface.activate);
        }

        template <typename T, typename... Args>
        void registerOutput(const uint8_t &id, OutputInterface<T> &interface, Args &&...args)
        {
#ifdef DEBUG
            if (interface.active())
                logger::error("The interface has been activated");
#endif
            outputList_.emplace_back(OutputDeclaration{id, interface.activate(std::forward<Args>(args)...)});
        }

    private:
        struct InputDeclaration
        {
            const uint8_t id;
            void **pointer_to_data_pointer;
        };

        struct OutputDeclaration
        {
            const uint8_t id;
            void *data_pointer;
        };
        std::vector<InputDeclaration> inputList_;
        std::vector<OutputDeclaration> outputList_;
    };
};