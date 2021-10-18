/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   fast_forward_png;
    const int            fast_forward_pngSize = 10274;

    extern const char*   fast_reverse_png;
    const int            fast_reverse_pngSize = 10276;

    extern const char*   file_png;
    const int            file_pngSize = 9293;

    extern const char*   loop_png;
    const int            loop_pngSize = 6742;

    extern const char*   folder_png;
    const int            folder_pngSize = 2151;

    extern const char*   save_png;
    const int            save_pngSize = 10075;

    extern const char*   info_png;
    const int            info_pngSize = 15493;

    extern const char*   volume_png;
    const int            volume_pngSize = 18570;

    extern const char*   shuffle_png;
    const int            shuffle_pngSize = 12098;

    extern const char*   mute_png;
    const int            mute_pngSize = 8561;

    extern const char*   pause_png;
    const int            pause_pngSize = 7799;

    extern const char*   play_png;
    const int            play_pngSize = 9320;

    extern const char*   restart_png;
    const int            restart_pngSize = 6742;

    extern const char*   stop_png;
    const int            stop_pngSize = 7522;

    extern const char*   insert_png;
    const int            insert_pngSize = 11458;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 15;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
