//
// Created by xiaoyuyu on 2022/6/8.
//

#include <list>
#include <mutex>
#include <memory>
#include <vector>
#include <shared_mutex> // c++17，不然就用boost，书上用的是boost

#ifndef MYCONCURRENCY_TLREADSAFE_MAP_HPP
#define MYCONCURRENCY_TLREADSAFE_MAP_HPP

template <typename Key, typename Value, typename Hash>
class threadsafe_map {
private:
    class bucket {
    private:
        using bucket_elem = std::pair<Key, Value>;
        using bucket_lst = std::list<bucket_elem>;
        using bucket_iter = typename bucket_lst::iterator;

        bucket_lst lst;
        mutable std::shared_mutex m;
    private:
        bucket_iter get_from_key(const Key& key) {
            bucket_iter res = std::find_if(lst.begin(), lst.end(), [&](const bucket_elem& elem) -> bool {
                return elem.first == key;
            });
            return res;
        }
    public:
        Value get_value(const Key& key, const Value& value) {
            // 加锁，同一时间能有大量线程访问，只读
            std::shared_lock<std::shared_mutex> lock(m);
            bucket_iter temp = get_from_key(key);
            if (temp != lst.end()) {
                return temp->second;
            } else {
                // 如果访问失败，按书上的需要返回一个默认值
                return value;
            }
        }

        void add_and_update(const Key& key, const Value& value) {
            // 加锁，同一时间只能有一个线程访问
            std::lock_guard<std::shared_mutex> lock(m);
            bucket_iter temp = get_from_key(key);
            if (temp != lst.end()) {
                 temp->second = value;
            } else {
                lst.push_back(bucket_elem(key, value));
            }
        }

        void remove_elem(const Key& key) {
            // 加锁，同一时间只能有一个线程访问
            std::lock_guard<std::shared_mutex> lock(m);
            bucket_iter temp = get_from_key(key);
            if (temp != lst.end()) {
                throw std::logic_error("can't remove which hasn't been added.\n");
            } else {
                lst.erase(temp);
            }
        }
    };

    std::vector<std::unique_ptr<bucket>> buckets;
    Hash hash;

    bucket& get_bucket(const Key& key) {
        size_t idx = hash(key) % buckets.size();
        return *buckets[idx];
    }

public:
    /*
     * 构造函数，给定一个确定的桶的数量
     * 禁用拷贝构造函数和拷贝赋值函数
     * 移动可以开启，暂时用默认的好了，先不测试
     * */
    threadsafe_map(int buckets_num = 11, const Hash& _hash = Hash()) : buckets(buckets_num), hash(_hash) {
        // vector有一个构造函数 explicit vector( size_type count );
        // vector默认构造函数，创建的是空指针
        for (int i = 0; i < buckets_num; i++) {
            buckets[i].reset(new bucket);
        }
    }

    Value get_value(const Key& key, const Value& default_value = Value()) {
        return get_bucket(key).get_value(key, default_value);
    }

    void add_and_update(const Key& key, const Value& value) {
        get_bucket(key).add_and_update(key, value);
    }

    void remove_elem(const Key& key) {
        get_bucket(key).remove_elem(key);
    }
};


#endif //MYCONCURRENCY_TLREADSAFE_MAP_HPP
