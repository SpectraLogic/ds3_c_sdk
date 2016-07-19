#include "ds3.h"
#include "ds3_string.h"
#include "ds3_string_multimap.h"
#include <boost/test/unit_test.hpp>
#include <glib.h>

BOOST_AUTO_TEST_CASE( string_multimap_insert_and_lookup ) {
    printf("-----Testing string_multimap insert_and_lookup-------\n");
    ds3_string_multimap* map = ds3_string_multimap_init();
    ds3_str* key = ds3_str_init("key");
    ds3_str* value = ds3_str_init("value");

    ds3_string_multimap_insert(map, key, value);

    ds3_string_multimap_entry* entry = ds3_string_multimap_lookup(map, key);

    BOOST_CHECK(ds3_string_multimap_entry_get_num_values(entry) == 1);
    ds3_str* entry_key = ds3_string_multimap_entry_get_key(entry);
    ds3_str* entry_value = ds3_string_multimap_entry_get_value_by_index(entry, 0);
    BOOST_CHECK_EQUAL(0, g_strcmp0(entry_key->value, key->value));
    BOOST_CHECK_EQUAL(0, g_strcmp0(entry_value->value, value->value));

    ds3_str_free(key);
    ds3_str_free(value);
    ds3_str_free(entry_key);
    ds3_str_free(entry_value);
    ds3_string_multimap_free(map);
    ds3_string_multimap_entry_free(entry);
}
