#pragma once
#include "plf_colony.hh"
#include <mutex>
#include <stdexcept>

namespace newt::ecs {
    template<typename T>
    class database_set {
        std::mutex mutex_;
        plf::colony<T> set;
        bool listening_for_inserted_values_ = false;
        std::vector<T*> inserted_values;
        public:
            inline std::mutex& mutex() {
                return mutex_;
            }

            // Whether or not to listen for insertion and add the values to the inserted_values vector
            inline bool& listening_for_inserted_values() {
                return listening_for_inserted_values_;
            }

            inline std::size_t size() const { return set.size(); }
        
            T* insert(const T& value) {
                // If we are listening for inserted values, add the index to the inserted_values vector
                if (listening_for_inserted_values_) {
                    inserted_values.push_back(&(*set.insert(value)));
                    return inserted_values.back();
                } else {
                    return &(*set.insert(value));
                }
            }

            void erase(T* value) {
                // If we are listening for inserted values, remove the index from the inserted_values vector
                if (listening_for_inserted_values_) {
                    // Since std::erase_if would be less efficient, we have to do it manually
                    for (auto it = inserted_values.begin(); it != inserted_values.end(); ++it) {
                        if (*it == index) {
                            inserted_values.erase(it);
                            break;
                        }
                    }
                }
                set.erase(set.get_iterator(value));
            }

            void clear_inserted_values() {
                inserted_values.clear();
            }

            std::vector<std::size_t> copy_inserted_values() const {
                if (!listening_for_inserted_values_) {
                    throw std::runtime_error("listening_for_inserted_values must be true to use this function");
                }
                return inserted_values;
            }

            inline auto begin() {
                return set.begin();
            }

            inline auto end() {
                return set.end();
            }

            inline auto begin() const {
                return set.begin();
            }

            inline auto end() const {
                return set.end();
            }
    };
}