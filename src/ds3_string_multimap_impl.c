
#include "ds3_string_multimap.h"
#include "ds3_string_multimap_impl.h"

GHashTable* ds3_string_multimap_get_hashtable(ds3_string_multimap* mp) {
    struct _ds3_string_multimap* _mp = mp;
    return _mp->hash;
}

void ds3_string_multimap_set_hashtable(ds3_string_multimap* mp, GHashTable* ht) {
    struct _ds3_string_multimap* _mp = mp;
    _mp->hash = ht;
}

