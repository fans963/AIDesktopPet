#pragma once

#include <string>
#include <vector>

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
                // if (ready())
                //     throw std::runtime_error("The interface has already been bound to somewhere");

                data_pointer_ = new T(std::forward<Args>(args)...);
                activated = true;
            }

            void bind_directly(T &destination)
            {
                // if (ready())
                //     throw std::runtime_error("The interface has already been bound to somewhere");

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

            T *data_pointer_ = nullptr;
            bool activated = false;
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
            bool activated = false;
        };

        template<typename T>
        void registerInput(const int&id,InputInterface<T>& interface) {
            // if(interface.active)
            //     throw std::runtime_error("The interface has been activated");
            inputList_.emplace_back(id, interface.activate);
        }

        void registerOutput() {}

    private:
        struct InputDeclaration
        {
            const int id;
            void **pointer_to_data_pointer;
        };

        struct OutputDeclaration
        {
            const int id;
            void *data_pointer;

            Component *component;
        };
        std::vector<InputDeclaration> inputList_;
        std::vector<OutputDeclaration> outputList_;
    };
};