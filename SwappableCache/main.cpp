#include <memory>
#include <string>
#include <vector>
using namespace std;
template <class T> class Cache {
  public:
    virtual void insert(int key, T value) = 0;
    virtual T get(int key) = 0;
    virtual vector<pair<int, T>> get_data() = 0;
};
template <class T> class LRUCache : public Cache<T> {
    // Doubly linked list
  public:
    void insert(int key, T value) {
        // LRU logic
    }
    T get(int key) {
        // LRU logic
    }
    vector<pair<int, T>> get_data() { return {}; }
};
template <class T> class FIFOCache : public Cache<T> {
    // queue
  public:
    void insert(int key, T value) {
        // FIFO logic
    }
    T get(int key) {
        // FIFO logic
    }
    vector<pair<int, T>> get_data() { return {}; }
};
template <class T> class DocumentCache {
    unique_ptr<Cache<T>> cache;

  public:
    DocumentCache(unique_ptr<Cache<T>> &&cache) {
        this->cache = std::move(cache);
    }
    T get(int key) { return cache->get(key); }
    void insert(int key, T value) { cache->insert(key, value); }
    void change_policy(unique_ptr<Cache<T>> &&new_cache) {
        auto old = cache->get_data();
        cache = std::move(new_cache);
        for (auto [key, value] : old)
            cache->insert(key, value);
    }
};
int main() {
    DocumentCache<string> c((make_unique<LRUCache<string>>()));
    c.change_policy(make_unique<FIFOCache<string>>());
    return 0;
}
