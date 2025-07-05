target_sources(PraxInternal PRIVATE
        "magic_enum.hpp"
        "magic_enum_all.hpp"
        "magic_enum_containers.hpp"
        "magic_enum_flags.hpp"
        "magic_enum_format.hpp"
        "magic_enum_fuse.hpp"
        "magic_enum_iostream.hpp"
        "magic_enum_switch.hpp"
        "magic_enum_utility.hpp"
)

target_include_directories(PraxInternal PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
