#include "ds3_string_multimap.h"
#include <boost/test/unit_test.hpp>
#include <glib.h>

BOOST_AUTO_TEST_CASE( basic_put ) {
    ds3_string_multimap* map = ds3_string_multimap_init();

    ds3_string_multimap_insert(map, "key", "value");

    GPtrArray* values = ds3_string_multimap_lookup(map, "key");

    BOOST_CHECK(g_ptr_array_size(values) == 1);

    ds3_string_multimap_free(map);
}
